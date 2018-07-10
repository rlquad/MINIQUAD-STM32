#ifndef ANGLE_H
#define ANGLE_H

#include "imu.h"
#include "stm32f30x.h"
#include "motor.h"
#include "MadgwickAHRS.h"
#include "led.h"

#define Calibration_Value 2000

extern double roll,pitch,yaw;
extern uint8_t I2C_Buffer[14];
extern uint8_t mpu_calibrated;
extern int angle_state;    //0 - NOT UPDATED, 1 - UPDATED, 2 - TIMEOUT

void angle_calc(int);
void DMA1_Channel7_IRQHandler(void);
void EXTI0_IRQHandler(void);
void calibrate();

#endif

