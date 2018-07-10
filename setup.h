
#ifndef SETUP_H
#define SETUP_H

#include "stm32f30x.h"
#include "ibus.h"
#include "motor.h"
#include "led.h"


void init_periph();
static void uart_init();
static void periph_clock_init();
static void tim_init();
static void interrupt_config();
static void i2c_config();
static void dma_init();

#endif
