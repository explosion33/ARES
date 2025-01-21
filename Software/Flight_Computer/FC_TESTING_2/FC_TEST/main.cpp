#include "mbed.h"

int main(){
    DigitalOut led_G(PA_15);
    DigitalOut led_B(PA_8);

    while(true){

        for(int i = 0; i<4; i++){
            led_B.write(0);
            led_G.write(1);
            ThisThread::sleep_for(200ms);
            led_B.write(1);
            led_G.write(0);
            ThisThread::sleep_for(200ms);
        }

        for(int i = 0; i<4; i++){
            led_B.write(0);
            ThisThread::sleep_for(200ms);
            led_B.write(1);
            ThisThread::sleep_for(200ms);
        }

        
    }
}

