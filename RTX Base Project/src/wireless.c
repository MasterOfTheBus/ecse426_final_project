#include "wireless.h"
#include "cc2500_settings.h"

__IO uint32_t Timeout = FLAG_TIMEOUT;

int TIMEOUT_UserCallback(void) {
	while (1);
}

uint8_t CC2500_state;

void delay(long num_ticks){
	while(num_ticks-- > 0);
}

uint8_t CC2500_Strobe(uint8_t Strobe){
	CC2500_CS_LOW();

	while (SPI_I2S_GetFlagStatus(CC2500_SPI, SPI_I2S_FLAG_TXE)== RESET);	// check flag for transmission to be RESET
	SPI_I2S_SendData(CC2500_SPI, Strobe);												// condition satisfied --> send command strobe

	while (SPI_I2S_GetFlagStatus(CC2500_SPI, SPI_I2S_FLAG_BSY) == SET);		// check flag for being busy to be SET
	CC2500_state = (SPI_I2S_ReceiveData(CC2500_SPI) & 0x70) >> 4;											// set status to most recent received data on SPI1

	// Set chip select High at the end of the transmission
	CC2500_CS_HIGH();   
	return CC2500_state;
}

// Initializes the low level interface used to drive the wireless components
static void LowLevel_Init(void){
  
	GPIO_InitTypeDef GPIO_InitStructure;
  SPI_InitTypeDef  SPI_InitStructure;

  // Enable the SPI periph
  RCC_APB1PeriphClockCmd(CC2500_SPI_CLK, ENABLE);
  
	// Enable NSS, SCK, MOSI and MISO GPIO clocks
	RCC_AHB1PeriphClockCmd(CC2500_SPI_GPIO_CLK, ENABLE);
	
	// Enable CSn clock
	//RCC_AHB1PeriphClockCmd(CC2500_SPI_CS_GPIO_CLK, ENABLE);
	
	// Alternate Functions
  GPIO_PinAFConfig(CC2500_SPI_GPIO_PORT, CC2500_SPI_SCK_SOURCE, CC2500_SPI_GPIO_AF); // SCK
  GPIO_PinAFConfig(CC2500_SPI_GPIO_PORT, CC2500_SPI_MISO_SOURCE, CC2500_SPI_GPIO_AF); // MISO
	GPIO_PinAFConfig(CC2500_SPI_GPIO_PORT, CC2500_SPI_MOSI_SOURCE, CC2500_SPI_GPIO_AF); // MOSI

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

  /* SPI configuration -------------------------------------------------------*/
	// make sure you know why these are this way
  SPI_I2S_DeInit(CC2500_SPI);
  SPI_InitStructure.SPI_Direction 				= SPI_Direction_2Lines_FullDuplex;
  SPI_InitStructure.SPI_DataSize 					= SPI_DataSize_8b;
  SPI_InitStructure.SPI_CPOL 							= SPI_CPOL_Low; // polarity as per section 3.1 of configuring cc2500
  SPI_InitStructure.SPI_CPHA 							= SPI_CPHA_1Edge; // phase as per section 3.1 of configuring cc2500
  SPI_InitStructure.SPI_NSS 							= SPI_NSS_Soft;
  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16;
  SPI_InitStructure.SPI_FirstBit 					= SPI_FirstBit_MSB;
  SPI_InitStructure.SPI_CRCPolynomial 		= 7;
  SPI_InitStructure.SPI_Mode 							= SPI_Mode_Master; // master mode as per section 3.1
  SPI_Init(CC2500_SPI, &SPI_InitStructure);

  // Enable SPI1
  SPI_Cmd(CC2500_SPI, ENABLE);

  // Configure GPIO PIN for Lis Chip select
  GPIO_InitStructure.GPIO_Pin 	= CC2500_SPI_NSS_PIN;
  GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(CC2500_SPI_CS_GPIO_PORT, &GPIO_InitStructure);

  // Deselect : Chip Select high
  GPIO_SetBits(CC2500_SPI_CS_GPIO_PORT, CC2500_SPI_NSS_PIN);
	
	CC2500_CS_LOW();
	delay(100);
	CC2500_CS_HIGH();
	delay(100);
	CC2500_CS_LOW();
	delay(150);
	
	// Send reset command
	CC2500_Strobe(SRES); 
	
	while ((GPIO_ReadInputDataBit(CC2500_SPI_GPIO_PORT, CC2500_SPI_MISO_PIN) & 0x80) == 0x80);
	//while(!GPIO_ReadInputDataBit(CC2500_SPI_GPIO_PORT, CC2500_SPI_MISO_PIN));
		
	CC2500_CS_HIGH();
	
	// Set to IDLE state
	//CC2500_Strobe(SIDLE);
}


