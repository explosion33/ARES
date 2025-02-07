#include "mbed.h"
#include "USBSerial.h"

int main(){
    DigitalOut led_G(PA_15);
    DigitalOut led_B(PA_8);

    USBSerial pc(true);
    BufferedSerial gps(PA_3, PA_2, 9600);

    ThisThread::sleep_for(10s);
    // led_G.write(1);
    while(true){

        if ( gps.readable() ){
            char c;
            gps.read(&c, 1);
            // pc._putc(c);

            led_B.write(1);
        }

    }
}

