#include "stm32f4xx.h"
#include "stm32f4xx_conf.h"

#define FLAG_TIMEOUT ((uint32_t)0x1000)

static uint8_t SendByte(uint8_t byte);
void wireless_Init();
void wireless_Read(uint8_t* pBuffer, uint8_t ReadAddr, uint16_t NumByteToRead);
void wireless_Write(uint8_t* pBuffer, uint8_t WriteAddr, uint16_t NumByteToWrite);