// Sends a byte through SPI protocol
static uint8_t SendByte(uint8_t byte) {
	
	// Loop while DR register is not empty
  Timeout = FLAG_TIMEOUT;
	while (SPI_I2S_GetFlagStatus(CC2500_SPI, SPI_I2S_FLAG_TXE) == RESET){
		if((Timeout--) == 0) return TIMEOUT_UserCallback();
  }
  
  // Send a Byte through the SPI peripheral
  SPI_I2S_SendData(CC2500_SPI, byte);
  
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
  printf ("Read Add: %i \n", address);
	// start SPI
	CC2500_CS_LOW();
	
	// send desired source address of read
	SendByte(address);
	
	// for each byte to read, send a dummy byte through SPI
	// send byte returned by SPI to buffer
	while (bytesToRead > 0x00){
		*pBuffer = SendByte (0x00);
		printf("Value read: %i\n", *pBuffer);
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
	
	printf("bytes to write: %i\n", bytesToWrite);
	
	// concatenate Burst bit to address
	address = burst_mode | address;
	printf ("Write Add: %i \n", address);
	// start SPI
	CC2500_CS_LOW();
	
	// send desired destination address of write
	SendByte(address);
	
	// send each byte in the buffer to registers through SPI
	while (bytesToWrite > 0x00){
		printf("Value to write: %i\n", *pBuffer);
		SendByte (*pBuffer);
		pBuffer ++;
		bytesToWrite--;
	}
		
	// end SPI
	CC2500_CS_HIGH();
}

void Transmit(uint8_t *buffer) {
	
}

void ReadRecvBuffer(uint8_t *buffer) {
	CC2500_CS_LOW();
	
	CC2500_Strobe(SRX);
	
	while ((GPIO_ReadInputDataBit(CC2500_SPI_GPIO_PORT, CC2500_SPI_MISO_PIN) & 0x80) == 0x80);
	
	uint8_t bytes_avail = CC2500_Strobe(SNOP) & 0x0F;
	
	if (bytes_avail > 0) {
		if (bytes_avail > 1) {
			SPI_Read(buffer, 0xBF, bytes_avail);
			printf("received: %i\n", buffer[0]);
		} else {
			SPI_Read(buffer, 0xFF, bytes_avail);
			printf("received: %i\n", buffer[0]);
		}
	}
	
	CC2500_Strobe(SIDLE);
	
	while ((GPIO_ReadInputDataBit(CC2500_SPI_GPIO_PORT, CC2500_SPI_MISO_PIN) & 0x80) == 0x80);
	
	CC2500_Strobe(SFRX);
		
	CC2500_CS_HIGH();
}

void wireless_Init(void) {
	uint8_t crtl1 = 0x00;
	LowLevel_Init();

	// Settings
	uint8_t w_buffer[47];
	
	w_buffer[0] = VAL_CC2500_IOCFG2;
	w_buffer[1] = 0x00; // doesn't matter for this application
	w_buffer[2] = VAL_CC2500_IOCFG0;
	w_buffer[3] = VAL_CC2500_FIFOTHR;
	w_buffer[4] = 0x00;
	
	w_buffer[5] = 0x00;
	w_buffer[6] = VAL_CC2500_PKTLEN;
	w_buffer[7] = VAL_CC2500_PKTCTRL1;
	w_buffer[8] = VAL_CC2500_PKTCTRL0;
	w_buffer[9] = VAL_CC2500_ADDR;
	
	w_buffer[10] = VAL_CC2500_CHANNR;
	w_buffer[11] = VAL_CC2500_FSCTRL1;
	w_buffer[12] = VAL_CC2500_FSCTRL0;
	w_buffer[13] = VAL_CC2500_FREQ2;
	w_buffer[14] = VAL_CC2500_FREQ1;
	
	w_buffer[15] = VAL_CC2500_FREQ0;
	w_buffer[16] = VAL_CC2500_MDMCFG4;
	w_buffer[17] = VAL_CC2500_MDMCFG3;
	w_buffer[18] = VAL_CC2500_MDMCFG2;
	w_buffer[19] = VAL_CC2500_MDMCFG1;
	
	w_buffer[20] = VAL_CC2500_MDMCFG0;
	w_buffer[21] = VAL_CC2500_DEVIATN;
	w_buffer[22] = 0x00;
	w_buffer[23] = VAL_CC2500_MCSM1;
	w_buffer[24] = VAL_CC2500_MCSM0;
	
	w_buffer[25] = VAL_CC2500_FOCCFG;
	w_buffer[26] = VAL_CC2500_BSCFG;
	w_buffer[27] = VAL_CC2500_AGCTRL2;
	w_buffer[28] = VAL_CC2500_AGCTRL1;
	w_buffer[29] = VAL_CC2500_AGCTRL0;
	
	w_buffer[30] = 0x00;
	w_buffer[31] = 0x00;
	w_buffer[32] = 0x00;
	w_buffer[33] = VAL_CC2500_FREND1;
	w_buffer[34] = VAL_CC2500_FREND0;
	
	w_buffer[35] = VAL_CC2500_FSCAL3;
	w_buffer[36] = VAL_CC2500_FSCAL2;
	w_buffer[37] = VAL_CC2500_FSCAL1;
	w_buffer[38] = VAL_CC2500_FSCAL0;
	w_buffer[39] = 0x00;
	
	w_buffer[40] = 0x00;
	w_buffer[41] = VAL_CC2500_FSTEST;
	w_buffer[42] = 0x00;
	w_buffer[43] = 0x00;
	w_buffer[44] = VAL_CC2500_TEST2;
	
	w_buffer[45] = VAL_CC2500_TEST1;
	w_buffer[46] = VAL_CC2500_TEST0;
	
	SPI_Write(w_buffer, 0x00, 47);

}


