#include "pid.h"

/*
float Kd_roll = 0;
float Kd_pitch = 0;
float Kp_roll = 4;
float Kp_pitch = 4;
*/
int yaw_des = 0;
float last_error[3];

float* pid_set()
{
    float error[3], pid_correct[3];

  //  Kd_calc();

    error[ROLL]  = roll - roll_des;
    error[PITCH] = pitch - pitch_des;
    error[YAW]   = yaw - yaw_des;

    pid_correct[ROLL]  = Kp_roll *(error[ROLL])  + Kd_roll *(error[ROLL] - last_error[ROLL]);
    pid_correct[PITCH] = Kp_pitch*(error[PITCH]) + Kd_pitch*(error[PITCH] - last_error[PITCH]);
    pid_correct[YAW]   = Kp_yaw  *(error[YAW])   + Kd_yaw  *(error[YAW] - last_error[YAW]);


    last_error[ROLL]  = error[ROLL];
    last_error[PITCH] = error[PITCH];
    last_error[YAW]   = error[YAW];

    return pid_correct;
}

/*
void Kd_calc()
{
    Kd_roll = 0.2*rcvalue[CHN_6] - 200.0;
    Kd_pitch = 0.2*rcvalue[CHN_6] - 200.0;
}
*/
