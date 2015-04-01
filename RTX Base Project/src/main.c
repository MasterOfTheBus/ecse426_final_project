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

int motor_0_angle;
int motor_1_angle;
int motor_2_angle;

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
		GPIO_ToggleBits(GPIOD, GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15);
		//printf("hello world\n");
		osDelay(250);
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
	
	uint8_t buffer[2];
	uint8_t address = 0x31;
	uint16_t bytes = 0x02;
	
	buffer[0] = 1;
	printf("buffer before: %i\n", buffer[0]);
	SPI_Read (buffer, address, bytes);

	printf ("Value after: %i \n", buffer[0]);

	uint8_t w_buffer[] = {8};
	printf("value writing: %i\n", w_buffer[0]);
	uint8_t r_buffer[1];
	address = 0x06;
	bytes = 0x01;
	
	SPI_Write (w_buffer, address, bytes);
	SPI_Read (r_buffer, address, bytes);
	
	printf ("Value: %i \n", r_buffer[0]);
}

/*
 * main: initialize and start the system
 */
int main (void) {
	
	wireless_testbench ();
//	
  osKernelInitialize ();                    // initialize CMSIS-RTOS
//	
//	// ID for thread
//	osThreadId	Blinky_thread;
//	
//  // initialize peripherals here
//	Blinky_GPIO_Init();
//	motors_init();
//	
//	// angle from 0 to 180
//	motor_0_angle = 45;
//	//motor_1_angle = 90;
//	//motor_2_angle = 45;
//	
//  // create 'thread' functions that start executing,
//  // example: tid_name = osThreadCreate (osThread(name), NULL);
//	Blinky_thread = osThreadCreate(osThread(Blinky), NULL);
//	motor_0_thread_id = osThreadCreate(osThread(motor_0_thread), NULL);
//	//motor_1_thread_id = osThreadCreate(osThread(motor_1_thread), NULL);
//	//motor_2_thread_id = osThreadCreate(osThread(motor_2_thread), NULL);
//	
//	
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




