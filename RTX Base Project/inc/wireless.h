#include "stm32f4xx.h"
#include "stm32f4xx_conf.h"

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

/* Define macros for general use */

#define FLAG_TIMEOUT ((uint32_t)0x1000)

#define CC2500_CS_LOW()       GPIO_ResetBits(CC2500_SPI_GPIO_PORT, CC2500_SPI_NSS_PIN)
#define CC2500_CS_HIGH()      GPIO_SetBits(CC2500_SPI_GPIO_PORT, CC2500_SPI_NSS_PIN)

static uint8_t SendByte(uint8_t byte);
void wireless_Init();
void SPI_Read(uint8_t* pBuffer, uint8_t address, uint16_t bytesToRead);
void SPI_Write(uint8_t* pBuffer, uint8_t address, uint16_t bytesToWrite);
