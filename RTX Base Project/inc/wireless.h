#include "stm32f4xx.h"
#include "stm32f4xx_conf.h"
#include <stdio.h>

/* Generalize the file for transmitter and receiver on different boards */
#define TRANSMITTER 0x00

#define CC2500_SPI							SPI1
#define CC2500_SPI_CLK					RCC_APB2Periph_SPI1

#if TRANSMITTER
/* Define the pins and ports for the configurations */
#define CC2500_SPI_GPIO_CLK 		RCC_AHB1Periph_GPIOA
#define CC2500_SPI_GPIO_PORT		GPIOA
#define CC2500_SPI_GPIO_AF			GPIO_AF_SPI1

#define CC2500_SPI_NSS_PIN    	GPIO_Pin_4
#define CC2500_SPI_SCK_PIN    	GPIO_Pin_5
#define CC2500_SPI_MISO_PIN    	GPIO_Pin_6
#define CC2500_SPI_MOSI_PIN    	GPIO_Pin_7

#else // they are the same ports nad pins, so the else can be deleted

#define CC2500_SPI_GPIO_CLK 		RCC_AHB1Periph_GPIOA
#define CC2500_SPI_GPIO_PORT		GPIOA
#define CC2500_SPI_GPIO_AF			GPIO_AF_SPI1

#define CC2500_SPI_NSS_PIN    	GPIO_Pin_4
#define CC2500_SPI_SCK_PIN    	GPIO_Pin_5
#define CC2500_SPI_MISO_PIN    	GPIO_Pin_6
#define CC2500_SPI_MOSI_PIN    	GPIO_Pin_7

#endif

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

#define CC2500_CS_LOW()       GPIO_ResetBits(CC2500_SPI_GPIO_PORT, CC2500_SPI_NSS_PIN)
#define CC2500_CS_HIGH()      GPIO_SetBits(CC2500_SPI_GPIO_PORT, CC2500_SPI_NSS_PIN)

static uint8_t SendByte(uint8_t byte);
void wireless_Init();
void SPI_Read(uint8_t* pBuffer, uint8_t address, uint16_t bytesToRead);
void SPI_Write(uint8_t* pBuffer, uint8_t address, uint16_t bytesToWrite);
