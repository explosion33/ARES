#include "mbed.h"



int main(){
    DigitalOut led(PA_15);
    DigitalOut led2(PA_8);

    while(true){
        led.write(0);
        led2.write(1);
        ThisThread::sleep_for(200ms);
        led.write(1);
        led2.write(0);
        ThisThread::sleep_for(200ms);
    }
}