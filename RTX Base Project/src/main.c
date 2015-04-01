/*----------------------------------------------------------------------------
 * CMSIS-RTOS 'main' function template
 *---------------------------------------------------------------------------*/

#define osObjectsPublic                     // define objects in main module
#include "osObjects.h"                      // RTOS object definitions
#include "stm32f4xx.h"                  // Device header
#include "stm32f4xx_conf.h"
#include "motors.h"
#include "wireless.h"
#include <stdio.h>

double motor_0_angle;
double motor_1_angle;
double motor_2_angle;

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
		for(double x = 6; x < 11; x = x + 0.1){
			goTo(0, x);
			osDelay(200);
			if(x == 10.9) x = 6;
		}
	}
}

osThreadDef(Blinky, osPriorityNormal, 1, 0);

osThreadDef(motor_0_thread, osPriorityNormal, 1, 0);
osThreadDef(motor_2_thread, osPriorityNormal, 1, 0);
osThreadDef(motor_1_thread, osPriorityNormal, 1, 0);

osThreadId motor_0_thread_id;
osThreadId motor_1_thread_id;
osThreadId motor_2_thread_id;


// To test wireless module and SPI
void wireless_testbench (){
	// initialize wireless SPI
	wireless_Init();
	
	uint8_t buffer[] = {8};
	uint8_t address = 0x36;
	uint16_t bytes = 0x02;
	
	//SPI_Write (buffer, address, bytes);
	*buffer = 1;
	SPI_Read (buffer, address, bytes);
	
	printf ("Value: %i \n", *buffer);
}

/*
 * main: initialize and start the system
 */
int main (void) {
	
	//wireless_testbench ();
//	
 // osKernelInitialize ();                    // initialize CMSIS-RTOS
	
	// ID for thread
	osThreadId	Blinky_thread;
	
  // initialize peripherals here
	Blinky_GPIO_Init();
	motors_init();
	
	// angle from 0 to 180
	//motor_0_angle = 45;
	//motor_1_angle = 90;
	//motor_2_angle = 45;
	
  // create 'thread' functions that start executing,
  // example: tid_name = osThreadCreate (osThread(name), NULL);
	Blinky_thread = osThreadCreate(osThread(Blinky), NULL);
	motor_0_thread_id = osThreadCreate(osThread(motor_0_thread), NULL);
	motor_1_thread_id = osThreadCreate(osThread(motor_1_thread), NULL);
	//motor_2_thread_id = osThreadCreate(osThread(motor_2_thread), NULL);
	
	
	//osKernelStart ();                         // start thread execution 
	
//	while(1){
//		goTo(0, 8);
//		goTo(-3, 8);
//		goTo(3, 8);
//		goTo(-1.6, 8);
//		goTo(1.6, 8);
//		goTo(1.9,8);
//		goTo(-1.9,8);
//	}
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




