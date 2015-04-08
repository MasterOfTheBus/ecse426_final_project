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
#include "LCD.h"
#include "math.h"

#define PI 3.14159265359
int direction;
int mode;
int shape;
int send;

uint16_t x1;
uint16_t y1;
uint16_t x2;
uint8_t y2;


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

// ID for theads
osThreadId example_1a_thread_id;
osThreadId example_1b_thread_id;
osThreadId example_1c_thread_id;
osThreadId square_thread_id;
osThreadId rectangle_thread_id;
osThreadId triangle_thread_id;
osThreadId line_thread_id;
osThreadId lcd_drawing_thread_id;

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
	
void square(void const *argument){
	while(1){
		printf("square");
		osSignalWait(0x01,osWaitForever);
	  LCD_DrawFullCircle(120, 160, 50);
		LCD_SetTextColor(LCD_COLOR_BLUE2); 
		
	 
		osDelay(50);
	}
}
void rectangle(void const *argument){
	while(1){
		printf("rectangle");
		osSignalWait(0x01,osWaitForever);
		LCD_DrawRect(90,130,60,60);
		LCD_SetTextColor(LCD_COLOR_MAGENTA);		
		
		osDelay(50);
	}
}

void triangle(void const *argument){
	while(1){
		printf("triangle");
		osSignalWait(0x01,osWaitForever);
		LCD_FillTriangle(90, 120, 150, 130, 180, 130);
		LCD_SetTextColor(LCD_COLOR_RED);

		
		osDelay(50);
	}
}

void line(void const *argument){
	while(1){
		printf("line");
		osSignalWait(0x01,osWaitForever);
		LCD_SetTextColor(LCD_COLOR_RED);
		//LCD_DrawLine(uint16_t Xpos, uint16_t Ypos, uint16_t Length, uint8_t Direction);
		//LCD_DrawUniLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
		LCD_DrawUniLine(x1,y1,x2,y2);
		
		osDelay(50);
		}
	}

	
void lcd_drawing_thread(void const *arguemnt){
	
while(1){

	lcd_drawing();
	//printf("mode: %i \ndirection: %i \nshape: %i", mode, direction, shape );
	if (send == 1){
			if (shape == 0 && direction == RESET){ // move to bottom left corner
				LCD_Clear(LCD_COLOR_WHITE);
				x1=0;
				x2=0;
				y1=0;
				y2=0;
			}

		if(mode == STAR){
	
				if (shape == SQUARE){
					
				 osSignalSet(square_thread_id, 0x01);
					
				} else if (shape == RECTANGLE){
						 osSignalSet(rectangle_thread_id, 0x01);
				}
					else if (shape == TRIANGLE){
						osSignalSet(triangle_thread_id, 0x01);
				}
					else if (shape ==0){
						x1=x2;
						x2=x2+10*cos(PI*direction/180);
						y1=y2;
						y2=y2+10*sin(PI*direction/180);
						osSignalSet(line_thread_id, 0x01);
						

						
				
					}
				}
			}
		//printf("before delay");
		osDelay(250);
		}
	
}
	

osThreadDef(example_1a, osPriorityNormal, 1, 0);
osThreadDef(example_1b, osPriorityNormal, 1, 0);
osThreadDef(example_1c, osPriorityNormal, 1, 0);
osThreadDef(square, osPriorityNormal, 1, 0);
osThreadDef(rectangle, osPriorityNormal, 1, 0);
osThreadDef(triangle, osPriorityNormal, 1, 0);
osThreadDef(line, osPriorityNormal, 1, 0);
osThreadDef(lcd_drawing_thread,osPriorityNormal, 1,0);




/*
 * main: initialize and start the system
 */
int main (void) {
	
  osKernelInitialize ();                    // initialize CMSIS-RTOS
	
  // initialize peripherals here
	/* LCD initiatization */
  LCD_Init();
  
  /* LCD Layer initiatization */
  LCD_LayerInit();
    
  /* Enable the LTDC controler */
  LTDC_Cmd(ENABLE);
  
  /* Set LCD foreground layer as the current layer */
  LCD_SetLayer(LCD_FOREGROUND_LAYER);
	LCD_Clear(LCD_COLOR_WHITE);
	
	lcd_drawing_thread_id=osThreadCreate(osThread(lcd_drawing_thread), NULL);
	square_thread_id = osThreadCreate(osThread(square), NULL);
	rectangle_thread_id = osThreadCreate(osThread(rectangle), NULL);
	triangle_thread_id = osThreadCreate(osThread(triangle), NULL);
	line_thread_id = osThreadCreate(osThread(line), NULL);
		

	
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
				


