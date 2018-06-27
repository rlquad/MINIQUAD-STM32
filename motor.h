#ifndef MOTOR_H
#define MOTOR_H

#include "stm32f30x.h"
#include "ibus.h"
#include "pid.h"
#include "uart.h"
#include "defines.h"

#define TimerPeriod  ((16000000/500) + 75)

uint16_t constrain(uint16_t ,uint16_t ,uint16_t );

void motor_update(void);

#endif
