#include "motors.h"

void motors_init(){
	// Hardware clock init
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

	//	Desired rate can be found by using the equation: 42Mhz/(Prescaler * period)
	TIM_TimeBaseInitTypeDef Timer_InitStruct; 
	Timer_InitStruct.TIM_Prescaler = 420;
	Timer_InitStruct.TIM_CounterMode = TIM_CounterMode_Up;
	Timer_InitStruct.TIM_Period = 2000;
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
	
	// Motors init
	GPIO_InitTypeDef  GPIO_InitStruct;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	
	// configure pins to be used
	GPIO_InitStruct.GPIO_Pin = 	MOTOR_0 |
															MOTOR_1 |
															MOTOR_2 ;
	
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;			// configure pin mode for output
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;	// configure the clock speed to avoid limiting slew rate
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;			// configure the pin type to push-pull
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;		// configure pullup/pulldown resistors on pin (not input, no pull)
	
	GPIO_Init(MOTOR_PORT, &GPIO_InitStruct);

}

void enable_motor(int motor) 
{
	switch(motor){
		case 0:
			GPIO_SetBits(MOTOR_PORT, MOTOR_0);
			break;
		case 1:
			GPIO_SetBits(MOTOR_PORT, MOTOR_1);
			break;
		case 2:
			GPIO_SetBits(MOTOR_PORT, MOTOR_2);
			break;
	}
}

void disable_motor(int motor) 
{	
	switch(motor){
		case 0:
			GPIO_ResetBits(MOTOR_PORT, MOTOR_0);
			break;
		case 1:
			GPIO_ResetBits(MOTOR_PORT, MOTOR_1);
			break;
		case 2:
			GPIO_ResetBits(MOTOR_PORT, MOTOR_2);
			break;
	}

}

void set_angle(int angle, int motor){
	int timerValue = TIM_GetCounter(TIM3);
	int timerStart = TIM_GetCounter(TIM3);
	int duty_cycle = (angle*2)+150;
	while((timerStart + duty_cycle) % 2000 > TIM_GetCounter(TIM3)){
		enable_motor(motor);
	}
	disable_motor(motor);
}

void motor_0_thread(void const *argument){
	while(1){
		osSignalWait(0x01, osWaitForever);
		set_angle(motor_0_angle, 0);
	}
}

void motor_1_thread(void const *argument){
	while(1){
		osSignalWait(0x01, osWaitForever);
		set_angle(motor_1_angle, 1);
	}
}

void motor_2_thread(void const *argument){
	while(1){
		osSignalWait(0x01, osWaitForever);
		set_angle(motor_2_angle, 2);
	}
}



