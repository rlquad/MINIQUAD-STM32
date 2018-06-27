
#ifndef IBUS_H
#define IBUS_H

#include "stm32f30x.h"
#include "motor.h"
#include "defines.h"

#define BUFF_SIZE  32

extern volatile uint8_t buff_data[BUFF_SIZE];

extern int rcvalue[6];

extern int rc_status;

void DMA1_Channel6_IRQHandler(void);
void read_recv();

#endif
