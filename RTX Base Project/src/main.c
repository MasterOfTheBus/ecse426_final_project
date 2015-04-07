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

void ReceiveData(void const *argument){
	uint8_t r_buffer;
	while (1){
		CC2500_ReadRecvBuffer(&r_buffer);
		osDelay(100);
	}
}

osThreadDef(ReceiveData, osPriorityNormal, 1, 0);

osThreadDef(motor_0_thread, osPriorityNormal, 1, 0);
osThreadDef(motor_2_thread, osPriorityNormal, 1, 0);
osThreadDef(motor_1_thread, osPriorityNormal, 1, 0);

osThreadId motor_0_thread_id;
osThreadId motor_1_thread_id;
osThreadId motor_2_thread_id;


// To test wireless module and SPI
void wireless_testbench (){
	// initialize wireless SPI
	CC2500_Init();
	uint8_t r_buffer[1];
#if 0
	printf("\n\n");

	uint8_t settings_buffer[1];
	uint8_t address = 0x00;
	while (address < 47) {
		SPI_Read(settings_buffer, address, 1);
		printf("settings 0x%02x: 0x%02x\n", address, settings_buffer[0]);
		address++;
		if (address == 1 || address == 22) {
			address++;
		} else if (address == 4 || address == 39 || address == 42) {
			address+=2;
		} else if (address == 30) {
			address+=3;
		}
	}

	settings_buffer[0] = 0x06;
	SPI_Write(settings_buffer, 0x02, 1);
	
	SPI_Read(settings_buffer, 0x02, 1);
	printf("read 0x%02x\n", settings_buffer[0]);
	
	printf("\n\n");
	
	// ------------------ SPI testing -----------------------
	
	uint8_t buffer[2];
	address = 0x31;
	uint16_t bytes = 0x02;
	
	// --- Strobe ----
	uint8_t status;
	status = CC2500_Strobe(SNOP);
	printf("status: 0x%02x\n\n", status);
	
	// ---- Read -----
	buffer[0] = 1;
	printf("buffer before: %i\n", buffer[0]);
	SPI_Read (buffer, address, bytes);

	printf ("Value after: %i \n\n", buffer[0]);

	// --- Write ---
	uint8_t w_buffer[] = {8};
	printf("value writing: %i\n", w_buffer[0]);

	address = 0x06;
	bytes = 0x01;
	
	SPI_Write (w_buffer, address, bytes);
	SPI_Read (r_buffer, address, bytes);
	
	printf ("Value: %i \n", r_buffer[0]);
	
	printf("\n\n\n");
	
	// -- Strobe again ---
	status = CC2500_Strobe(SNOP);
	printf("status: 0x%02x\n", status);
	
	delay(100);
	
	status = CC2500_Strobe(SRX);
	printf("status: 0x%02x\n", status);
	
	delay(100);
	
	while (status != 0x01) {
		status = CC2500_Strobe(SNOP);
		printf("status: 0x%02x\n", status);
	
		delay (100);
	}
	
	status = CC2500_Strobe(SIDLE);
	printf("status: 0x%02x\n", status);
	
	delay(100);
	
	status = CC2500_Strobe(SNOP);
	printf("status: 0x%02x\n", status);
#endif
	// ---------------- Receive testing --------------------
#if 1
	while (1){
		CC2500_ReadRecvBuffer(r_buffer);
		delay(100);
	}
#endif
}

/*
 * main: initialize and start the system
 */
int main (void) {
	
	wireless_testbench ();
	
  osKernelInitialize ();                    // initialize CMSIS-RTOS
	
	// ID for thread
	//osThreadId	RecvData_thread;
	
  // initialize peripherals here
	//wireless_Init();
//	motors_init();
	
	// angle from 0 to 180
//	motor_0_angle = 45;
	//motor_1_angle = 90;
	//motor_2_angle = 45;
	
  // create 'thread' functions that start executing,
  // example: tid_name = osThreadCreate (osThread(name), NULL);
	//RecvData_thread = osThreadCreate(osThread(ReceiveData), NULL);
//	motor_0_thread_id = osThreadCreate(osThread(motor_0_thread), NULL);
	//motor_1_thread_id = osThreadCreate(osThread(motor_1_thread), NULL);
	//motor_2_thread_id = osThreadCreate(osThread(motor_2_thread), NULL);
	
	
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




