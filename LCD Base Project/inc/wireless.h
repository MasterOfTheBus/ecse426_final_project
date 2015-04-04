#include "stm32f4xx.h"
#include "stm32f4xx_conf.h"
#include <stdio.h>

/* Generalize the file for transmitter and receiver on different boards */

#define CC2500_SPI							SPI2
#define CC2500_SPI_CLK					RCC_APB1Periph_SPI2

/* Define the pins and ports for the configurations */
#define CC2500_SPI_GPIO_CLK 					RCC_AHB1Periph_GPIOB
#define CC2500_SPI_CS_GPIO_CLK				RCC_AHB1Periph_GPIOB
#define CC2500_SPI_GPIO_PORT					GPIOB
#define CC2500_SPI_CS_GPIO_PORT				GPIOB
#define CC2500_SPI_GPIO_AF						GPIO_AF_SPI2

#define CC2500_SPI_NSS_PIN    				GPIO_Pin_12 // p17 
#define CC2500_SPI_SCK_PIN    				GPIO_Pin_13 // p16
#define CC2500_SPI_MISO_PIN    				GPIO_Pin_14 // p15
#define CC2500_SPI_MOSI_PIN    				GPIO_Pin_15 // p18
#define CC2500_SPI_NSS_SOURCE         GPIO_PinSource12
#define CC2500_SPI_SCK_SOURCE         GPIO_PinSource13
#define CC2500_SPI_MISO_SOURCE        GPIO_PinSource14
#define CC2500_SPI_MOSI_SOURCE        GPIO_PinSource15


#define SRES 0x30
#define SFSTXON 0x31
#define SXOFF 0x32
#define SCAL 0x33
#define SRX 0x34
#define STX 0x35
#define SIDLE 0x36
#define SWOR 0x38
#define SPWD 0x39
#define SFRX 0x3A
#define SFTX 0x3B
#define SWORRST 0x3C
#define SNOP 0x3D        

/* Define macros for general use */

#define FLAG_TIMEOUT ((uint32_t)0x1000)

#define CC2500_CS_LOW()       GPIO_ResetBits(CC2500_SPI_CS_GPIO_PORT, CC2500_SPI_NSS_PIN)
#define CC2500_CS_HIGH()      GPIO_SetBits(CC2500_SPI_CS_GPIO_PORT, CC2500_SPI_NSS_PIN)


/*	Buffer status registers	*/
#define CC2500REG_TXBYTES		(0x3A|0xFA)
#define CC2500REG_RXBYTES		(0x3B|0xFB)

/*	Burst access to FIFOs	*/
#define CC2500REG_TX_FIFO		0x3F
#define CC2500REG_RX_FIFO		0xBF

static uint8_t SendByte(uint8_t byte);
void wireless_Init(void);
uint8_t CC2500_Strobe(uint8_t Strobe);
void SPI_Read(uint8_t* pBuffer, uint8_t address, uint16_t bytesToRead);
void SPI_Write(uint8_t* pBuffer, uint8_t address, uint16_t bytesToWrite);
void ReadRecvBuffer(uint8_t *buffer);
void Transmit(uint8_t *buffer);
void delay(long num_ticks);
