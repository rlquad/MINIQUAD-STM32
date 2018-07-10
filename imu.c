#include "imu.h"

const int gyro_dlpf   = 0x1A;                                                          //DIGITAL LOW PASS FILTER
const int gyro_config = 0x1B;
const int acc_config  = 0x1C;
const int WHO_AM_I    = 0x75;                                                         //ADDRESS OF MPU6050 IS STORED IN THIS REGISTER
const int PWR_MGMT    = 0x6B;
const int MPU_ADD     = 0x68;                                                          //ADDRESS OF MPU
const int obt_orient  = 0x3B;
const int int_status  = 0x3A;
const int int_enable  = 0x38;
const int int_setup   = 0x37;
const int accl_dlpf   = 0x1D;
const int mag_add     = 0x0C;
const int TIMEOUT     = 1000;

const int acc_sens  = 0x0;
const int gyro_sens = 0x3;

int16_t gyro[3],acc[3];
int16_t gyro_cal[3];
uint32_t timein;

int check_imu_connect()
{
    wrt_reg(WHO_AM_I,1);

  I2C_GenerateSTOP(I2C1,ENABLE);                                                    //GENERATE STOPPING CONDITION
  timein = TIM_GetCounter(TIM2);
  while(I2C1->CR2 & I2C_CR2_STOP)                                                  //ENSURE STOP CONDITION IS ENFORCED BEFORE PROCEEDING
  {
    if((TIM_GetCounter(TIM2)-timein)>TIMEOUT)
      {
          angle_state = 2;
           break;
      }
  }


    return recv_byte();

}



void wrt_reg(int reg,int bytes)
{
  I2C_MasterRequestConfig(I2C1, I2C_Direction_Transmitter);                         //DEFINING DIRECTION OF DATA TRANSMISSION

  I2C_NumberOfBytesConfig(I2C1, bytes);                                             //NUMBER OF BYTES TO BE TRANSMITTED

  I2C_GenerateSTART(I2C1,ENABLE);                                                   //GENERATE START CONDITION
  timein = TIM_GetCounter(TIM2);
  while(I2C1->CR2 & I2C_CR2_START)     //TO ENSURE START CONDITION HAS BEEN FULFILLED
   {
     if((TIM_GetCounter(TIM2)-timein)>TIMEOUT)
      {
          angle_state = 2;
           break;
      }
   }

  I2C_SendData(I2C1, reg);                                                         //WRITE DATA TO A SPECIFIC REGISTER
  timein = TIM_GetCounter(TIM2);
  while(!(I2C1->ISR & I2C_ISR_TXE))                                              //TO ENSURE DATA HAS BEEN WRITTEN TO TXE BIT
   {
     if((TIM_GetCounter(TIM2)-timein)>TIMEOUT)
       {
          angle_state = 2;
           break;
       }
   }

}

void wrt_reg_data(int reg,int data,int bytes)
{
  I2C_MasterRequestConfig(I2C1, I2C_Direction_Transmitter);                         //DEFINING DIRECTION OF DATA TRANSMISSION

  I2C_NumberOfBytesConfig(I2C1, bytes);                                             //NUMBER OF BYTES TO BE TRANSMITTED

  I2C_GenerateSTART(I2C1,ENABLE);                                                   //GENERATE START CONDITION
  timein = TIM_GetCounter(TIM2);
  while(I2C1->CR2 & I2C_CR2_START)                                                   //TO ENSURE START CONDITION HAS BEEN FULFILLED
   {
     if((TIM_GetCounter(TIM2)-timein)>TIMEOUT)
      {
          angle_state = 2;
           break;
      }
   }                                                 //TO ENSURE START CONDITION HAS BEEN FULFILLED


  I2C_SendData(I2C1, reg);                                                         //WRITE DATA TO A SPECIFIC REGISTER
  timein = TIM_GetCounter(TIM2);
  while(!(I2C1->ISR & I2C_ISR_TXE))                                                //TO ENSURE DATA HAS BEEN WRITTEN TO TXE BIT
   {
     if((TIM_GetCounter(TIM2)-timein)>TIMEOUT)
       {
          angle_state = 2;
           break;
      }
   }

  I2C_SendData(I2C1, data);                                                         //WRITE DATA TO A SPECIFIC REGISTER
  timein = TIM_GetCounter(TIM2);
  while(!(I2C1->ISR & I2C_ISR_TXE))                                               //TO ENSURE DATA HAS BEEN WRITTEN TO TXE BIT
   {
     if((TIM_GetCounter(TIM2)-timein)>TIMEOUT)
       {
          angle_state = 2;
           break;
      }
   }

}

int recv_byte()
{
  I2C_MasterRequestConfig(I2C1, I2C_Direction_Receiver);                            //DEFINING DIRECTION OF DATA TRANSMISSION

  I2C_NumberOfBytesConfig(I2C1, 1);                                               //NUMBER OF BYTES TO RECEIVE FROM SLAVE

  I2C_GenerateSTART(I2C1,ENABLE);                                                   //GENERATE START CONDITION
  timein = TIM_GetCounter(TIM2);
  while(I2C1->CR2 & I2C_CR2_START)                                                   //TO ENSURE START CONDITION HAS BEEN FULFILLED
   {
     if((TIM_GetCounter(TIM2)-timein)>TIMEOUT)
      {
          angle_state = 2;
          break;
      }
   }

  timein = TIM_GetCounter(TIM2);
  while(!(I2C1->ISR & I2C_ISR_RXNE))                                              //CHECKING IF DATA IS RECEIVED
  {
    if((TIM_GetCounter(TIM2)-timein)>TIMEOUT)
      {
          angle_state = 2;
          break;
      }
  }
   int data = I2C_ReadRegister(I2C1,I2C_Register_RXDR);

  I2C_GenerateSTOP(I2C1,ENABLE);                                                    //GENERATE STOPPING CONDITION
  timein = TIM_GetCounter(TIM2);
  while(I2C1->CR2 & I2C_CR2_STOP)                                                  //ENSURE STOP CONDITION IS ENFORCED BEFORE PROCEEDING
  {
    if((TIM_GetCounter(TIM2)-timein)>TIMEOUT)
      {
          angle_state = 2;
          break;
      }
  }

return data;

}


