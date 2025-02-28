#include "mbed.h"
#include <string>

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


        // Updates global variables
        void updateGlobals() {
            position = encoderCounter;
            angle = (encoderCounter * 360.0) / totalCounts;
            rotations = angle / 360.0;
        }


        void aRiseCallback() {
            aUp = true;

            if (!bUp) {
                encoderCounter++;
            } else {
                encoderCounter--;
            }

        }

        void bRiseCallback() {
            bUp = true;

            if (aUp) {
                encoderCounter++;
            } else {
                encoderCounter--;
            }
        }

        void aFallCallback() {
            aUp = false;

            if (bUp) {
                encoderCounter++;
            } else {
                encoderCounter--;
            }
        }

        void bFallCallback() {
            bUp = false;

            if (!aUp) {
                encoderCounter++;
            } else {
                encoderCounter--;
            }
        }
    

    public:

        Motor(PinName PIN_A, PinName PIN_B, PinName MOTOR_1, PinName MOTOR_2, PinName MOTOR_3,
                 PinName MOTOR_4) : encoderA(PIN_A), encoderB(PIN_B), motorPin1(MOTOR_1), 
                 motorPin3(MOTOR_3), motorPin2(MOTOR_2), motorPin4(MOTOR_4) {

            // Init pins and set pin modes for encoders
            encoderA.mode(PullDown);
            encoderB.mode(PullDown);

            if (encoderA.read() == 1) {
                aUp = true;
            }
            if (encoderB.read() == 1) {
                bUp = true;
            }

            // Attach the address of the encoderCallback function to the rising edge
            // encoderA.rise(&aRiseCallback);
            // encoderB.rise(&bRiseCallback);
            // encoderA.fall(&aFallCallback);
            // encoderB.fall(&bFallCallback);
            encoderA.rise([this]() {aRiseCallback();});
            encoderA.fall([this]() {aFallCallback();});
            encoderB.rise([this]() {bRiseCallback();});
            encoderB.fall([this]() {bFallCallback();});
        }

        void motorPower(float power) {
            if (power > 0) {
                //set forward pins proportional to powe
                this->motorPin1.write(1);
                motorPin4.write(power);
            } else if (power < 0) {
                //set backwards pins prop to |power|
                motorPin3.write(1);
                motorPin2.write(-power);

            } else {
                //set all pins to off
                motorPin1.write(0);
                motorPin2.write(0);
                motorPin3.write(0);
                motorPin4.write(0);
            }
        }


        void spinDegrees(int degrees) {
            printf("spin degrees\n");
            int initial = (int)angle;
            if (degrees > 0) {
                printf("positive\n");
                motorPower(1);
                while (angle < (initial + degrees)) {
                    ThisThread::sleep_for(10ms);
                    updateGlobals();
                }
                printf("off");
                motorPower(0);
            } else if (degrees < 0) {
                printf("negative motor power\n");
                motorPower(-1);
                ThisThread::sleep_for(100ms);
                while (degrees > (initial + degrees)) {
                    ThisThread::sleep_for(10ms);
                    updateGlobals();
                }
                printf("off");
                motorPower(0);
            }
        }


        // float getSpeed() {
        //     updateGlobals();
        //     float rotations_1 = rotations;
        //     // TODO: copy rest from arduino code
        // }

        int getDegrees() {
            updateGlobals();
            return (int)angle;
        }

};