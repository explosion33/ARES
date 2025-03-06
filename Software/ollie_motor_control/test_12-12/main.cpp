#include "mbed.h"
#include <string>
#include "Motor.h"
#include "PID.h"

// Define the pins for the encoder
// #define PIN_A PA_8 
// #define PIN_B PA_10 
// #define MOTOR_1 PB_2
// #define MOTOR_2 PB_1
// #define MOTOR_3 PB_15
// #define MOTOR_4 PB_14

// Motor motor(PA_8, PA_10, PB_2, PB_1, PB_15, PB_14);
Motor motor1(PB_3, PB_5, PA_11, PA_12, PA_10, PA_9);
Motor motor2(PA_6, PA_5, PB_14, PB_15, PB_13, PA_8);
// Motor angles
float theta1 = 0;
float theta2 = 0;

PID pid(500, 0, 0);

// TODO: void toDegrees

// main() runs in its own thread in the OS
int main()
{
    printf("start\n");
    DigitalOut led1(PC_14);
    led1.write(0);

    while (true) {
        ThisThread::sleep_for(10ms);
        motor1.motorPower(pid.compute(motor1.getDegrees(), 720, 10));
    }
}

