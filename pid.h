#ifndef PID_H
#define PID_H

#include "stm32f30x.h"
#include "angle.h"
#include "ibus.h"
#include "defines.h"
#include "uart.h"
/*
#define Kp_roll  4
#define Kd_roll  200.0
#define Ki_roll  0

#define Kp_pitch 4
#define Kd_pitch 200.0
#define Ki_pitch 0

#define Kp_yaw   4
#define Kd_yaw   100
#define Ki_yaw   0

*/

#define Kp_roll  0
#define Kd_roll  000.0
#define Ki_roll  0

#define Kp_pitch 0
#define Kd_pitch 00.0
#define Ki_pitch 0

#define Kp_yaw   0
#define Kd_yaw   00
#define Ki_yaw   0


#define roll_des  0
#define pitch_des 0

extern int yaw_des;

float* pid_set();
void Kd_calc();

#endif

