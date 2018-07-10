
#include "setup.h"

 void init_periph()
{
    SystemInit();
    SystemCoreClockUpdate();

    periph_clock_init();
    led_init();
    led_on();
    uart_init();
    tim_init();
    i2c_config();
    dma_init();
    interrupt_config();

}

static void periph_clock_init()
{

  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA,ENABLE);
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB,ENABLE);
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOF,ENABLE);

  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);

  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3 , ENABLE);
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

  RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1,ENABLE);

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

}

static void uart_init()
{

  GPIO_InitTypeDef gpio_init;

  gpio_init.GPIO_Pin   = GPIO_Pin_2|GPIO_Pin_3;
  gpio_init.GPIO_Mode  = GPIO_Mode_AF;
  gpio_init.GPIO_Speed = GPIO_Speed_Level_3;
  gpio_init.GPIO_OType = GPIO_OType_PP;
  gpio_init.GPIO_PuPd  = GPIO_PuPd_NOPULL;

  GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_7);
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_7);

  GPIO_Init(GPIOA, &gpio_init);

  USART_InitTypeDef usart2;

  usart2.USART_BaudRate =  115200;
  usart2.USART_WordLength = USART_WordLength_8b;
  usart2.USART_StopBits = USART_StopBits_1;
  usart2.USART_Parity = USART_Parity_No;
  usart2.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  usart2.USART_Mode = USART_Mode_Tx|USART_Mode_Rx;

  USART_Init(USART2, &usart2);

 // USART2->CR2 = 0x20000010;      //http://www.openstm32.org/forumthread3101

  USART_ITConfig(USART2,USART_IT_IDLE,ENABLE);
  USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);
  USART_DMACmd(USART2, USART_DMAReq_Rx, ENABLE);
  USART_Cmd(USART2, ENABLE);

}


static void tim_init()
{

//////////////////////////////////////////////////////////////////////////
    GPIO_InitTypeDef        gpio_init;
    TIM_OCInitTypeDef       tim_init;
    TIM_TimeBaseInitTypeDef setuptimer;
    TIM_OCInitTypeDef  TIM_OCInitStructure;


/////////////////////////////////////////////////////////////////////////

//TIMER 2 SETUP
//////////////////////////////////////////////////////////////////////////

    setuptimer.TIM_Prescaler = 64 - 1;                                                    //DONT KNOW WHY
    setuptimer.TIM_CounterMode = TIM_CounterMode_Up;
    setuptimer.TIM_Period = 0xFFFFFFF;
    setuptimer.TIM_ClockDivision = 0;
    setuptimer.TIM_RepetitionCounter = TIM_CKD_DIV1;

    TIM_TimeBaseInit(TIM2, &setuptimer);

    TIM_Cmd(TIM2, ENABLE);



//////////////////////////////////////////////////////////////////////////
//TIMER 3 SETUP
//////////////////////////////////////////////////////////////////////////

//http://www.micromouseonline.com/2016/02/06/pwm-basics-on-the-stm32-general-purpose-timers/

    gpio_init.GPIO_Pin   = GPIO_Pin_4;
    gpio_init.GPIO_Mode  = GPIO_Mode_OUT;
    gpio_init.GPIO_Speed = GPIO_Speed_Level_3;
    gpio_init.GPIO_OType = GPIO_OType_PP;
    gpio_init.GPIO_PuPd  = GPIO_PuPd_DOWN;

    GPIO_PinAFConfig(GPIOA, GPIO_PinSource4, GPIO_AF_2);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource4, GPIO_AF_2);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource0, GPIO_AF_2);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource1, GPIO_AF_2);

     GPIO_Init(GPIOA, &gpio_init);

    gpio_init.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_4;

    GPIO_Init(GPIOB, &gpio_init);



    gpio_init.GPIO_Pin   = GPIO_Pin_4;
    gpio_init.GPIO_Mode  = GPIO_Mode_AF;
    gpio_init.GPIO_Speed = GPIO_Speed_Level_3;
    gpio_init.GPIO_OType = GPIO_OType_PP;
    gpio_init.GPIO_PuPd  = GPIO_PuPd_DOWN;

    GPIO_PinAFConfig(GPIOA, GPIO_PinSource4, GPIO_AF_2);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource4, GPIO_AF_2);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource0, GPIO_AF_2);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource1, GPIO_AF_2);

    GPIO_Init(GPIOA, &gpio_init);

    gpio_init.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_4;

    GPIO_Init(GPIOB, &gpio_init);

    setuptimer.TIM_Prescaler = 0;                                                    //DONT KNOW WHY
    setuptimer.TIM_CounterMode = TIM_CounterMode_Up;
    setuptimer.TIM_Period = TimerPeriod - 1;
    setuptimer.TIM_ClockDivision = 0;
    setuptimer.TIM_RepetitionCounter = 0;

    TIM_TimeBaseInit(TIM3, &setuptimer);

    uint16_t Channel1Pulse = (uint16_t) (12.5 * TimerPeriod / 100);     //DUTY CYCLE OF 25 USED FOR ARMING ESC

    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse = Channel1Pulse;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
    TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset;

    TIM_OC1PreloadConfig(TIM3,TIM_OCPreload_Enable);
    TIM_OC1Init(TIM3, &TIM_OCInitStructure);

    TIM_OC2PreloadConfig(TIM3,TIM_OCPreload_Enable);
    TIM_OC2Init(TIM3, &TIM_OCInitStructure);

    TIM_OC3PreloadConfig(TIM3,TIM_OCPreload_Enable);
    TIM_OC3Init(TIM3, &TIM_OCInitStructure);

    TIM_OC4PreloadConfig(TIM3,TIM_OCPreload_Enable);
    TIM_OC4Init(TIM3, &TIM_OCInitStructure);

    TIM_Cmd(TIM3, ENABLE);

}