void imu_config()
{

  wrt_reg_data(PWR_MGMT,0,2);
  I2C_GenerateSTOP(I2C1,ENABLE);                                                    //GENERATE STOPPING CONDITION
  while(I2C1->CR2 & I2C_CR2_STOP);

  wrt_reg_data(gyro_config,0x18,2);
  I2C_GenerateSTOP(I2C1,ENABLE);                                                    //GENERATE STOPPING CONDITION
  while(I2C1->CR2 & I2C_CR2_STOP);

  wrt_reg_data(acc_config,0x00,2);
  I2C_GenerateSTOP(I2C1,ENABLE);                                                    //GENERATE STOPPING CONDITION
  while(I2C1->CR2 & I2C_CR2_STOP);

  wrt_reg_data(gyro_dlpf,0x01,2);
  I2C_GenerateSTOP(I2C1,ENABLE);                                                    //GENERATE STOPPING CONDITION
  while(I2C1->CR2 & I2C_CR2_STOP);

  wrt_reg_data(accl_dlpf,0x06,2);
  I2C_GenerateSTOP(I2C1,ENABLE);                                                    //GENERATE STOPPING CONDITION
  while(I2C1->CR2 & I2C_CR2_STOP);

  wrt_reg_data(0x37,0x20,2);
  I2C_GenerateSTOP(I2C1,ENABLE);                                                    //GENERATE STOPPING CONDITION
  while(I2C1->CR2 & I2C_CR2_STOP);


  wrt_reg_data(0x38,0x01,2);
  I2C_GenerateSTOP(I2C1,ENABLE);                                                    //GENERATE STOPPING CONDITION
  while(I2C1->CR2 & I2C_CR2_STOP);

  wrt_reg_data(MPU_ADD,0x07,2);                    //RESET ALL REGISTERS
  I2C_GenerateSTOP(I2C1,ENABLE);                                                    //GENERATE STOPPING CONDITION
  while(I2C1->CR2 & I2C_CR2_STOP);

  clear_int();
}

void clear_int()
{
   wrt_reg(0x3A,1);

  I2C_GenerateSTOP(I2C1,ENABLE);                                                    //GENERATE STOPPING CONDITION
  timein = TIM_GetCounter(TIM2);
  while(I2C1->CR2 & I2C_CR2_STOP)                                                  //ENSURE STOP CONDITION IS ENFORCED BEFORE PROCEEDING
  {
    if((TIM_GetCounter(TIM2)-timein)>TIMEOUT)
      {
          angle_state = 2;
          break;
      }
  }
  recv_byte();

}

void i2cUnstick()    //https://www.microchip.com/forums/m263593.aspx
{
    GPIO_InitTypeDef GPIO_InitStructure;

    uint8_t i;

    ///////////////////////////////////
        GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_7;
        GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
        GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;

        GPIO_Init(GPIOB, &GPIO_InitStructure);

        if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_7)==1)
            return;

	    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_6;
        GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
        GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;

        GPIO_Init(GPIOB, &GPIO_InitStructure);

        GPIO_SetBits(GPIOB,GPIO_Pin_6);
        for(int i=0;i<9;i++);

        for (i = 0; i < 8; i++)
        {
          //  while (!GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_6))  // Wait for any clock stretching to finish
                                       // 2.5 would be 400 kHz, 3 is 333.33333 kHz
            // Pull low
            GPIO_ResetBits(GPIOB, GPIO_Pin_6);                 // Set bus low
            for(int i=0;i<9;i++);                                    // 2.5 would be 400 kHz, 3 is 333.33333 kHz
            // Release high again
            GPIO_SetBits(GPIOB, GPIO_Pin_6);                   // Set bus high
            for(int i=0;i<9;i++);                                   // 2.5 would be 400 kHz, 3 is 333.33333 kHz
        }

        GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_7;
        GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
        GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;

        GPIO_Init(GPIOB, &GPIO_InitStructure);


        GPIO_ResetBits(GPIOB, GPIO_Pin_7);                     // Set bus data low
        for(int i=0;i<9;i++);                                        // 2.5 would be 400 kHz, 3 is 333.33333 kHz


        GPIO_ResetBits(GPIOB, GPIO_Pin_6);                     // Set bus scl low
        for(int i=0;i<9;i++);                                        // 2.5 would be 400 kHz, 3 is 333.33333 kHz
        GPIO_SetBits(GPIOB, GPIO_Pin_6);                       // Set bus scl high
        for(int i=0;i<9;i++);                                        // 2.5 would be 400 kHz, 3 is 333.33333 kHz
        GPIO_SetBits(GPIOB, GPIO_Pin_7);                       // Set bus sda high
        for(int i=0;i<9;i++);                                        // 2.5 would be 400 kHz, 3 is 333.33333 kHz
                                        // 2.5 would be 400 kHz, 3 is 333.33333 kHz


}



void mpu_reset()
{


}
