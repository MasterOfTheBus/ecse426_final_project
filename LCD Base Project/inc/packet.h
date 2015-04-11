#include "stm32f4xx.h"
#include "stm32f4xx_conf.h"

#define PKT_LENGTH 1 // byte
#define CONFIRMATION_PKT 0x99

void makeLCD2MotorPkt(uint8_t *pkt, uint8_t keypad_val, uint8_t state/*, uint8_t control*/);
uint8_t makeCallbackPkt(uint8_t *pkt, int8_t x, int8_t y, uint8_t fail);

//uint8_t getX
