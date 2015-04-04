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
	
	printf("\n\n");
	
	uint8_t settings_buffer[1];
	uint8_t address = 0x00;
//	while (address < 47) {
//		SPI_Read(settings_buffer, address, 1);
//		printf("settings 0x%02x: 0x%02x\n", address, settings_buffer[0]);
//		address++;
//		if (address == 1 || address == 22) {
//			address++;
//		} else if (address == 4 || address == 39 || address == 42) {
//			address+=2;
//		} else if (address == 30) {
//			address+=3;
//		}
//	}

	SPI_Read(settings_buffer, 0x00, 1);
	printf("0x00: 0x%02x\n", settings_buffer[0]);
	SPI_Read(settings_buffer, 0x02, 1);
	printf("0x02: 0x%02x\n", settings_buffer[0]);
	SPI_Read(settings_buffer, 0x03, 1);
printf("0x03: 0x%02x\n", settings_buffer[0]);
	SPI_Read(settings_buffer, 0x06, 1);
printf("0x06: 0x%02x\n", settings_buffer[0]);
	SPI_Read(settings_buffer, 0x07, 1);
printf("0x07: 0x%02x\n", settings_buffer[0]);
	SPI_Read(settings_buffer, 0x08, 1);
printf("0x08: 0x%02x\n", settings_buffer[0]);
	SPI_Read(settings_buffer, 0x09, 1);
printf("0x09: 0x%02x\n", settings_buffer[0]);
	SPI_Read(settings_buffer, 0x0A, 1);
printf("0x0A: 0x%02x\n", settings_buffer[0]);
	SPI_Read(settings_buffer, 0x0B, 1);
printf("0x0B: 0x%02x\n", settings_buffer[0]);
	SPI_Read(settings_buffer, 0x0C, 1);
printf("0x0C: 0x%02x\n", settings_buffer[0]);
	SPI_Read(settings_buffer, 0x0D, 1);
printf("0x0D: 0x%02x\n", settings_buffer[0]);
	SPI_Read(settings_buffer, 0x0E, 1);
	printf("0x0E: 0x%02x\n", settings_buffer[0]);
	SPI_Read(settings_buffer, 0x0F, 1);
printf("0x0F: 0x%02x\n", settings_buffer[0]);
	SPI_Read(settings_buffer, 0x10, 1);
printf("16: 0x%02x\n", settings_buffer[0]);
	SPI_Read(settings_buffer, 0x11, 1);
printf("17: 0x%02x\n", settings_buffer[0]);
	SPI_Read(settings_buffer, 0x12, 1);
printf("18: 0x%02x\n", settings_buffer[0]);
	SPI_Read(settings_buffer, 0x13, 1);
	printf("19: 0x%02x\n", settings_buffer[0]);
	SPI_Read(settings_buffer, 0x14, 1);
printf("20: 0x%02x\n", settings_buffer[0]);
	SPI_Read(settings_buffer, 0x15, 1);
printf("21: 0x%02x\n", settings_buffer[0]);
	SPI_Read(settings_buffer, 0x17, 1);
printf("23: 0x%02x\n", settings_buffer[0]);
	SPI_Read(settings_buffer, 0x18, 1);
printf("24: 0x%02x\n", settings_buffer[0]);
	SPI_Read(settings_buffer, 0x19, 1);
printf("25: 0x%02x\n", settings_buffer[0]);
	SPI_Read(settings_buffer, 0x1A, 1);
printf("26: 0x%02x\n", settings_buffer[0]);
	SPI_Read(settings_buffer, 0x1B, 1);
printf("27: 0x%02x\n", settings_buffer[0]);
	SPI_Read(settings_buffer, 0x1C, 1);
printf("28: 0x%02x\n", settings_buffer[0]);
	SPI_Read(settings_buffer, 0x1D, 1);
printf("29: 0x%02x\n", settings_buffer[0]);
	SPI_Read(settings_buffer, 0x21, 1);
printf("33: 0x%02x\n", settings_buffer[0]);
	SPI_Read(settings_buffer, 0x22, 1);
	printf("34: 0x%02x\n", settings_buffer[0]);
	SPI_Read(settings_buffer, 0x23, 1);
printf("35: 0x%02x\n", settings_buffer[0]);
	SPI_Read(settings_buffer, 0x24, 1);
printf("36: 0x%02x\n", settings_buffer[0]);
	SPI_Read(settings_buffer, 0x25, 1);
printf("37: 0x%02x\n", settings_buffer[0]);
	SPI_Read(settings_buffer, 0x26, 1);
printf("38: 0x%02x\n", settings_buffer[0]);
	SPI_Read(settings_buffer, 0x29, 1);
printf("41: 0x%02x\n", settings_buffer[0]);
	SPI_Read(settings_buffer, 0x2C, 1);
printf("44: 0x%02x\n", settings_buffer[0]);
	SPI_Read(settings_buffer, 0x2D, 1);
printf("45: 0x%02x\n", settings_buffer[0]);
	SPI_Read(settings_buffer, 0x2E, 1);
	printf("46: 0x%02x\n", settings_buffer[0]);

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
	uint8_t r_buffer[1];
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
	
	// ---------------- Receive testing --------------------
	
//	while (1) {
//		ReadRecvBuffer(r_buffer);
//	}
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




