#include "packet.h"

void makeLCD2MotorPkt(uint8_t *pkt, uint8_t keypad_val, uint8_t state) {
	*pkt = 0x00;
	
	*pkt |= keypad_val;
	*pkt |= (state >> 4);
}

uint8_t makeCallbackPkt(uint8_t *pkt, int8_t x, int8_t y, uint8_t fail) {
	if (fail) {
		*pkt = 0x00;
	} else {
		if ((x < -7 || x > 7) || (y < 6 || y > 12)) {
			return 0;
		}
		int8_t pkt_x = x + 7;
		uint8_t pkt_y = y - 6;
		*pkt = 0x00;
		*pkt = *pkt | (pkt_x >> 4);
		*pkt = *pkt | pkt_y;
	}
	return 1;
}
