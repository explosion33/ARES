// This is a pid class to control the motors for ares
// it has a method which will output a float for the motor power given the angle error 
// (or target and curr angle) and the time step
// it will be initialized with constants

#ifndef _PID_H_
#define _PID_H_

class PID {
    public: 
        PID(float Kp, float Ki, float Kd);
        float compute(float currAngle, float targetAngle, float dt);

    private:
        float Kp;
        float Ki;
        float Kd;
        float setpoint; 
        float errorLast;
        float integralError;
};


#endif //_PID_H_