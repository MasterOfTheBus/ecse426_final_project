#include "timer.h"

void timer_init()
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

	//	Desired rate can be found by using the equation: 42Mhz/(Prescaler * period)
	TIM_TimeBaseInitTypeDef Timer_InitStruct; 
	Timer_InitStruct.TIM_Prescaler = 1000;
	Timer_InitStruct.TIM_CounterMode = TIM_CounterMode_Up;
	Timer_InitStruct.TIM_Period = 200;
	Timer_InitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
	Timer_InitStruct.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM3, &Timer_InitStruct);
	TIM_Cmd(TIM3, ENABLE);
	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
	
	// NVIC interrupt settings
	NVIC_InitTypeDef NVIC_InitStruct;
	NVIC_InitStruct.NVIC_IRQChannel = TIM3_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStruct);
	
}



