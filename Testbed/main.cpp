/* mbed Microcontroller Library
 * Copyright (c) 2019 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
 */

#include "mbed.h"
#include "USBSerial.h"


// Blinking rate in milliseconds
#define BLINKING_RATE     500ms


int main() {
    DigitalOut led(PA_15);
    USBSerial pc(false, 0x1F00, 0x2012);

    size_t i = 0;
    while (true) {
        led = !led;
        pc.printf("%d\n", i);
        i ++;
        ThisThread::sleep_for(BLINKING_RATE);
    }
}
