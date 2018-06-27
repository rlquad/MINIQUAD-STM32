#include "motor.h"

void motor_update(void)
{
   uint16_t Channel1Pulse = 0,Channel2Pulse = 0,Channel3Pulse = 0,Channel4Pulse = 0;

   float* correction;

   correction = pid_set();

   if(rcvalue[ARM]>1500 && rcvalue[THROTTLE]>1050)                     //ARMING ONLY DONE IF ARM SWITCH ENABLED AND THROTTLE NOT 1000
      {
        print_ang(2,rcvalue[THROTTLE]);
       Channel1Pulse = (uint16_t) (((uint32_t) constrain((rcvalue[THROTTLE] + rcvalue[YAW_R] - rcvalue[PITCH] - rcvalue[ROLL_R] ),1000,2000)/40 * (TimerPeriod - 1 )) / 100) - 1;
       Channel2Pulse = (uint16_t) (((uint32_t) constrain((rcvalue[THROTTLE] - rcvalue[YAW_R] + rcvalue[PITCH] - rcvalue[ROLL_R] ),1000,2000)/40 * (TimerPeriod - 1 )) / 100) - 1;
       Channel3Pulse = (uint16_t) (((uint32_t) constrain((rcvalue[THROTTLE] + rcvalue[YAW_R] + rcvalue[PITCH] + rcvalue[ROLL_R] ),1000,2000)/40 * (TimerPeriod - 1 )) / 100) - 1;
       Channel4Pulse = (uint16_t) (((uint32_t) constrain((rcvalue[THROTTLE] - rcvalue[YAW_R] - rcvalue[PITCH] + rcvalue[ROLL_R] ),1000,2000)/40 * (TimerPeriod - 1 )) / 100) - 1;

      }
   else

      { //print_ang(1,0);
        Channel1Pulse = (uint16_t) ((uint32_t) 25 * (TimerPeriod - 1 ) / 100) - 1;
        Channel2Pulse = (uint16_t) ((uint32_t) 25 * (TimerPeriod - 1 ) / 100) - 1;
        Channel3Pulse = (uint16_t) ((uint32_t) 25 * (TimerPeriod - 1 ) / 100) - 1;
        Channel4Pulse = (uint16_t) ((uint32_t) 25 * (TimerPeriod - 1 ) / 100) - 1;
      }


    TIM3->CCR1 = Channel1Pulse;
    TIM3->CCR2 = Channel2Pulse;
    TIM3->CCR3 = Channel3Pulse;
    TIM3->CCR4 = Channel4Pulse;


}

uint16_t constrain(uint16_t x,uint16_t a,uint16_t b)
{
    if(x<a)
        return a;
    else if(x>b)
        return b;
    else
        return x;
}

