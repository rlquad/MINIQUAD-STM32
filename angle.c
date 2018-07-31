#include "angle.h"
#include <math.h>

const int acc_lsb      =  16384;                                                         //CONVERSION FACTOR FOR ACCELERATION OUTPUT
const float gyro_lsb   =  16.4 ;                                                         //CONVERSION FACTOR FOR GYROSCOPE OUTPUT
const float degconvert =  57.2957795;
const int acc_smt  = 1;
//const int gyro_smt = 10;

//const uint16_t acc_bias[3] = {-1792,-512,256};
//const uint16_t acc_bias[3] = {-1775,-545,102};
const uint16_t acc_bias[3] = {0,0,0};

double roll,pitch,yaw;
uint8_t I2C_Buffer[14];
int count = 0;
int angle_state = 0;    //0 - NOT UPDATED, 1 - UPDATED, 2 - TIMEOUT
int dt;
float temp;

uint32_t timer;
int32_t last_gyro[3] = {0,0,0};
uint8_t mpu_calibrated = 0;


void angle_calc()
{
    angle_state = 1;

    while((TIM_GetCounter(TIM2)-timer)<1550);
     dt = TIM_GetCounter(TIM2) - timer;

    timer = TIM_GetCounter(TIM2);
    if(dt<0)
        return;

 //print_ang(2,roll,pitch,dt);
   // ang_update((gyro[0])/(degconvert*16.4),(gyro[1])/(degconvert*16.4),(gyro[2])/(degconvert*16.4),acc[0],acc[1],acc[2],dt);

   roll  = 0.01*(atan2( acc[1],acc[2])*degconvert) + 0.99*(roll  + (gyro[0])*dt/(1000000.0*gyro_lsb));

    pitch = 0.01*(atan2(-acc[0],acc[2])*degconvert) + 0.99*(pitch + (gyro[1])*dt/(1000000.0*gyro_lsb));
    yaw   = yaw + (gyro[2] - gyro_cal[2])*dt/(1000000.0*gyro_lsb);
/*

    uint32_t t = TIM_GetCounter(TIM2);
    for(int i=0;i<3;i++)
    MadgwickAHRSupdateIMU((gyro[0]-gyro_cal[0])/(degconvert*16.4),(gyro[1]-gyro_cal[1])/(degconvert*16.4),(gyro[2]-gyro_cal[2])/(degconvert*16.4),acc[0],acc[1],acc[2]);

/*


    roll  = atan2(2*((q0*q1) + (q2*q3)),1.0 - 2.0 * (q1 * q1 + q2 * q2))*degconvert;
    pitch = asin(2*(q0*q2 - q1*q3))*degconvert;
    yaw   = atan2(2*((q0*q3) + (q2*q1)),1.0 - 2.0 * (q3 * q3 + q2 * q2))*degconvert;
*/
   // print_ang(2,acc[1],acc[0],pitch,roll);


}


