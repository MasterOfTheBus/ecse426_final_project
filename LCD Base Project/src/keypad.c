#include "keypad.h"

int debounce_timer;
const int debounce_time = 20;
int column;
int row;
char key;

// keymap
char keys[4][3] = {  
 {'1', '2', '3'},
 {'4', '5', '6'},
 {'7', '8', '9'},
 {'*', '0', '#'}
};


int get_column()
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOD, ENABLE);
	GPIO_InitTypeDef GPIO_InitStruct;
	
	// Set the rows as outputs
	GPIO_InitStruct.GPIO_Pin = ROW_0 | ROW_1 | ROW_2 | ROW_3;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_DOWN; 
	GPIO_Init(GPIOD, &GPIO_InitStruct);
	
	// Set the columns as inputs
	GPIO_InitStruct.GPIO_Pin = COL_0 | COL_1 | COL_2;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP; 
	GPIO_Init(GPIOB, &GPIO_InitStruct);
	
	if(GPIO_ReadInputDataBit(GPIOB, COL_0) == 0)
		return 0;
	else if(GPIO_ReadInputDataBit(GPIOB, COL_1) == 0)
		return 1;
	else if(GPIO_ReadInputDataBit(GPIOB, COL_2) == 0)
		return 2;
	else
		return -1;
}

int get_row()
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOD, ENABLE);
	GPIO_InitTypeDef GPIO_InitStruct;
	
	// Set the columns as outputs
	GPIO_InitStruct.GPIO_Pin = COL_0 | COL_1 | COL_2;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_DOWN; 
	GPIO_Init(GPIOB, &GPIO_InitStruct);
	
	// Set the row as inputs
	GPIO_InitStruct.GPIO_Pin = ROW_0 | ROW_1 | ROW_2 | ROW_3;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP; 
	GPIO_Init(GPIOD, &GPIO_InitStruct);
	
	if(GPIO_ReadInputDataBit(GPIOD, ROW_0) == 0)
		return 0;
	else if(GPIO_ReadInputDataBit(GPIOD, ROW_1) == 0)
		return 1;
	else if(GPIO_ReadInputDataBit(GPIOD, ROW_2) == 0)
		return 2;
	else if(GPIO_ReadInputDataBit(GPIOD, ROW_3) == 0)
		return 3;
	else
		return -1;
}

void key_fsm()
{
	//printf("%d\n", keypad_state);
	switch(keypad_state){
		case IDLE:
			if(column != -1 && row != -1){
				debounce_timer = TIM_GetCounter(TIM3);
				keypad_state = DEBOUNCING;
			}
			break;
		case DEBOUNCING:
			if((debounce_timer + debounce_time) % 200 == TIM_GetCounter(TIM3)){
				key = keys[row][column];
				keypad_state = PRESSED;
			}
			break;
		case PRESSED:
			if(column == -1 || row == -1){
				debounce_timer = TIM_GetCounter(TIM3);
				keypad_state = RELEASED;
			}
			break;
		case RELEASED:
			if((debounce_timer + debounce_time) % 200 == TIM_GetCounter(TIM3)){
				keypad_state = IDLE;
			}
			break;
	
	}
}

char get_key()
{
	column = get_column();
	row = get_row();
	key = NO_KEY;
	
	key_fsm();
	
	return key;
}


