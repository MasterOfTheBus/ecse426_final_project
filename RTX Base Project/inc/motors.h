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

// Constants
#define arm_a (double) 6.5
#define arm_b (double) 7
#define motor_position (double) 1.9
#define segm_length 1
#define step_size 0.1
	
#define PI 3.14159265359


// Frame
#define x_max 7
#define x_min -7
#define y_max 12
#define y_min 6



extern double motor_0_angle;
extern double motor_1_angle;
extern double motor_2_angle;

void motors_init();
void enable_motor(int motor);
void disable_motor(int motor);
void set_angle(int angle, int motor);
void motor_0_thread(void const *argument);
void motor_1_thread(void const *argument);
void motor_2_thread(void const *argument);

void goTo(double x, double y);
//void draw_LroR(double x, double y);
void setPath(double x, double y, int mode, int direction);
#endif



