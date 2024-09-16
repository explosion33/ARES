/* mbed Microcontroller Library
 * Copyright (c) 2019 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
 */

#include "mbed.h"
#include <cstdio>


char buf[50] = {0};
int index = 0;

int main() {
    DigitalOut Q1(PA_0);
    DigitalOut Q2(PA_1);
    DigitalOut Q3(PA_4);
    DigitalOut Q4(PB_0);

    Q1 = 0;
    Q2 = 0;
    Q3 = 0;
    Q4 = 0;

    printf("start\n");
    DigitalIn btn(PC_13);

    bool press = false;
    bool dir = true;
    while (true) {
        if (btn.read() == 1)
            press = false;
        else if (!press && btn.read() == 0) {
            printf("switching\n");
            press = true;

            dir = !dir;
            Q1 = 0;
            Q2 = 0;
            Q3 = 0;
            Q4 = 0;
            ThisThread::sleep_for(1ms);

            if (dir) {
                Q1 = 1;
                Q4 = 1;
            }
            else {
                Q2 = 1;
                Q3 = 1;
            }
        }
    }

}
