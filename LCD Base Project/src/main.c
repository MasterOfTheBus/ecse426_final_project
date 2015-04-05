/*----------------------------------------------------------------------------
 * CMSIS-RTOS 'main' function template
 *---------------------------------------------------------------------------*/

#define osObjectsPublic                     // define objects in main module
#include "osObjects.h"                      // RTOS object definitions
#include "stm32f4xx.h"                  // Device header

#include "stm32f4xx_conf.h"
#include "stm32f429i_discovery.h"
#include "stm32f429i_discovery_lcd.h"
#include "stm32f429i_discovery_l3gd20.h"
#include "background16bpp.h"

#include <stdio.h>
#include <string.h>

#include "wireless.h"

static void delay(__IO uint32_t nCount)
{
  __IO uint32_t index = 0; 
  for(index = 100000*nCount; index != 0; index--)
  {
  }
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
  * @brief    Illustartes a simple shape draw and fill, and string dsiplay
  * @function This function draws concentrated colour-filled circles. It also draw a square and a triangle. Some text at two
              different font sizes is displayed.
  * @param    None
  * @retval   None
  */

void example_1a(void const *argument){
	while(1){
		/* Clear the LCD */ 
    LCD_Clear(LCD_COLOR_WHITE);
	
	  //The files source and header files implement drawing characters (drawing strings)
	  //using different font sizes, see the file font.h for the four sizes
    LCD_SetFont(&Font8x8);
	  //The number of string lines avaialble is dependant on the font height:
	  //A font height of 8 will result in 320 / 8 = 40 lines
    LCD_DisplayStringLine(LINE(1), (uint8_t*)"      Welcome to uP lab     ");
    LCD_DisplayStringLine(LINE(2), (uint8_t*)"          Good Luck         ");
	  
	  //The stm32f429i_discovery_lcd.h file offers functions which allows to draw various shapes
	  //in either border or filled with colour. You can draw circles, rectangles, triangles, lines,
	  //ellipses, and polygons. You can draw strings or characters, change background/foreground 
	  //colours.
	
	  LCD_DrawLine(0, 32, 240, LCD_DIR_HORIZONTAL);
	  LCD_DrawLine(0, 34, 240, LCD_DIR_HORIZONTAL);
	  LCD_SetTextColor(LCD_COLOR_BLUE2); 
	  LCD_DrawFullCircle(120, 160, 100);
	  LCD_SetTextColor(LCD_COLOR_CYAN); 
	  LCD_DrawFullCircle(120, 160, 90);
	  LCD_SetTextColor(LCD_COLOR_YELLOW); 
	  LCD_DrawFullCircle(120, 160, 80);
	  LCD_SetTextColor(LCD_COLOR_RED); 
	  LCD_DrawFullCircle(120, 160, 70);
	  LCD_SetTextColor(LCD_COLOR_BLUE); 
	  LCD_DrawFullCircle(120, 160, 60);
	  LCD_SetTextColor(LCD_COLOR_GREEN); 
	  LCD_DrawFullCircle(120, 160, 50);
	  LCD_SetTextColor(LCD_COLOR_BLACK); 
	  LCD_DrawFullCircle(120, 160, 40);
		LCD_SetTextColor(LCD_COLOR_WHITE);
		LCD_DrawRect(90,130,60,60);
		LCD_SetTextColor(LCD_COLOR_MAGENTA);
		LCD_FillTriangle(90, 120, 150, 130, 180, 130);
		LCD_SetFont(&Font12x12);
		LCD_DisplayStringLine(LINE(15), (uint8_t*)"      Success!    ");
		
		osDelay(250);
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
  * @brief    Displays a bitmap image
  * @function This function copies a bitmap image converted by STM32 Imager into an array "background16bpp.h" and stored
in flash memory into the active buffer in SDRAM. The SDRAM has two layer buffer:
              ->Background layer at address LCD_FRAME_BUFFER = 0xD0000000
              ->Foreground layer at address LCD_FRAME_BUFFER + BUFFER_OFFSET = 0xD0000000 + 0x00050000
              memcpy is a processor intiated and managed transfer. A more efficient way is to use DMA2D ChromeART acccelerator
              Once the image is copied into the active buffer (which we set by LCD_SetLayer(LCD_FOREGROUND_LAYER) command ), the
              LTDC updates the display when it continously refreshes the output display
  * @param    None
  * @retval   None
  */

void example_1b(void const *argument){
	while(1){
		memcpy ( (void *)(LCD_FRAME_BUFFER + BUFFER_OFFSET), (void *) &background, sizeof(background));
		printf("hellow world\n");
		osDelay(250);
	}
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
  * @brief    Illustartes a simple animation program
  * @function This function draws concentrated circles emanating from the center towards the LCD edge in
              an animated fashion. It will look as a sonar or radar display. Then it simulates locking 
              onto a target by flashing a small red circle and displaying the text "Object located"
  * @param    None
  * @retval   None
  */

void example_1c(void const *argument){
	while(1){
			/* Clear the LCD */ 
		LCD_Clear(LCD_COLOR_WHITE);
		LCD_SetFont(&Font8x8);
		LCD_DisplayStringLine(LINE(1), (uint8_t*)"  Radar Scanning for Object  ");
		
		LCD_SetTextColor(LCD_COLOR_BLUE2);
		LCD_DrawLine(10, 160, 220, LCD_DIR_HORIZONTAL);
		LCD_DrawLine(120, 50, 220, LCD_DIR_VERTICAL );
		
		LCD_SetTextColor(LCD_COLOR_BLUE2);
		LCD_DrawCircle(120, 160, 10);	
		delay(35);
		LCD_DrawCircle(120, 160, 20);	
		delay(35);
		LCD_DrawCircle(120, 160, 30);	
		delay(35);
		LCD_DrawCircle(120, 160, 40);	
		delay(35);
		LCD_DrawCircle(120, 160, 50);	
		delay(35);
		LCD_DrawCircle(120, 160, 60);	
		delay(35);
		LCD_DrawCircle(120, 160, 70);	
		delay(35);
		LCD_DrawCircle(120, 160, 80);	
		delay(35);
		LCD_DrawCircle(120, 160, 90);	
		delay(35);
		LCD_DrawCircle(120, 160, 100);	
		delay(35);
		LCD_SetTextColor(LCD_COLOR_RED);
		LCD_DisplayStringLine(LINE(36), (uint8_t*)"        Object Located    ");
		LCD_DrawFullRect(90,130,10,10);
		delay(25);
		LCD_SetTextColor(LCD_COLOR_WHITE);
		LCD_DrawFullRect(90,130,10,10);
		delay(25);
		LCD_SetTextColor(LCD_COLOR_RED);
		LCD_DrawFullRect(90,130,10,10);
		delay(25);
		LCD_SetTextColor(LCD_COLOR_WHITE);
		LCD_DrawFullRect(90,130,10,10);
		delay(25);
		LCD_SetTextColor(LCD_COLOR_RED);
		LCD_DrawFullRect(90,130,10,10);
		delay(25);
		LCD_SetTextColor(LCD_COLOR_WHITE);
		LCD_DrawFullRect(90,130,10,10);
		delay(25);
		LCD_SetTextColor(LCD_COLOR_RED);
		LCD_DrawFullRect(90,130,10,10);
		delay(25);
		LCD_SetTextColor(LCD_COLOR_WHITE);
		LCD_DrawFullRect(90,130,10,10);
		delay(25);
		LCD_SetTextColor(LCD_COLOR_RED);
		LCD_DrawFullRect(90,130,10,10);
		delay(25);
		
		osDelay(250);
	}
}

osThreadDef(example_1a, osPriorityNormal, 1, 0);
osThreadDef(example_1b, osPriorityNormal, 1, 0);
osThreadDef(example_1c, osPriorityNormal, 1, 0);

// ID for theads
osThreadId example_1a_thread;
osThreadId example_1b_thread;
osThreadId example_1c_thread;

// To test wireless module and SPI
void wireless_testbench (){
	// initialize wireless SPI
	wireless_Init();
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
	while (1){
		Transmit(&i, 1);
		i = (i + 1) % 10;
		delay(100);
	}
#endif
}

/*
 * main: initialize and start the system
 */
int main (void) {
	wireless_testbench();
	
  osKernelInitialize ();                    // initialize CMSIS-RTOS
	
  // initialize peripherals here
	/* LCD initiatization */
//  LCD_Init();
  
//  /* LCD Layer initiatization */
//  LCD_LayerInit();
//    
//  /* Enable the LTDC controler */
//  LTDC_Cmd(ENABLE);
//  
//  /* Set LCD foreground layer as the current layer */
//  LCD_SetLayer(LCD_FOREGROUND_LAYER);
	
	
	
  // create 'thread' functions that start executing,
  // example: tid_name = osThreadCreate (osThread(name), NULL);
	
	/*******************************************************
	         Uncomment the example you want to see
	example_1a: Simple shape draw, fill and text display
	example_1b: bitmap image display
	example_1c: Simple animation
	********************************************************/
	
	//example_1a_thread = osThreadCreate(osThread(example_1a), NULL);
	//example_1b_thread = osThreadCreate(osThread(example_1b), NULL);
	//example_1c_thread = osThreadCreate(osThread(example_1c), NULL);
	
	osKernelStart ();                         // start thread execution 
}


