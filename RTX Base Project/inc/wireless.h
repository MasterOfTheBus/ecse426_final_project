#include "stm32f4xx.h"
#include "stm32f4xx_conf.h"

#define FLAG_TIMEOUT ((uint32_t)0x1000)

typedef struct{
	uint8_t Power_Mode;
	uint8_t Oscillator_Power;
	uint8_t Receive_Transmit_Mode;
	uint8_t RF_Channel;
	uint8_t Data_Rate;
	uint8_t Modulation_Format;
	uint8_t RX_Channel_Filter_Bandwidth;
	uint8_t RF_Output_Power;
	uint8_t Data_Buffering;
} wireless_InitTypeDef;

static uint8_t SendByte(uint8_t byte);
static void LowLevel_Init(void);