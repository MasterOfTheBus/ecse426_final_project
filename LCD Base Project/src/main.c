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


#include <stdio.h>
#include <string.h>
#include "keypad.h"
#include "LCD.h"
#include "math.h"

#define PI 3.141592653589793238462643383279502884197169399375105820974944592307816406286

// For keypad
int direction;
int mode;
int shape;
int send;
int drawn=0; 
int userInput;

// For LCD
double x1=90;
double x2=90;
double y1=130;
double y2=100;
double xBlink=90;
double yBlink=100;
int blink = 1;
int done=0;


static void delay(__IO uint32_t nCount)
{
  __IO uint32_t index = 0; 
  for(index = 100000*nCount; index != 0; index--)
  {
  }
}


// ID for theads
//osThreadId example_1a_thread_id;
//osThreadId example_1b_thread_id;
//osThreadId example_1c_thread_id;
osThreadId keypad_thread_id;
osThreadId drawSquare_thread_id;
osThreadId blinkSquare_thread_id;
osThreadId drawRectangle_thread_id;
osThreadId blinkRectangle_thread_id;
osThreadId drawTriangle_thread_id;
osThreadId blinkTriangle_thread_id;
osThreadId drawLine_thread_id;
osThreadId blinkLine_thread_id;

void wireless_receive(){ //set x1, x2
	
	
}
void wireless_send(){
	
}
void keypad_thread(void const *argument){
	int lastMode;
	int lastDirection;
	int lastShape;
	int lastUserInput;
	
	double last_x1;
	double last_y1;
	while(1){
			Keypad_read();
//		shape =1;
//		send=1;
//		direction = 45;
//		mode = 1;
		
		
		printf("--------------------");	
		printf("send: %i\n", send);		
		printf("shape: %i\n", shape);
		printf("direction: %i\n", direction);
		printf("mode: %i\n", mode);
	//	done =0;
		if(/*shape != 0 &&*/(shape != lastShape || direction!=lastDirection || mode!=lastMode)){
			if (send == 0) blink = 1;
			else {
				blink = 0;
				// send mode, direction and shape
			}
			printf("blink: %i\n", blink);
		}
		
		if (direction == RESET){
			LCD_Clear(LCD_COLOR_WHITE);
			x1=90;
			y1=130;
		}
		if (mode == STAR){		//*
			//printf("++++++++++SHAPE: %i\n", shape);
			if (shape == SQUARE) {
				if(blink == 0) {
					osSignalSet (drawSquare_thread_id, 0x01);
					//osDelay(50);
					
				}else {
					osSignalSet (blinkSquare_thread_id, 0x01);
					osDelay(50);
				}
				
				lastShape = shape;
				lastMode = mode;
				lastDirection = direction;
				lastUserInput = userInput;
			}else if (shape == RECTANGLE) {
				if(blink == 0) {
					osSignalSet (drawRectangle_thread_id, 0x01);
			//		osDelay(50);
				}else{
					osSignalSet (blinkRectangle_thread_id, 0x01);
					osDelay(50);
				}
				lastShape = shape;
				lastMode = mode;
				lastDirection = direction;
				lastUserInput = userInput;
			}else if (shape == TRIANGLE) {
				if(blink == 0) {
					osSignalSet (drawTriangle_thread_id, 0x01);
			//		osDelay(50);
				}else {
					osSignalSet (blinkTriangle_thread_id, 0x01);
					osDelay(50);
				}
				lastShape = shape;
				lastMode = mode;
				lastDirection = direction;
				lastUserInput = userInput;
			}else if (shape == 0) {
				
				if(send ==1) blink =0;
				else blink =1;
				
				x2 = x1-25*cos(PI*direction/180);
				y2 = y1+25*sin(PI*direction/180);
				if(blink == 0) {
					osSignalSet (drawLine_thread_id, 0x01);
					osDelay(50);
					x1=x2;
					y1=y2;
					
				}else {
	//				printf("blink the line!!!");
					osSignalSet (blinkLine_thread_id, 0x01);
					osDelay(50);
				}

			}
		
		}else { //#
		
		}
		
		
		
		osDelay(100);
	}
	

}

//osThreadDef(example_1a, osPriorityNormal, 1, 0);
//osThreadDef(example_1b, osPriorityNormal, 1, 0);
//osThreadDef(example_1c, osPriorityNormal, 1, 0);
osThreadDef(keypad_thread, osPriorityNormal, 1, 0);
osThreadDef(drawSquare_thread, osPriorityNormal, 1, 0);
osThreadDef(blinkSquare_thread, osPriorityNormal, 1, 0);
osThreadDef(drawRectangle_thread, osPriorityNormal, 1, 0);
osThreadDef(blinkRectangle_thread, osPriorityNormal, 1, 0);
osThreadDef(drawTriangle_thread, osPriorityNormal, 1, 0);
osThreadDef(blinkTriangle_thread, osPriorityNormal, 1, 0);
osThreadDef(drawLine_thread, osPriorityNormal, 1, 0);
osThreadDef(blinkLine_thread, osPriorityNormal, 1, 0);


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

	keypad_thread_id = osThreadCreate(osThread(keypad_thread), NULL);
	drawSquare_thread_id = osThreadCreate(osThread(drawSquare_thread), NULL);
	blinkSquare_thread_id = osThreadCreate(osThread(blinkSquare_thread), NULL);	
	drawRectangle_thread_id = osThreadCreate(osThread(drawRectangle_thread), NULL);
	blinkRectangle_thread_id = osThreadCreate(osThread(blinkRectangle_thread), NULL);	
	drawTriangle_thread_id = osThreadCreate(osThread(drawTriangle_thread), NULL);
	blinkTriangle_thread_id = osThreadCreate(osThread(blinkTriangle_thread), NULL);	
	drawLine_thread_id = osThreadCreate(osThread(drawLine_thread), NULL);
	blinkLine_thread_id = osThreadCreate(osThread(blinkLine_thread), NULL);		
	
	osKernelStart ();                         // start thread execution 
}
				


