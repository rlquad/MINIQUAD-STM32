#include "q_angle.h"

//const float gyro_lpf = 1/(16.4);
//const double degconvert = 57.2957795131;
//const double invdeg = 1/degconvert;
//const float accelw = 0.004;
const float gyrow  = 0.999;
const float accelw = 0.001;


double q0 = 1,q1 = 0,q2 = 0,q3 = 0;

void ang_update(float gx,float gy,float gz,float ax,float ay,float az,int dt)
{
    double qdot0,qdot1,qdot2,qdot3;
    double _q0,_q1,_q2,_q3;

    double _ax,_ay,_az;

  //  print_ang(2,ax,ay,az);
    double norm = 1/sqrt(ax*ax + ay*ay + az*az);

    ax *= norm;
    ay *= norm;
    az *= norm;

    //print_ang(2,norm,az,0);

    qdot0 = 0.5f * (-q1 * gx - q2 * gy - q3 * gz);
    qdot1 = 0.5f * (q0 * gx + q2 * gz - q3 * gy);
    qdot2 = 0.5f * (q0 * gy - q1 * gz + q3 * gx);
    qdot3 = 0.5f * (q0 * gz + q1 * gy - q2 * gx);

    q0 += qdot0*dt/1000000;
    q1 += qdot1*dt/1000000;
    q2 += qdot2*dt/1000000;
    q3 += qdot3*dt/1000000;

    double _2q0q2 = 2.0f * q0 * q2;
    double _2q1q3 = 2.0f * q1 * q3;
    double _2q1q2 = 2.0f * q1 * q2;
    double _2q0q3 = 2.0f * q0 * q3;
    double _2q2q3 = 2.0f * q2 * q3;
    double _2q0q1 = 2.0f * q0 * q1;
    double q0q0 = q0 * q0;
    double q1q1 = q1 * q1;
    double q2q2 = q2 * q2;
    double q3q3 = q3 * q3;


    //print_ang(2,q0q0,q0q1);

    _ax = (q0q0 + q1q1 - q2q2 - q3q3)*ax + (_2q1q2 - _2q0q3)*ay + (_2q1q3 + _2q0q2)*az;
    _ay = (_2q1q2 + _2q0q3)*ax + (q0q0 - q1q1 + q2q2 - q3q3)*ay + (_2q2q3 - _2q0q1)*az;
    _az = (_2q1q3 - _2q0q2)*ax + (_2q2q3 + _2q0q1)*ay + (q0q0 - q1q1 - q2q2 + q3q3)*az;

    // print_ang(2,_ax,_ay,_az);

    _q0 = sqrt((_az + 1)/2);
    _q1 = -_ay/sqrt(2*(_az+1));
    _q2 = _ax/sqrt(2*(_az+1));
    _q3 = 0;


  //print_ang(2,_q0,_q1,_q2);
    _q0 = gyrow + accelw*_q0;
    _q1 = accelw*_q1;
    _q2 = accelw*_q2;
    _q3 = accelw*_q3;

    norm = 1/sqrt(_q0*_q0 + _q1*_q1 + _q2*_q2 + _q3*_q3);

    _q0 *= norm;
    _q1 *= norm;
    _q2 *= norm;

 //print_ang(2,_q0,_q1,_q2);

double tmp0,tmp1,tmp2,tmp3;

    tmp0 = q0;
    tmp1 = -q1;
    tmp2 = -q2;
    tmp3 = -q3;
//print_ang(2,q0,q1,q2);

    q0 = ((_q0 * tmp0) - (_q1 * tmp1) - (_q2 * tmp2));
    q1 = -((tmp0 * _q1) + (tmp1 * _q0) - (tmp3 * _q2));
    q2 = -((tmp0 * _q2) + (tmp2 * _q0) + (tmp3 * _q1));
    q3 = -((tmp1 * _q2) - (tmp2 * _q1) + (tmp3 * _q0));


    norm = 1/sqrt(q0*q0 + q1*q1 + q2*q2 + q3*q3);

    q0 *= norm;
    q1 *= norm;
    q2 *= norm;
    q3 *= norm;

}


float invSqrt(float x) {

		/* close-to-optimal  method with low cost from http://pizer.wordpress.com/2008/10/12/fast-inverse-square-root */
		unsigned int i = 0x5F1F1412 - (*(unsigned int*)&x >> 1);
		float tmp = *(float*)&i;
		return tmp * (1.69000231f - 0.714158168f * x * tmp * tmp);

}

