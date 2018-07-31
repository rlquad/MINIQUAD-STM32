
#include "uart.h"


void OutString(char *s)
{
  while(*s)
  {
    while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);                     // Wait for Empty

    USART_SendData(USART2, *s++);                                                   // Send Char
  }
}

void print_ang(double a1,double a2,double a3,double a4,double a5,double a6,double a7,double a8)
{
    char dummy[100];

  /*  if(choice==0)

        sprintf(dummy,"ROLL %f PITCH %f YAW %f \r\n",roll,pitch,yaw);

    else if(choice==1)

        sprintf(dummy,"ARM %d THROTTLE %d PITCH %d ROLL %d YAW %d CHANNEL 6 %d \r\n",rcvalue[ARM],rcvalue[THROTTLE],rcvalue[PITCH],rcvalue[ROLL_R],rcvalue[YAW_R],rcvalue[CHN_6]);

    else*/
        //sprintf(dummy,"%f \r\n",roll);
        sprintf(dummy,"%f,%f,%f,%f,%f,%f,%f,%f \r\n",a1,a2,a3,a4,a5,a6,a7,a8);

    OutString(dummy);


}



