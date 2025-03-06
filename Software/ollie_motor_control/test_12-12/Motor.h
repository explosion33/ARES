// Class which controls DC motor with ethan's mosftet motor board
#ifndef _MOTOR_H_
#define _MOTOR_H_


#include "mbed.h"

class Motor {
    private:
        // Defining constants from arduino
        const int LOW = 0;
        const int HIGH = 1;

        DigitalOut motorPin1;
        DigitalOut motorPin3;
        PwmOut motorPin2;
        PwmOut motorPin4;
        InterruptIn encoderA;
        InterruptIn encoderB;

        // Encoder state boolean
        bool aUp = false;
        bool bUp = false;

        volatile int encoderCounter = 0; // Counter for the encoder ticks

        const int countsPerRev = 64;
        const float gearRatio = 50; // Might need to be updated for different motors

        const float totalCounts = countsPerRev * gearRatio; // Total counts per rotation of motor output shaft
        long position;
        float angle;
        float rotations;


    
    public:

        void updateGlobals();

        void aRiseCallback();
        void bRiseCallback();
        void aFallCallback();
        void bFallCallback();
        

        Motor(PinName PIN_A, PinName PIN_B, PinName MOTOR_1, PinName MOTOR_2, PinName MOTOR_3,
                 PinName MOTOR_4);

        void motorPower(float power);

        void spinDegrees(int degrees);

        int getDegrees();
};

#endif //_MOTOR_H_