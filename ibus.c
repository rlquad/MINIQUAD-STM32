#include "ibus.h"

volatile uint8_t buff_data[BUFF_SIZE];

 int rcvalue[6];

int rc_status = 0;

const float roll_coeff  = 0.1;
const float pitch_coeff = 0.1;
const float yaw_coeff   = 0.1;

void USART2_IRQHandler(void)
{   volatile uint8_t tmp;
    if (USART_GetFlagStatus(USART2,USART_FLAG_RXNE) == SET)
    {
       tmp = USART2->RDR;
       USART_ClearFlag(USART2,USART_FLAG_ORE);
    }

    if (USART_GetFlagStatus(USART2,USART_FLAG_IDLE) == SET)
    {
      USART_ClearFlag(USART2,USART_FLAG_IDLE);

      USART_ITConfig(USART2,USART_IT_IDLE,DISABLE);
      USART_ITConfig(USART2,USART_IT_RXNE,DISABLE);

      DMA_Cmd(DMA1_Channel6, ENABLE);
    }
}

void DMA1_Channel6_IRQHandler(void) {

    if (DMA_GetITStatus(DMA1_IT_TC6)) {
         DMA_ClearITPendingBit(DMA1_IT_TC6);          /* Clear transfer complete flag */
         rc_status = 1;

    }
}


void read_recv()
{
  int start = 0;
  int index;
  rc_status = 0;


  if((buff_data[1]!=0x40)&&(buff_data[1]!=0x20))
  {
    return;
  }

  if(buff_data[1]==0x40)
    start = 0;
  else if(buff_data[1]==0x20)
    start = 1;

  index = 0;
    for(int i = start; index<6; i+=2,index++)
    {
      rcvalue[index] = (buff_data[i + 3]<<8)|buff_data[i + 2];
    }

    rcvalue[0] = (rcvalue[0] - 1500)*yaw_coeff;
    rcvalue[1] = (rcvalue[1] - 1500)*pitch_coeff;
    rcvalue[3] = (rcvalue[3] - 1500)*roll_coeff;

}
