#include "ibus.h"

volatile uint8_t buff_data[BUFF_SIZE];

 int rcvalue[6];

int rc_status = 0;

const float roll_coeff  = 0.25;
const float pitch_coeff = 0.25;
const float yaw_coeff   = 0.25;


void DMA1_Channel6_IRQHandler(void) {

    if (DMA_GetITStatus(DMA1_IT_TC6)) {
         DMA_ClearITPendingBit(DMA1_IT_TC6);          /* Clear transfer complete flag */

         rc_status = 1;

    }
}


void read_recv()
{

  if(buff_data[0]!=0x20)
  {
    return;
  }

  if(buff_data[1]!=0x40)
  {
    return;
  }

    rcvalue[YAW_R]    = (((buff_data[3]<<8)|buff_data[2]) - 1500)*yaw_coeff;     //YAW
    rcvalue[PITCH]    = (((buff_data[5]<<8)|buff_data[4]) - 1500)*pitch_coeff;   //PITCH
    rcvalue[THROTTLE] =  (buff_data[7]<<8)|buff_data[6];                       //THROTTLE
    rcvalue[ROLL_R]   = (((buff_data[9]<<8)|buff_data[8]) - 1500)*roll_coeff;    //ROLL
    rcvalue[ARM]      =  (buff_data[11]<<8)|buff_data[10];                     //ARM
    rcvalue[CHN_6]    =  (buff_data[13]<<8)|buff_data[12];                     //CHANNEL 6

    rc_status = 0;

}
