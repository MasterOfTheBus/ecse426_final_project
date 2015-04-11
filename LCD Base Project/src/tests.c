#include "tests.h"
#include "stm32f4xx.h"                  // Device header

#include "stm32f4xx_conf.h"
#include "wireless.h"
#include "packet.h"
#include <stdio.h>

// To test wireless module and SPI
void wireless_testbench (void){

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
#if 0
	settings_buffer[0] = 0x06;
	SPI_Write(settings_buffer, 0x02, 1);
	
	SPI_Read(settings_buffer, 0x02, 1);
	printf("read 0x%02x\n", settings_buffer[0]);
#endif
	printf("\n\n");
	
	// ------------------ SPI testing -----------------------
	
	uint8_t buffer[2];
	address = 0x31;
	uint16_t bytes = 0x02;
	
	// --- Strobe ----
	uint8_t status;
	status = status_state(CC2500_Strobe(SNOP));
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
	status = status_state(CC2500_Strobe(SNOP));
	printf("status: 0x%02x\n", status);
	
	wireless_delay(100);
	
	status = status_state(CC2500_Strobe(SRX));
	printf("status: 0x%02x\n", status);
	
	wireless_delay(100);
	
	while (status != 0x01) {
		status = status_state(CC2500_Strobe(SNOP));
		printf("status: 0x%02x\n", status);
	
		wireless_delay (100);
	}
	
	status = status_state(CC2500_Strobe(SIDLE));
	printf("status: 0x%02x\n", status);
	
	wireless_delay(100);
	
	status = status_state(CC2500_Strobe(SNOP));
	printf("status: 0x%02x\n", status);
#endif
	// ---------------- Transmit testing --------------------
#if 1
	uint8_t i = 0;
	//uint8_t buff;
	while (i<11) {
		uint8_t pkt;
		makeLCD2MotorPkt(&pkt, i, 0);
		printf("transmitting: 0x%02x\n", pkt);
		CC2500_Transmit(&pkt, 1);
		//CC2500_ReadRecvBuffer(&buff);
		i = (i + 1) % 17;
		wireless_delay(10000000);
	}
#endif
#if 0
	uint8_t buff;
	while (1) {
		ReadRecvBuffer(&buff);
		
		delay(100);
	}
#endif
}
