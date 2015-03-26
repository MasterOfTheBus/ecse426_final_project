#ifndef motors_h
#define motors_h

#include "stm32f4xx_gpio.h"
#include "osObjects.h"

// GPIO MOTOR CLOCKS
#define MOTOR_CLOCK RCC_AHB1Periph_GPIOB

// GPIO MOTOR PORT
#define MOTOR_PORT GPIOB

// GPIO MOTOR PINS
#define MOTOR_0 GPIO_Pin_11
#define MOTOR_1 GPIO_Pin_12
#define MOTOR_2 GPIO_Pin_13

extern int motor_0_angle;
extern int motor_1_angle;
extern int motor_2_angle;

void motors_init();
void enable_motor(int motor);
void disable_motor(int motor);
void set_angle(int angle, int motor);
void motor_0_thread(void const *argument);
void motor_1_thread(void const *argument);
void motor_2_thread(void const *argument);

#endif



