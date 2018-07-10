#ifndef PID_H
#define PID_H

#include "stm32f30x.h"
#include "angle.h"
#include "ibus.h"
#include "defines.h"
#include "uart.h"

#define Kp_roll  4
#define Kd_roll  200.0
#define Ki_roll  0

#define Kp_pitch 4
#define Kd_pitch 200.0
#define Ki_pitch 0

#define Kp_yaw   1
#define Kd_yaw   1
#define Ki_yaw   1

#define roll_des  0
#define pitch_des 0
#define yaw_des   0

float* pid_set();
void Kd_calc();

#endif

