#include "angle.h"
#include <math.h>

const int acc_lsb      =  16384;                                                         //CONVERSION FACTOR FOR ACCELERATION OUTPUT
const float gyro_lsb   =  16.4 ;                                                         //CONVERSION FACTOR FOR GYROSCOPE OUTPUT
const float degconvert =  57.2957795;


const float gyrow = 0.996;
const float accelw = 0.004;

double roll,pitch,yaw;
uint8_t I2C_Buffer[14];
int count = 0;

uint8_t mpu_calibrated = 0;

void angle_calc(int dt)
{

    roll  = accelw*(atan2(acc[1],acc[2])*degconvert)  + gyrow*(roll  + (gyro[0] - gyro_cal[0])*dt/(1000000.0*gyro_lsb));
    pitch = accelw*(atan2(-acc[0],acc[2])*degconvert) + gyrow*(pitch + (gyro[1] - gyro_cal[1])*dt/(1000000.0*gyro_lsb));
    yaw   = yaw + (gyro[2] - gyro_cal[2])*dt/(1000000.0*gyro_lsb);

    motor_update();

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
      //Skip byte 3 of temperature data
    //Read Gyro data from byte 4 to byte 6
    for(int i=4; i<7; i++)
      gyro[i-4]=((s16)((u16)I2C_Buffer[2*i] << 8) + I2C_Buffer[2*i+1]);
    }

  clear_int();

  if(mpu_calibrated)
  angle_calc(1000);

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
