
#include "uart.h"


void OutString(char *s)
{
  while(*s)
  {
    while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);                     // Wait for Empty

    USART_SendData(USART2, *s++);                                                   // Send Char
  }
}

void print_ang(int choice,int dt)
{
    char dummy[100];

    if(choice==0)

        sprintf(dummy,"ROLL %f PITCH %f YAW %f \r\n",roll,pitch,yaw);

    else if(choice==1)

        sprintf(dummy,"ARM %d THROTTLE %d PITCH %d ROLL %d YAW %d CHANNEL 6 %d \r\n",rcvalue[ARM],rcvalue[THROTTLE],rcvalue[PITCH],rcvalue[ROLL_R],rcvalue[YAW_R],rcvalue[CHN_6]);

    else
        sprintf(dummy,"%d \r\n",dt);

    OutString(dummy);


}



