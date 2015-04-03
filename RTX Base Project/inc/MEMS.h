#ifndef MEMS_h
#define MEMS_h

#include <stdio.h>
#include <math.h>
#include "lis3dsh.h"
#include "KalmanFilter.h"
#include "stm32f4xx.h"
#include "stm32f4xx_conf.h"
#include "cmsis_os.h"

void MEMS_config();
void MEMS_interrupt_config();
void MEMS_read_value();
double get_pitch();
double get_roll();

#endif