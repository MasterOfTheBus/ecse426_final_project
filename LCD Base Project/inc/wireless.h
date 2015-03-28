#include "stm32f4xx.h"
#include "stm32f4xx_conf.h"

typedef struct
{
	uint8_t Power_Mode;
	uint8_t Oscillator_Power;
	uint8_t Receive_Transmit_Mode;
	uint8_t RF_Channel;
	uint8_t Data_Rate;
	uint8_t Modulation_Format;
	uint8_t RX_Channel_Filter_Bandwidth;
	uint8_t RF_Output_Power;
	uint8_t Data_Buffering;
}CC2500_InitTypeDef;

void CC2500_Init(CC2500_InitTypeDef *CC2500_InitStruct);
//void CC2500_InterruptConfig(CC2500_InterruptConfigTypeDef *CC2500_InterruptConfigStruct);
//void CC2500_ReadMSG(int32_t* out);
//void CC2500_Write(uint8_t* pBuffer, uint8_t WriteAddr, uint16_t NumByteToWrite);
//void CC2500_Read(uint8_t* pBuffer, uint8_t ReadAddr, uint16_t NumByteToRead);