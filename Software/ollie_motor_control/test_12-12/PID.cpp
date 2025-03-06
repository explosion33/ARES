#include "PID.h"


PID::PID(float Kp, float Ki, float Kd) {
    errorLast = 0;
    integralError = 0;
    this->Kp = Kp;
    this->Ki = Ki;
    this->Kd = Kd;
}

float PID::compute(float currAngle, float targetAngle, float dt) {
    float theta = targetAngle - currAngle;
    float error = theta;
    integralError += error*dt; // todo: solve integral windup later
    
    float P = -Kp * error; //Siply proportional to error
    float I = -Ki * integralError;
    float D = -Kd * (error - errorLast) / dt;
    errorLast = error;

    return P + I + D;
}