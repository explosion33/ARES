/* mbed Microcontroller Library
 * Copyright (c) 2019 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
 */

#include "mbed.h"
#include "USBSerial.h"


char buf[50] = {0};
int index = 0;

int main() {
    USBSerial pc(false, 0x1F00, 0x2012);
    ThisThread::sleep_for(2s);
    pc.printf("start\n");

    PwmOut Q1(PA_1);
    DigitalOut Q2(PB_1);
    PwmOut Q3(PB_10);
    DigitalOut Q4(PB_13);

    Q1.period_ms(5);
    Q3.period_ms(5);

    Q1.write(0.0f);
    Q2 = 0;
    Q3.write(0.0f);
    Q4 = 0;

    DigitalIn btn(PC_13);

    int speed = 100;
    bool dir = true;
    while (true) {
        index = 0;
        while (true) {
            buf[index] = pc._getc();
            index ++;

            if (buf[index-1] == '\n') {
                buf[index] = 0;
                break;
            }
        }

        if (strstr(buf, "switch")) {
            pc.printf("switching\n");

            dir = !dir;
            Q1.write(0.0f);
            Q2 = 0;
            Q3.write(0.0f);
            Q4 = 0;

            ThisThread::sleep_for(1ms);

            if (dir) {
                Q1.write((float) speed / 100.0f);
                Q4 = 1;
            }
            else {
                Q3.write((float) speed / 100.0f);
                Q2 = 1;
            }
        }

        else if (sscanf(buf, "%d", &speed) == 1) {
            if (speed > 100)
                speed = 100;
            else if (speed < 0)
                speed = 0;

            if (dir) {
                Q1.write((float) speed / 100.0f);
                Q4 = 1;
            }
            else {
                Q3.write((float) speed / 100.0f);
                Q2 = 1;
            }
            
            pc.printf("setting speed to %d\n", speed);
        }
    }

}
