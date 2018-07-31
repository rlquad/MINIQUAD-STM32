#include "dcm.h"

const float accelw = 0.004;
const float grow   = 0.996;
const float gyro_factor = 1/(57.2957795 * 16.4 * 1000000);
const float degconvert = 57.2957795;
const float invdeg = 1/(57.2957795);

double roll,pitch,yaw;
double dtheta[3] = {0,0,0};
double dcm_mat[3][3] = { 1, 0, 0,
                         0, 1, 0,
                         0, 0, 1 };

void angle_calc(int dt)
{
    dtheta[ROLL]  = gyrow*gyro[0]*dt*gyro_factor + accelw*(atan2( acc[1],acc[2])*degconvert - roll*invdeg);
    dtheta[PITCH] = gyrow*gyro[0]*dt*gyro_factor + accelw*(atan2(-acc[0],acc[2])*degconvert - pitch*invdeg);
    dtheta[YAW]   = gyrow*gyro[0]*dt*gyro_factor;



}

void rotate()
{
    double rot_mat[3][3] = {  };



}
