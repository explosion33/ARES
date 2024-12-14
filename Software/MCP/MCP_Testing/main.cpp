/* mbed Microcontroller Library
 * Copyright (c) 2019 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
 */

#include "mbed.h"


int main() {
    DigitalOut led(PC_13);
    DigitalOut led2(PC_14);

    PwmOut Q1(PA_11);
    DigitalOut Q2(PA_12);
    PwmOut Q3(PA_10);
    DigitalOut Q4(PA_9);

    Q1.period_ms(5);
    Q3.period_ms(5);

    Q1.write(0.0f);
    Q2 = 0;
    Q3.write(0.0f);
    Q4 = 0;

    while (true) {
        Q1.write(0.2f);
        Q4.write(1);
        led.write(1);
        led2.write(0);
        ThisThread::sleep_for(3s);

        Q1.write(0.0f);
        Q2 = 0;
        Q3.write(0.0f);
        Q4 = 0;

        ThisThread::sleep_for(20ms);

        Q3.write(0.2f);
        Q2.write(1);
        led.write(0);
        led2.write(1);
        ThisThread::sleep_for(3s);

        Q1.write(0.0f);
        Q2 = 0;
        Q3.write(0.0f);
        Q4 = 0;

        ThisThread::sleep_for(20ms);
    }
}