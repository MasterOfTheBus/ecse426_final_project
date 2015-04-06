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
#include "UI.h"
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


int direction;
int mode;
int shape;
int send;


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

osThreadDef(path_thread, osPriorityNormal, 1, 0);
osThreadDef(set_xy_thread, osPriorityNormal, 1, 0);
osThreadDef(angle_thread, osPriorityNormal, 1, 0);

// ID for thread
osThreadId path_thread_id;
osThreadId set_xy_thread_id;
osThreadId angle_thread_id;


/*
 * main: initialize and start the system
 */
int main (void) {
	drawRectangle(0.0,7.0);

  osKernelInitialize ();                    // initialize CMSIS-RTOS
	
  // initialize peripherals here
	motors_init();
	MEMS_config();
	MEMS_interrupt_config();
	
//	while(1){
//		u8 ReadValue;
//		ReadValue = GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_6);
//		printf("%d\n", ReadValue);
//	}
	
	// angle from 0 to 180
	//motor_0_angle = 90;
	//motor_1_angle = 120;
	//motor_2_angle = 90;
	//set_angles();
	
  // create 'thread' functions that start executing,
  // example: tid_name = osThreadCreate (osThread(name), NULL);
	path_thread_id = osThreadCreate(osThread(path_thread), NULL);
	
	//angle_thread_id = osThreadCreate(osThread(angle_thread), NULL);
	
	
	osKernelStart ();                         // start thread execution 
	
}


void EXTI0_IRQHandler(void){
	if(EXTI_GetITStatus(EXTI_Line0) != RESET){
		// clear the flag in the sensor's end
		EXTI_ClearITPendingBit(EXTI_Line0);
		osSignalSet(angle_thread_id, 0x01);
	}
}


