#include "wireless.h"

__IO uint32_t Timeout = FLAG_TIMEOUT;

int TIMEOUT_UserCallback() {
	while (1);
}

uint8_t CC2500_state;

void delay(long num_ticks){
	while(num_ticks-- > 0);
}

uint8_t CC2500_Strobe(uint8_t Strobe){
	CC2500_CS_LOW();

	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE)== RESET);	// check flag for transmission to be RESET
	SPI_I2S_SendData(SPI1, Strobe);												// condition satisfied --> send command strobe

	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_BSY) == SET);		// check flag for being busy to be SET
	CC2500_state = (SPI_I2S_ReceiveData(SPI1) & 0x70) >> 4;											// set status to most recent received data on SPI1

	// Set chip select High at the end of the transmission
	CC2500_CS_HIGH();   
	return CC2500_state;
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
	
	CC2500_CS_LOW();
	delay(100);
	CC2500_CS_HIGH();
	delay(100);
	CC2500_CS_LOW();
	delay(150);
	
	// Send reset command
	CC2500_Strobe(SRES); 
	
	while(!GPIO_ReadInputDataBit(CC2500_SPI_GPIO_PORT, CC2500_SPI_MISO_PIN));
		
	CC2500_CS_HIGH();
	
	// Set to IDLE state
	CC2500_Strobe(SIDLE);
}


// Sends a byte through SPI protocol
static uint8_t SendByte(uint8_t byte) {
	
	// Loop while DR register is not empty
  Timeout = FLAG_TIMEOUT;
	while (SPI_I2S_GetFlagStatus(CC2500_SPI, SPI_I2S_FLAG_TXE) == RESET){
		if((Timeout--) == 0) return TIMEOUT_UserCallback();
  }
  
  // Send a Byte through the SPI peripheral
  SPI_I2S_SendData(CC2500_SPI, (uint16_t)byte);
  
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
  printf ("Read Add: %i", address);
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
	printf ("Write Add: %i", address);
	// start SPI
	CC2500_CS_LOW();
	
	// send desired destination address of write
	SendByte(address);
	
	// send each byte in the buffer to registers through SPI
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


