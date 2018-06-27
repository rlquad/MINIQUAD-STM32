#ifndef IMU_H
#define IMU_H

#include "stm32f30x.h"

#define TIMEOUT 1000

extern int16_t gyro[3],acc[3];
extern int16_t gyro_cal[3];

static int check_imu_connect();
void wrt_reg(int ,int);
void wrt_reg_data(int,int,int);
int recv_byte();
void recv_orient_data();
void imu_config();
void clear_int();
void mpu_reset();




#endif
