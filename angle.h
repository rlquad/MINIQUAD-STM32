#ifndef ANGLE_H
#define ANGLE_H

#include "imu.h"
#include "stm32f30x.h"
#include "motor.h"
#include "led.h"
#include "q_angle.h"

#define Calibration_Value 2000
#define LOOP_TIME 995

extern double roll,pitch,yaw;
extern uint8_t I2C_Buffer[14];
extern uint8_t mpu_calibrated;
extern int angle_state;    //0 - NOT UPDATED, 1 - UPDATED, 2 - TIMEOUT
extern int dt;
extern uint32_t timer;
extern float temp;


void angle_calc();
void DMA1_Channel7_IRQHandler(void);
void EXTI0_IRQHandler(void);
void calibrate();

#endif

