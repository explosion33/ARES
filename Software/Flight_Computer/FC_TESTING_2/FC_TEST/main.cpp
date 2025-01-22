#include "mbed.h"

int main(){
    DigitalOut led_G(PA_15);
    DigitalOut led_B(PA_8);

    while(true){

        led_B.write(1);
        ThisThread::sleep_for(1000ms);
        led_B.write(0);
        ThisThread::sleep_for(1000ms);

    }
}

