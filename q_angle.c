#include "q_angle.h"

const float gyro_lpf = 1/(16.4);
const double degconvert = 57.2957795131;
const double invdeg = 1/degconvert;

float q0 = 1,q1 = 0,q2 = 0,q3 = 0;

void ang_update(uint16_t ax,uint16_t ay,uint16_t az,uint16_t gx,uint16_t gy,uint16_t gz,int dt)
{
    float qdot0,qdot1,qdot2,qdot3;
    float _q0,_q1,_q2,_q3;
    
    float _ax,_ay,_az;
    
    float norm = invsqrt(ax*ax + ay*ay + az*az);
    
    ax *= norm;
    ay *= norm;
    az *= norm;
    
    qDot0 = 0.5f * (-q1 * gx - q2 * gy - q3 * gz);
    qDot1 = 0.5f * (q0 * gx + q2 * gz - q3 * gy);
    qDot2 = 0.5f * (q0 * gy - q1 * gz + q3 * gx);
    qDot3 = 0.5f * (q0 * gz + q1 * gy - q2 * gx);
    
    q0 += qdot0*dt;
    q1 += qdot1*dt;
    q2 += qdot2*dt;
    q3 += qdot3*dt;
    

    float _2q0 = 2.0f * q0;
    float _2q1 = 2.0f * q1;
    float _2q2 = 2.0f * q2;
    float _2q3 = 2.0f * q3;
    float _2q0q2 = 2.0f * q0 * q2;
    float _2q1q3 = 2.0f * q1 * q3;
    float _2q1q2 = 2.0f * q1 * q2;
    float _2q0q3 = 2.0f * q0 * q3;
    float _2q2q3 = 2.0f * q2 * q3;
    float _2q0q1 = 2.0f * q0 * q1;
    float q0q0 = q0 * q0;
    float q0q1 = q0 * q1;
    float q0q2 = q0 * q2;
    float q0q3 = q0 * q3;
    float q1q1 = q1 * q1;
    float q1q2 = q1 * q2;
    float q1q3 = q1 * q3;
    float q2q2 = q2 * q2;
    float q2q3 = q2 * q3;
    float q3q3 = q3 * q3;
    
    _ax = (q0q0 + q1q1 - q2q2 - q3q3)*ax + (_2q1q2 - _2q0q3)*ay + (_2q1q3 + _2q0q2)*az;
    _ay = (_2q1q2 + _2q0q3)*ax + (q0q0 - q1q1 + q2q2 - q3q3)*ay + (_2q2q3 - _2q0q1)*az;
    _az = (_2q1q3 - _2q0q2)*ax + (_2q2q3 + _2q2q3)*ay + (q0q0 - q1q1 - q2q2 + q3q3)*az;
    
    _q0 = sqrt((_az + 1)/2);
    _q1 = -ay/sqrt(2*(az+1));
    _q2 = ax/sqrt(2*(az+1));
    _q3 = 0;
    
    q0 = 0.5f * (q0 * _q0 - q1 * _q1 - q2 * _q2);
    q1 = 0.5f * (q0 * _q1 + q1 * _q0 - q3 * _q2);
    q2 = 0.5f * (q0 * _q2 + q2 * _q0 + q3 * _q1);
    q3 = 0.5f * (q3 * _q0 + q1 * _q2 - q2 * _q1);
 
}
