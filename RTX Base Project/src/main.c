/*----------------------------------------------------------------------------
 * CMSIS-RTOS 'main' function template
 *---------------------------------------------------------------------------*/

#define osObjectsPublic                     // define objects in main module
#include "osObjects.h"                      // RTOS object definitions
#include "stm32f4xx.h"                  // Device header
#include "stm32f4xx_conf.h"
#include "motors.h"
#include "wireless.h"
#include "MEMS.h"
#include <stdio.h>

double pitch;
double roll;

double motor_0_angle;
double motor_1_angle;
double motor_2_angle;
double current_x;
double current_y;
double x_path[array_length] = {0};
double y_path[array_length] = {0};

void Blinky_GPIO_Init(void){
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13| GPIO_Pin_14| GPIO_Pin_15;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOD, &GPIO_InitStructure);
	
}

void set_xy_thread(void const *argument){
	while(1){
		printf("roll: %f\n", roll);
		printf("pitch: %f\n", pitch);
		printf("x: %f\n", current_x);
		printf("y: %f\n", current_y);
		
		double new_x = current_x;
		double new_y = current_y;
		
		if(pitch < - 30) new_x = current_x + 0.2;
		else if(pitch > 30) new_x = current_x - 0.2;
		
		if(roll < - 30) new_y = current_y + 0.2;
		else if(roll > 30) new_y = current_y - 0.2;
		
		goTo (new_x,new_y);
		
		osDelay(50);
	}
}

void Blinky(void const *argument){
	while(1){
		//GPIO_ToggleBits(GPIOD, GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15);
		//printf("hello world\n");
//		goTo(-3,7);
//		osDelay(2000);
//		goTo(3,7);
//		osDelay(2000);
//		goTo(0,8);
//		osDelay(2000);
//		goTo(0,11);
//		osDelay(2000);
//		goTo(-5,6);
//		osDelay(2000);
//		goTo(5,6);
//		osDelay(2000);
		
//		for(double x = 6; x < 11; x = x + 0.1){
//			goTo(0, x);
//			osDelay(200);
//			if(x == 10.9) x = 6;
//		}
		
		
		for(int i=0; i < 50; i++){
			goTo(x_path[i], y_path[i]);
			//printf("position : {%f,%f}\n", x_path[i], y_path[i]);
			osDelay(50);
	
		}
	}
}

osThreadDef(Blinky, osPriorityNormal, 1, 0);
osThreadDef(set_xy_thread, osPriorityNormal, 1, 0);
osThreadDef(motor_0_thread, osPriorityNormal, 1, 0);
osThreadDef(motor_2_thread, osPriorityNormal, 1, 0);
osThreadDef(motor_1_thread, osPriorityNormal, 1, 0);
osThreadDef(angle_thread, osPriorityNormal, 1, 0);

// ID for thread
osThreadId Blinky_thread;
osThreadId set_xy_thread_id;
osThreadId motor_0_thread_id;
osThreadId motor_1_thread_id;
osThreadId motor_2_thread_id;
osThreadId angle_thread_id;


/*
 * main: initialize and start the system
 */
int main (void) {
	drawTriangle(0.0,7.0);

  osKernelInitialize ();                    // initialize CMSIS-RTOS
	
  // initialize peripherals here
	Blinky_GPIO_Init();
	motors_init();
	MEMS_config();
	MEMS_interrupt_config();
	
	// angle from 0 to 180
	//motor_0_angle = 45;
	//motor_1_angle = 90;
	//motor_2_angle = 90;
	
  // create 'thread' functions that start executing,
  // example: tid_name = osThreadCreate (osThread(name), NULL);
	//Blinky_thread = osThreadCreate(osThread(Blinky), NULL);
	set_xy_thread_id = osThreadCreate(osThread(set_xy_thread), NULL);
	motor_0_thread_id = osThreadCreate(osThread(motor_0_thread), NULL);
	motor_1_thread_id = osThreadCreate(osThread(motor_1_thread), NULL);
	motor_2_thread_id = osThreadCreate(osThread(motor_2_thread), NULL);
	
	angle_thread_id = osThreadCreate(osThread(angle_thread), NULL);
	
	
	osKernelStart ();                         // start thread execution 
	
}

void TIM3_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
	{
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
		osSignalSet(motor_0_thread_id, 0x01);
		osSignalSet(motor_1_thread_id, 0x01);
		osSignalSet(motor_2_thread_id, 0x01);
	}
}


void EXTI0_IRQHandler(void){
	if(EXTI_GetITStatus(EXTI_Line0) != RESET){
		// clear the flag in the sensor's end
		EXTI_ClearITPendingBit(EXTI_Line0);
		osSignalSet(angle_thread_id, 0x01);
	}
}


