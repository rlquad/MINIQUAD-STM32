#include "imu.h"

const int dlpf_reg    = 0x1A;                                                          //DIGITAL LOW PASS FILTER
const int gyro_config = 0x1B;
const int acc_config  = 0x1C;
const int WHO_AM_I    = 0x75;                                                         //ADDRESS OF MPU6050 IS STORED IN THIS REGISTER
const int PWR_MGMT    = 0x6B;
const int MPU_ADD     = 0x68;                                                          //ADDRESS OF MPU
const int obt_orient  = 0x3B;
const int int_status  = 0x3A;
const int int_enable  = 0x38;
const int int_setup   = 0x37;

const int acc_sens  = 0x0;
const int gyro_sens = 0x3;

const int acc_smt = 8;


int16_t gyro[3],acc[3];
int16_t gyro_cal[3];

static int check_imu_connect()
{
    wrt_reg(WHO_AM_I,1);
    return recv_byte();

}



void wrt_reg(int reg,int bytes)
{
  I2C_MasterRequestConfig(I2C1, I2C_Direction_Transmitter);                         //DEFINING DIRECTION OF DATA TRANSMISSION

  I2C_NumberOfBytesConfig(I2C1, bytes);                                             //NUMBER OF BYTES TO BE TRANSMITTED

  I2C_GenerateSTART(I2C1,ENABLE);                                                   //GENERATE START CONDITION
  while((I2C1->CR2 & I2C_CR2_START));

  I2C_SendData(I2C1, reg);                                                         //WRITE DATA TO A SPECIFIC REGISTER
  while(!(I2C1->ISR & I2C_ISR_TXE));


}

void wrt_reg_data(int reg,int data,int bytes)
{
  I2C_MasterRequestConfig(I2C1, I2C_Direction_Transmitter);                         //DEFINING DIRECTION OF DATA TRANSMISSION

  I2C_NumberOfBytesConfig(I2C1, bytes);                                             //NUMBER OF BYTES TO BE TRANSMITTED

  I2C_GenerateSTART(I2C1,ENABLE);                                                   //GENERATE START CONDITION
  while(I2C1->CR2 & I2C_CR2_START);                                                 //TO ENSURE START CONDITION HAS BEEN FULFILLED


  I2C_SendData(I2C1, reg);                                                          //WRITE DATA TO A SPECIFIC REGISTER
  while(!(I2C1->ISR & I2C_ISR_TXE));                                                //TO ENSURE DATA HAS BEEN WRITTEN TO TXE BIT

  I2C_SendData(I2C1, data);                                                          //WRITE DATA TO A SPECIFIC REGISTER
  while(!(I2C1->ISR & I2C_ISR_TXE));                                                //TO ENSURE DATA HAS BEEN WRITTEN TO TXE BIT
}

int recv_byte()
{
  I2C_MasterRequestConfig(I2C1, I2C_Direction_Receiver);                            //DEFINING DIRECTION OF DATA TRANSMISSION

  I2C_NumberOfBytesConfig(I2C1, 1);                                               //NUMBER OF BYTES TO RECEIVE FROM SLAVE

  I2C_GenerateSTART(I2C1,ENABLE);                                                    //GENERATE START CONDITION
  while(I2C1->CR2 & I2C_CR2_START);                                                 //TO ENSURE START CONDITION HAS BEEN FULFILLED

  while(!(I2C1->ISR & I2C_ISR_RXNE));                                               //CHECKING IF DATA IS RECEIVED
  int data = I2C_ReadRegister(I2C1,I2C_Register_RXDR);

  I2C_GenerateSTOP(I2C1,ENABLE);                                                    //GENERATE STOPPING CONDITION
  while(I2C1->CR2 & I2C_CR2_STOP);                                                  //ENSURE STOP CONDITION IS ENFORCED BEFORE PROCEEDING

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

  wrt_reg_data(dlpf_reg,0x01,2);
  I2C_GenerateSTOP(I2C1,ENABLE);                                                    //GENERATE STOPPING CONDITION
  while(I2C1->CR2 & I2C_CR2_STOP);

  wrt_reg_data(0x37,0x20,2);
  I2C_GenerateSTOP(I2C1,ENABLE);                                                    //GENERATE STOPPING CONDITION
  while(I2C1->CR2 & I2C_CR2_STOP);


  wrt_reg_data(0x38,0x01,2);
  I2C_GenerateSTOP(I2C1,ENABLE);                                                    //GENERATE STOPPING CONDITION
  while(I2C1->CR2 & I2C_CR2_STOP);

  wrt_reg_data(0x68,0x07,2);
  I2C_GenerateSTOP(I2C1,ENABLE);                                                    //GENERATE STOPPING CONDITION
  while(I2C1->CR2 & I2C_CR2_STOP);

  clear_int();
}

void clear_int()
{
   wrt_reg(0x3A,1);
    I2C_GenerateSTOP(I2C1,ENABLE);                                                    //GENERATE STOPPING CONDITION
  while(I2C1->CR2 & I2C_CR2_STOP);

  recv_byte();

}

void mpu_reset()
{


}