void DMA1_Channel7_IRQHandler(void)
{
   if (DMA_GetFlagStatus(DMA1_FLAG_TC7))
    {
    /* Clear transmission complete flag */
    DMA_ClearFlag(DMA1_FLAG_TC7);

    I2C_DMACmd(I2C1,I2C_DMAReq_Rx, DISABLE);
    /* Send I2C1 STOP Condition */
    I2C_GenerateSTOP(I2C1, ENABLE);
    while(I2C1->CR2 & I2C_CR2_STOP);                                                  //ENSURE STOP CONDITION IS ENFORCED BEFORE PROCEEDING

    /* Disable DMA channel*/
    DMA_Cmd(DMA1_Channel7, DISABLE);

    //Read Accel data from byte 0 to byte 2
    for(int i=0; i<3; i++)
     acc[i]=((s16)((u16)I2C_Buffer[2*i] << 8) + I2C_Buffer[2*i+1]);

     int tmp;
    tmp = ((s16)((u16)I2C_Buffer[6] << 8) + I2C_Buffer[7]);
    temp = tmp/340 + 36.35;
    //Read Gyro data from byte 4 to byte 6
    for(int i=4; i<7; i++)
      gyro[i-4]=((s16)((u16)I2C_Buffer[2*i] << 8) + I2C_Buffer[2*i+1]);
    }

    acc[0] = ((acc[0])>>acc_smt)<<acc_smt;
    acc[1] = ((acc[1])>>acc_smt)<<acc_smt;
    acc[2] = ((acc[2])>>acc_smt)<<acc_smt;

  clear_int();

  if(mpu_calibrated)
  {
      acc[0] = ((acc[0] + acc_bias[0]));
      acc[1] = ((acc[1] + acc_bias[1]));
      acc[2] = ((acc[2] + acc_bias[2]));

      gyro[0] = ((gyro[0] - gyro_cal[0]));
      gyro[1] = ((gyro[1] - gyro_cal[1]));
      gyro[2] = ((gyro[2] - gyro_cal[2]));

  /*    gyro[0] = ((gyro[0])/gyro_smt)*gyro_smt;
      gyro[1] = ((gyro[1])/gyro_smt)*gyro_smt;
      gyro[2] = ((gyro[2])/gyro_smt)*gyro_smt;

     if((gyro[0]-last_gyro[0])>10||(gyro[0]-last_gyro[0])<-10)
        {
            last_gyro[0] = gyro[0];
        }
     else
        {
            gyro[0] = last_gyro[0];
        }
     if(gyro[1]-last_gyro[1]>10||gyro[1]-last_gyro[1]<-10)
        {
            last_gyro[1] = gyro[1];
        }
        else
        {
            gyro[1] = last_gyro[1];
        }
      if(gyro[2]-last_gyro[2]>10||gyro[2]-last_gyro[2]<-10)
        {
           last_gyro[2] = gyro[2];
        }
        else
        {
            gyro[2] = last_gyro[2];
        }

    if(gyro[0]<10&&gyro[0]>-10)
        {
            gyro[0]=0;
        }
     if(gyro[1]<10&&gyro[1]>-10)
        {
            gyro[1]=0;
        }

      if(gyro[2]<10&&gyro[2]>-10)
        {
            gyro[2]=0;
        }
*/
      angle_calc();
  }
  else
    calibrate();

}

void I2C_DMA_Read()
  {
  /* Disable DMA channel*/
  DMA_Cmd(DMA1_Channel7, DISABLE);
  /* Set current data number again to 14 for MPu6050, only possible after disabling the DMA channel */
  DMA_SetCurrDataCounter(DMA1_Channel7, 14);

  /* While the bus is busy */
  while(I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY)==SET);

  wrt_reg(0x3B,1);

  I2C_MasterRequestConfig(I2C1, I2C_Direction_Receiver);                            //DEFINING DIRECTION OF DATA TRANSMISSION

  I2C_NumberOfBytesConfig(I2C1, 14);                                                //NUMBER OF BYTES TO RECEIVE FROM SLAVE

  I2C_GenerateSTART(I2C1, ENABLE);
  while(I2C1->CR2 & I2C_CR2_START);

  /* Start DMA to receive data from I2C */
  DMA_Cmd(DMA1_Channel7, ENABLE);
  I2C_DMACmd(I2C1,I2C_DMAReq_Rx, ENABLE);

}


void EXTI0_IRQHandler(void)
{
   if(EXTI->PR & EXTI_PR_PR0)
  { EXTI->PR |= EXTI_PR_PR0;
      I2C_DMA_Read();
  }
}

void calibrate()
{
    count++;
    if(count >= Calibration_Value)
       {
           mpu_calibrated = 1;
           led_off();

           gyro_cal[0]/=Calibration_Value;
           gyro_cal[1]/=Calibration_Value;
           gyro_cal[2]/=Calibration_Value;

       }

    else
    {
        gyro_cal[0] += gyro[0];
        gyro_cal[1] += gyro[1];
        gyro_cal[2] += gyro[2];


    }

}
