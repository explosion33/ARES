#include "mbed.h"
#include <string>
#include "Motor.cpp"

// Define the pins for the encoder
// #define PIN_A PA_8 
// #define PIN_B PA_10 
// #define MOTOR_1 PB_2
// #define MOTOR_2 PB_1
// #define MOTOR_3 PB_15
// #define MOTOR_4 PB_14

// Motor motor(PIN_A, PIN_B, PA_11, MOTOR_2, MOTOR_3, MOTOR_4);
Motor motor(PB_3, PB_5, PA_11, PA_12, PA_10, PA_9);

// TODO: void toDegrees

/* DISCONTINUED
void encoderCallback() {
    int aCurrent = encoderA.read();
    int bCurrent = encoderB.read();
    counter++;

    if (aCurrent != lastA) {
        if (aCurrent == HIGH) { // Rising edge of A
            if (bCurrent == LOW) {
                encoderCounter++; // Clockwise
            } else {
                encoderCounter--; // Counterclockwise
            }
        } else { // Falling edge of A
            if (bCurrent == HIGH) {
                encoderCounter++; // Clockwise
            } else {
                encoderCounter--; // Counterclockwise
            }
        }
        lastA = aCurrent;
    } else {
        counter += 1000;
    }

    if (bCurrent != lastB) {
        if (bCurrent == HIGH) { // Rising edge of B
            if (aCurrent == HIGH) {
                encoderCounter++; // Clockwise
            } else {
                encoderCounter--; // Counterclockwise
            }
        } else { // Falling edge of B
            if (aCurrent == LOW) {
                encoderCounter++; // Clockwise
            } else {
                encoderCounter--; // Counterclockwise
            }
        }
        lastB = bCurrent;
    } else {
        counter -= 1000;
    }
} */
// ETHAN'S METHOD

/*/ Callback global
bool fired = false; //idk which to start it at, I think 
                    // this works assuming a fires first

void aRiseCallback() {
    counter++;

    if (!fired) {
        fired = true;
        encoderCounter++;
    } else {
        fired = false;
        encoderCounter--;
    }
}
void bRiseCallback() {
    counter++;

    if (fired) {
        fired = false;
        encoderCounter++;
    } else {
        fired = true;
        encoderCounter--;
    }
}//*/


// main() runs in its own thread in the OS
int main()
{
    printf("start\n");
    motor.motorPower(0);
    ThisThread::sleep_for(1000ms);
    DigitalOut led1(PC_14);
    // DigitalOut led2
    while (true) {
        printf("main\n");
        led1.write(1);
        motor.motorPower(-1);


        ThisThread::sleep_for(500ms);
        printf("degrees: %d \n", motor.getDegrees());
        // printf("rotations: %d\n", (int)rotations);
        // printf("encoder a: %d\n", encoderA.read());
        // printf("encoder b: %d\n\n", encoderB.read());
    }
}

