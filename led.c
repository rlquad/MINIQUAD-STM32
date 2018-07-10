#include "led.h"

void led_init()
{
  GPIO_InitTypeDef gpio_init;

  gpio_init.GPIO_Pin   = GPIO_Pin_1;
  gpio_init.GPIO_Mode  = GPIO_Mode_OUT;
  gpio_init.GPIO_Speed = GPIO_Speed_Level_3;
  gpio_init.GPIO_OType = GPIO_OType_PP;
  gpio_init.GPIO_PuPd  = GPIO_PuPd_NOPULL;

  GPIO_Init(GPIOF, &gpio_init);


}

void led_on()
{
    GPIO_SetBits(GPIOF,GPIO_Pin_1);

}

void led_off()
{
    GPIO_ResetBits(GPIOF,GPIO_Pin_1);
}
