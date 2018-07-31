#ifndef q_angle_h
#define q_angle_h

#include "stm32f30x.h"
#include "imu.h"
#include "angle.h"
#include "math.h"
#include "uart.h"

extern double q0,q1,q2,q3;

void ang_update(float ,float ,float ,float ,float ,float ,int );
float invSqrt(float );

#endif /* q_angle_h */
