/* mbed Microcontroller Library
 * Copyright (c) 2019 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
 */

#include "mbed.h"


int main() {
    DigitalOut led(PA_15);
    DigitalOut led2(PA_8);

    led.write(1);
    led2.write(0);

    while (true) {
        ThisThread::sleep_for(500ms);
        led  = !led;
        led2 = !led2;
    }
}