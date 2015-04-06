#define osObjectsPublic                     // define objects in main module
#include "osObjects.h"                      // RTOS object definitions
#include "stm32f429i_discovery_lcd.h"
#include "keypad.h"


int mode;
uint16_t x1;
uint16_t y1;
uint16_t x2;
uint8_t y2;

// ID for theads
osThreadId circle_thread;
osThreadId square_thread;
osThreadId triangle_thread;
osThreadId line_thread;
osThreadId keypad_thread;
osThreadId lcd_drawing_thread;

	
void circle(void const *argument){
	while(1){

    LCD_Clear(LCD_COLOR_WHITE);
	  LCD_DrawFullCircle(120, 160, 100);
			  LCD_SetTextColor(LCD_COLOR_BLUE2); 
	 
		osDelay(250);
	}
}
void square(void const *argument){
	while(1){
		
    LCD_Clear(LCD_COLOR_WHITE);
		LCD_DrawRect(90,130,60,60);
		LCD_SetTextColor(LCD_COLOR_MAGENTA);		
		
		osDelay(250);
	}
}

void triangle(void const *argument){
	while(1){

    LCD_Clear(LCD_COLOR_WHITE);
		LCD_FillTriangle(90, 120, 150, 130, 180, 130);
		LCD_SetTextColor(LCD_COLOR_RED);

		
		osDelay(250);
	}
}

void line(void const *argument){
	while(1){
		LCD_SetTextColor(LCD_COLOR_BLACK);
		//LCD_DrawLine(uint16_t Xpos, uint16_t Ypos, uint16_t Length, uint8_t Direction);
		//LCD_DrawUniLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
		LCD_DrawUniLine(x1, y1, x2, y2);

		osDelay(250);
	}
}

osThreadDef(circle, osPriorityNormal, 1, 0);
osThreadDef(square, osPriorityNormal, 1, 0);
osThreadDef(triangle, osPriorityNormal, 1, 0);
osThreadDef(line, osPriorityNormal, 1, 0);

void keypad(void const *argument)
{	
	while(1){
		char c = get_key();
		switch(c){
			case '#':
				mode = (mode + 1) %2;
				break;
		}
	}
}

void lcd_drawing(void const *argument)
{	
	while(1){
		if(mode == 0){															//Predefined shape mode
		while(1){
			char c = get_key();
			switch(c){
			case '1':
				circle_thread = osThreadCreate(osThread(circle), NULL);
				break;
			case '2':
				square_thread = osThreadCreate(osThread(square), NULL);
				break;
			case '3':
				triangle_thread = osThreadCreate(osThread(triangle), NULL);
				break;
		}
	}
}
			else if(mode == 1){ 												//Drawing mode
			while(1){
				char c = get_key();
				switch(c){
					if (x1< 240 && y1<320){
						
				case '6':																	// left
					x1=x2;
					x2=x2+10;
					line_thread = osThreadCreate(osThread(line), NULL);
					break;
				case '4':																	// right
					x1=x2;
					x2=x2-10;
					line_thread = osThreadCreate(osThread(line), NULL);
					break;
				case '8':																	// up
					y1=y2;
					y2=y2+10;
					line_thread = osThreadCreate(osThread(line), NULL);
					break;
				case '2':													 				// down
					y1=y2;
					y2=y2-10;
					line_thread = osThreadCreate(osThread(line), NULL);
					break;
				case '5':																	// reset
					LCD_Clear(LCD_COLOR_WHITE);
					x1=0;
					x2=0;
					y1=0;
					y2=0;
					break;
				case '3':																	// upper right
					x1=x2;
					x2=x2+8.66;
					y1=y2;
					y2=y2+5;
					line_thread = osThreadCreate(osThread(line), NULL);
					break;
				case '1':																	// upper left
					x1=x2;
					x2=x2-8.66;
					y1=y2;
					y2=y2+5;
					line_thread = osThreadCreate(osThread(line), NULL);
					break;
				case '9':																	// lower right
					x1=x2;
					x2=x2+8.66;
					y1=y2;
					y2=y2-5;
					line_thread = osThreadCreate(osThread(line), NULL);
					break;
				case '7':																	// lower left
					x1=x2;
					x2=x2-8.66;
					y1=y2;
					y2=y2-5;
					line_thread = osThreadCreate(osThread(line), NULL);
					break;
			}
		}
	 }
  }
 }
}

osThreadDef(keypad, osPriorityNormal, 1, 0);
osThreadDef(lcd_drawing, osPriorityNormal, 1, 0);


int main (void) {
 osKernelInitialize ();                    // initialize CMSIS-RTOS
  LCD_Init();
  LCD_LayerInit();
  LTDC_Cmd(ENABLE);
  LCD_SetLayer(LCD_FOREGROUND_LAYER);
	keypad_thread = osThreadCreate(osThread(keypad), NULL);
	lcd_drawing_thread = osThreadCreate(osThread(lcd_drawing), NULL);
	osKernelStart ();                         // start thread execution 
	
}




