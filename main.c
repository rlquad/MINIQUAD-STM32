#include "stm32.h"

int main()
{
  init_periph();
  
  while(1)
  {
    if(rc_status)
      read_recv();
  
   // print_ang(1,3);
    
  }
  
  return 0;
}
