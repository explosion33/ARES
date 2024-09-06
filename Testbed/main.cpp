/* mbed Microcontroller Library
 * Copyright (c) 2019 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
 */

#include "mbed.h"
#include "USBSerial.h"


// Blinking rate in milliseconds
#define BLINKING_RATE     500ms


I2C i2c(PB_7, PB_8);
void scanI2C(USBSerial* pc) {
    pc->printf("Starting I2C Scan\n");
    int ack;
    int address;
    for(address=1;address<256;address++) {    
        ack = i2c.write(address, "11", 1);
        if (ack == 0) {
            pc->printf("\tFound at %3d -- %3x\r\n", address>>1,address>>1);
        }    
    } 
    pc->printf("I2C Scan Finished\n");
}

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
