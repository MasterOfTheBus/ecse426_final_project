/*----------------------------------------------------------------------------
 * CMSIS-RTOS 'main' function template
 *---------------------------------------------------------------------------*/

#define osObjectsPublic                     // define objects in main module
#include "osObjects.h"                      // RTOS object definitions
#include "stm32f4xx.h"                  // Device header
#include "stm32f4xx_conf.h"
#include "motors.h"
#include "wireless.h"
#include "packet.h"
#include <stdio.h>

#define TESTING 0 // 0 to not test
#define RECVSIG 0x01

#if TESTING
#include "tests.h"
#endif

int motor_0_angle;
int motor_1_angle;
int motor_2_angle;

void ReceiveData(void const *argument){
	uint8_t r_buffer;
	uint8_t prev;
	uint8_t not_idle = 1;
	while (1){
		if (not_idle) {
			prev = r_buffer;
			CC2500_Change_State (SRX);
			osSignalWait(RECVSIG, osWaitForever);
		}
		uint8_t status = status_state(CC2500_Strobe(SNOP));
		if (status != IDLE_STATE) {
			not_idle = 0;
			osDelay(100);
		} else {
			not_idle = 1;
		}
		if (not_idle) {
			not_idle = 1;
			CC2500_ReadRecvBuffer(&r_buffer);
			if (r_buffer != prev && status != RX_STATE) {
				// processing for non-repeated data
				printf("read: 0x%02x\n", r_buffer);
#if 0
				// send a confirmation packet to the sender
				uint8_t pkt = CONFIRMATION_PKT;
				CC2500_Transmit(&pkt, 1);
#endif
			}
		}
	}
}

osThreadDef(ReceiveData, osPriorityNormal, 1, 0);

//osThreadDef(motor_0_thread, osPriorityNormal, 1, 0);
//osThreadDef(motor_2_thread, osPriorityNormal, 1, 0);
//osThreadDef(motor_1_thread, osPriorityNormal, 1, 0);

// ID for thread
osThreadId RecvData_thread;
//osThreadId motor_0_thread_id;
//osThreadId motor_1_thread_id;
//osThreadId motor_2_thread_id;


/*
 * main: initialize and start the system
 */
int main (void) {
#if TESTING
	wireless_testbench ();
#else
	
  osKernelInitialize ();                    // initialize CMSIS-RTOS
	
  // initialize peripherals here
	CC2500_Init();
//	motors_init();
	
	// angle from 0 to 180
//	motor_0_angle = 45;
	//motor_1_angle = 90;
	//motor_2_angle = 45;
	
  // create 'thread' functions that start executing,
  // example: tid_name = osThreadCreate (osThread(name), NULL);
	RecvData_thread = osThreadCreate(osThread(ReceiveData), NULL);
//	motor_0_thread_id = osThreadCreate(osThread(motor_0_thread), NULL);
	//motor_1_thread_id = osThreadCreate(osThread(motor_1_thread), NULL);
	//motor_2_thread_id = osThreadCreate(osThread(motor_2_thread), NULL);
	
	
	osKernelStart ();                         // start thread execution 
#endif
}

void TIM3_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
	{
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
//		osSignalSet(motor_0_thread_id, 0x01);
//		osSignalSet(motor_1_thread_id, 0x01);
//		osSignalSet(motor_2_thread_id, 0x01);
	}
}

void EXTI15_10_IRQHandler(void) {
	if (EXTI_GetITStatus(EXTI_Line14) != RESET) {
		osSignalSet (RecvData_thread, RECVSIG);
		EXTI_ClearITPendingBit(EXTI_Line14);
	}
}


