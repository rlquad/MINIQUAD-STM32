#include "motor.h"

//////////////////////////////////////////////////////////////
//CHANNEL 1 - MOTOR 1 - PB4 - D12
//CHANNEL 2 - MOTOR 2 - PA4 - A3
//CHANNEL 3 - MOTOR 3 - PB0 - D3
//CHANNEL 4 - MOTOR 4 - PB1 - D6
//////////////////////////////////////////////////////////////

void motor_update(void)
{
   uint16_t Channel1Pulse = 0,Channel2Pulse = 0,Channel3Pulse = 0,Channel4Pulse = 0;

   float* correction;

   //
   correction = pid_set();
   float roll_correct = *correction;
   float pitch_correct = *(correction + 1);

   if(rcvalue[ARM]>1500 && rcvalue[THROTTLE]>1050)                     //ARMING ONLY DONE IF ARM SWITCH ENABLED AND THROTTLE NOT 1000
      {
      //print_ang(2,rcvalue[THROTTLE]);
       print_ang(2,*correction,*(correction + 1));
       Channel1Pulse = (uint16_t) ((( constrain((rcvalue[THROTTLE] + rcvalue[YAW_R] - rcvalue[PITCH] + rcvalue[ROLL_R] + roll_correct - pitch_correct)/8.0,125,250) ) * (TimerPeriod)) / 1000.0);
       Channel2Pulse = (uint16_t) ((( constrain((rcvalue[THROTTLE] - rcvalue[YAW_R] + rcvalue[PITCH] + rcvalue[ROLL_R] + roll_correct + pitch_correct)/8.0,125,250) ) * (TimerPeriod)) / 1000.0);
       Channel3Pulse = (uint16_t) ((( constrain((rcvalue[THROTTLE] + rcvalue[YAW_R] + rcvalue[PITCH] - rcvalue[ROLL_R] - roll_correct + pitch_correct)/8.0,125,250) ) * (TimerPeriod)) / 1000.0);
       Channel4Pulse = (uint16_t) ((( constrain((rcvalue[THROTTLE] - rcvalue[YAW_R] - rcvalue[PITCH] - rcvalue[ROLL_R] - roll_correct - pitch_correct)/8.0,125,250) ) * (TimerPeriod)) / 1000.0);

      }
   else

      { //print_ang(1,0);
        Channel1Pulse = (uint16_t) ( 12.5 * (TimerPeriod) / 100);
        Channel2Pulse = (uint16_t) ( 12.5 * (TimerPeriod) / 100);
        Channel3Pulse = (uint16_t) ( 12.5 * (TimerPeriod) / 100);
        Channel4Pulse = (uint16_t) ( 12.5 * (TimerPeriod) / 100);
      }

 //print_ang(2,  constrain((rcvalue[THROTTLE] + rcvalue[YAW_R] - rcvalue[PITCH] - rcvalue[ROLL_R] )/8,125,250));

    TIM3->CCR1 = Channel1Pulse;
    TIM3->CCR2 = Channel2Pulse;
    TIM3->CCR3 = Channel3Pulse;
    TIM3->CCR4 = Channel4Pulse;


}

double constrain(uint16_t x,uint16_t a,uint16_t b)
{
    if(x<a)
        return a;
    else if(x>b)
        return b;
    else
        return x;
}