static void i2c_config()
{
    i2cUnstick();

   GPIO_InitTypeDef init;
   init.GPIO_Pin   = GPIO_Pin_6|GPIO_Pin_7;
   init.GPIO_Mode  =  GPIO_Mode_AF;
   init.GPIO_Speed = GPIO_Speed_Level_1;
   init.GPIO_OType = GPIO_OType_OD;
   init.GPIO_PuPd  = GPIO_PuPd_UP;

   GPIO_PinAFConfig(GPIOB,GPIO_PinSource6,GPIO_AF_4);
   GPIO_PinAFConfig(GPIOB,GPIO_PinSource7,GPIO_AF_4);

   GPIO_Init(GPIOB,&init);

   I2C_InitTypeDef i2c_init;

   i2c_init.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
   i2c_init.I2C_Mode = I2C_Mode_I2C;
   i2c_init.I2C_Ack = I2C_Ack_Disable;
   i2c_init.I2C_OwnAddress1 = 0x00;
   i2c_init.I2C_DigitalFilter = 0x00;
   i2c_init.I2C_AnalogFilter = I2C_AnalogFilter_Enable;
   i2c_init.I2C_Timing = 0x00C0216C;                        //400kHz Fast Mode 64MHz
   //i2c_init.I2C_Timing = 0x00C0125D;
   I2C_SlaveAddressConfig(I2C1, 0x68);

   I2C_Init(I2C1, &i2c_init);
   I2C_Cmd(I2C1, ENABLE);

   int add = check_imu_connect();

   if(add == 0x71)
   imu_config();

   else
    SCB->AIRCR = 0x05fa0004;        //SYSTEM RESET   https://os.mbed.com/questions/67953/How-can-I-RESET-this-from-my-program/
                                    //https://www.st.com/content/ccc/resource/technical/document/programming_manual/6c/3a/cb/e7/e4/ea/44/9b/DM00046982.pdf/files/DM00046982.pdf/jcr:content/translations/en.DM00046982.pdf


}


static void interrupt_config()
{

  NVIC_InitTypeDef  NVIC_InitStructure;
  GPIO_InitTypeDef gpio_init;
  EXTI_InitTypeDef exti_init;

  gpio_init.GPIO_Pin   = GPIO_Pin_0;
  gpio_init.GPIO_Mode  = GPIO_Mode_IN;
  gpio_init.GPIO_OType = GPIO_OType_PP;
  gpio_init.GPIO_PuPd  = GPIO_PuPd_NOPULL;

  GPIO_Init(GPIOA, &gpio_init);

  SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA,EXTI_PinSource0);

  exti_init.EXTI_Line    = EXTI_Line0;
  exti_init.EXTI_Mode    = EXTI_Mode_Interrupt;
  exti_init.EXTI_Trigger = EXTI_Trigger_Rising;
  exti_init.EXTI_LineCmd = ENABLE;

  EXTI_Init(&exti_init);

  NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel6_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel7_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_Init(&NVIC_InitStructure);

}

static void dma_init()
{
  DMA_InitTypeDef DMA_InitStruct;

//////////////////////////////////////////////////////////////////////////
//USART2 DMA1_Channel6
/////////////////////////////////////////////////////////////////////////

    DMA_InitStruct.DMA_MemoryBaseAddr = (uint32_t)buff_data;
    DMA_InitStruct.DMA_BufferSize = BUFF_SIZE;
    DMA_InitStruct.DMA_PeripheralBaseAddr = (uint32_t)&USART2->RDR;
    DMA_InitStruct.DMA_DIR = DMA_DIR_PeripheralSRC;
    DMA_InitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
    DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStruct.DMA_Mode =  DMA_Mode_Circular;
    DMA_InitStruct.DMA_Priority = DMA_Priority_Medium;
    DMA_InitStruct.DMA_M2M = DMA_M2M_Disable;
    DMA_Init(DMA1_Channel6, &DMA_InitStruct);

    DMA_ITConfig(DMA1_Channel6, DMA_IT_TC, ENABLE);


//////////////////////////////////////////////////////////////////////////
//I2C1 DMA1_Channel7
/////////////////////////////////////////////////////////////////////////

    DMA_InitStruct.DMA_PeripheralBaseAddr = (uint32_t)&I2C1->RXDR; // address of data reading register of I2C1
    DMA_InitStruct.DMA_MemoryBaseAddr = (uint32_t)I2C_Buffer; //variable to store data
    DMA_InitStruct.DMA_M2M = DMA_M2M_Disable; //channel will be used for peripheral to memory transfer
    DMA_InitStruct.DMA_Mode = DMA_Mode_Normal;    //setting normal mode (non circular)
    DMA_InitStruct.DMA_Priority = DMA_Priority_Medium;    //medium priority
    DMA_InitStruct.DMA_DIR = DMA_DIR_PeripheralSRC;    //Location assigned to peripheral register will be source
    DMA_InitStruct.DMA_BufferSize = 14;    //number of data to be transfered
    DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Disable; //automatic memory increment disable for peripheral
    DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;    //automatic memory increment enable for memory
    DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;    //source peripheral data size = 8bit
    DMA_InitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;    //destination memory data size = 8bit
    DMA_Init(DMA1_Channel7, &DMA_InitStruct);

    DMA_ITConfig(DMA1_Channel7, DMA_IT_TC, ENABLE);

}
