#ifndef keypad_h
#define keypad_h

#include "stm32f4xx_gpio.h"
#include "timer.h"

// GPIO PORT D
#define ROW_0 GPIO_Pin_8
#define ROW_1 GPIO_Pin_9
#define ROW_2 GPIO_Pin_10
#define ROW_3 GPIO_Pin_11

// GPIO PORT B
#define COL_0 GPIO_Pin_11
#define COL_1 GPIO_Pin_12
#define COL_2 GPIO_Pin_13

#define NO_KEY 0

typedef int STATE;
#define IDLE 0
#define DEBOUNCING 1
#define PRESSED 2
#define RELEASED 3

extern STATE keypad_state;

char get_key();

#endif


