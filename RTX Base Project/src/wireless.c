#include "wireless.h"

__IO uint32_t Timeout = FLAG_TIMEOUT;

/* Read/Write command */
#define READWRITE_CMD              ((uint8_t)0x80) 
/* Multiple byte read/write command */ 
#define MULTIPLEBYTE_CMD           ((uint8_t)0x40)
/* Dummy Byte Send by the SPI Master device in order to generate the Clock to the Slave device */
#define DUMMY_BYTE                 ((uint8_t)0x00)

int TIMEOUT_UserCallback() {
	while (1);
}

// Initializes the low level interface used to drive the wireless components
static void LowLevel_Init(void){
  GPIO_InitTypeDef GPIO_InitStructure;
  SPI_InitTypeDef  SPI_InitStructure;

  // Enable the SPI periph
  RCC_APB2PeriphClockCmd(CC2500_SPI_CLK, ENABLE);

  // Enable NSS, SCK, MOSI and MISO GPIO clocks
	// PA4, PA5, PA7, PA6 */
	RCC_AHB1PeriphClockCmd(CC2500_SPI_GPIO_CLK, ENABLE);
	
	// Alternate Functions
	GPIO_PinAFConfig(CC2500_SPI_GPIO_PORT, CC2500_SPI_NSS_PIN, CC2500_SPI_GPIO_AF); // NSS - not slave select, similar to chip select
  GPIO_PinAFConfig(CC2500_SPI_GPIO_PORT, CC2500_SPI_SCK_PIN, CC2500_SPI_GPIO_AF); // SCK
  GPIO_PinAFConfig(CC2500_SPI_GPIO_PORT, CC2500_SPI_MISO_PIN, CC2500_SPI_GPIO_AF); // MISO
	GPIO_PinAFConfig(CC2500_SPI_GPIO_PORT, CC2500_SPI_MOSI_PIN, CC2500_SPI_GPIO_AF); // MOSI

  // Enable CS  GPIO clock
	// use GPIOA again
  //RCC_AHB1PeriphClockCmd(LIS302DL_SPI_CS_GPIO_CLK, ENABLE);
	
  GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_DOWN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

  // SPI SCK pin configuration
  GPIO_InitStructure.GPIO_Pin = CC2500_SPI_SCK_PIN;
  GPIO_Init(CC2500_SPI_GPIO_PORT, &GPIO_InitStructure);

  // SPI  MOSI pin configuration
  GPIO_InitStructure.GPIO_Pin = CC2500_SPI_MOSI_PIN;
  GPIO_Init(CC2500_SPI_GPIO_PORT, &GPIO_InitStructure);

  //SPI MISO pin configuration
  GPIO_InitStructure.GPIO_Pin = CC2500_SPI_MISO_PIN;
  GPIO_Init(CC2500_SPI_GPIO_PORT, &GPIO_InitStructure);
	
	// NSS pin configuration
	GPIO_InitStructure.GPIO_Pin = CC2500_SPI_NSS_PIN;
	GPIO_Init(CC2500_SPI_GPIO_PORT, &GPIO_InitStructure);

  /* SPI configuration -------------------------------------------------------*/
	// make sure you know why these are this way
  SPI_I2S_DeInit(CC2500_SPI);
  SPI_InitStructure.SPI_Direction 				= SPI_Direction_2Lines_FullDuplex;
  SPI_InitStructure.SPI_DataSize 					= SPI_DataSize_8b;
  SPI_InitStructure.SPI_CPOL 							= SPI_CPOL_Low; // polarity as per section 3.1 of configuring cc2500
  SPI_InitStructure.SPI_CPHA 							= SPI_CPHA_1Edge; // phase as per section 3.1 of configuring cc2500
  SPI_InitStructure.SPI_NSS 							= SPI_NSS_Soft;
  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;
  SPI_InitStructure.SPI_FirstBit 					= SPI_FirstBit_MSB;
  SPI_InitStructure.SPI_CRCPolynomial 		= 7;
  SPI_InitStructure.SPI_Mode 							= SPI_Mode_Master; // master mode as per section 3.1
  SPI_Init(CC2500_SPI, &SPI_InitStructure);

  // Enable SPI1
  SPI_Cmd(CC2500_SPI, ENABLE);

  // Configure GPIO PIN for Lis Chip select
//  GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_9;
//  GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_OUT;
//  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
//  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//  GPIO_Init(GPIOA, &GPIO_InitStructure);

  // Deselect : Chip Select high
  GPIO_SetBits(CC2500_SPI_GPIO_PORT, CC2500_SPI_NSS_PIN);
}


// Sends a byte through SPI protocol
static uint8_t SendByte(uint8_t byte) {
	
	// Loop while DR register is not empty
  Timeout = FLAG_TIMEOUT;
	while (SPI_I2S_GetFlagStatus(CC2500_SPI, SPI_I2S_FLAG_TXE) == RESET){
		if((Timeout--) == 0) return TIMEOUT_UserCallback();
  }
  
  // Send a Byte through the SPI peripheral
  SPI_I2S_SendData(SPI1, (uint16_t)byte);
  
	// Wait to receive a Byte
  Timeout = FLAG_TIMEOUT;
  while (SPI_I2S_GetFlagStatus(CC2500_SPI, SPI_I2S_FLAG_RXNE) == RESET){
    if((Timeout--) == 0) return TIMEOUT_UserCallback();
  }
  
  // Return the Byte read from the SPI bus
  return (uint8_t)SPI_I2S_ReceiveData(CC2500_SPI);
}

void SPI_Read(uint8_t* pBuffer, uint8_t address, uint16_t bytesToRead) {
	
	// enable Read mode
	uint8_t RW_mode = 0x80;
	
	// if more than 1 byte to read, enable burst mode
	uint8_t burst_mode = 0x00;
	if (bytesToRead > 0x01){
		burst_mode = 0x40;
	}
	
	// concatenate R/W and Burst bits to address
	address = RW_mode | burst_mode | address;

	// start SPI
	CC2500_CS_LOW();
	
	// send desired source address of read
	SendByte(address);
	
	// for each byte to read, send a dummy byte through SPI
	// send byte returned by SPI to buffer
	while (bytesToRead > 0x00){
		*pBuffer = SendByte (0x00);
		pBuffer ++;
		bytesToRead--;
	}
		
	// end SPI
	CC2500_CS_HIGH();
}

void SPI_Write(uint8_t* pBuffer, uint8_t address, uint16_t bytesToWrite) {
			
	// if more than 1 byte to write, enable burst mode
	uint8_t burst_mode = 0x00;
	if (bytesToWrite > 0x01){
		burst_mode = 0x40;
	}
	
	// concatenate Burst bit to address
	address = burst_mode | address;
	
	// start SPI
	CC2500_CS_LOW();
	
	// send desired source address of read
	SendByte(address);
	
	// for each byte in the buffer to registers through SPI
	while (bytesToWrite > 0x00){
		SendByte (*pBuffer);
		pBuffer ++;
		bytesToWrite--;
	}
		
	// end SPI
	CC2500_CS_HIGH();
}

void wireless_Init() {
	uint8_t crtl1 = 0x00;
	LowLevel_Init();
	// TODO: control registers?
	
	// automatic power on
	
}


