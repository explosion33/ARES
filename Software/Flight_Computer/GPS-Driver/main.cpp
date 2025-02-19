#include "mbed.h"
#include "EUSBSerial.h"

int main(){
    DigitalOut led_G(PA_15);
    DigitalOut led_B(PA_8);

    EUSBSerial pc(true); // new class Extended USB Serial
    BufferedSerial gps(PA_2, PA_3);

    led_B.write(1);
    
    // setup timer for use in secondary actions
    Timer t;
    t.start();

    char c;
    while(true){
        char buf[256] = {0};
        int index = 0;

        if (gps.readable()) {
            gps.read(buf[index], 1);
            index ++;
        }

        if (index != 0 && buf[index-1] == '\n') {
            // call GPS code
            index = 0;
        }
        
        
        
        // // if gps has characters, read into c and write it to USB
        // if (gps.readable()){ 
        //     gps.read(&c, 1);
        //     pc.write(&c, 1);
        // }

        // // every 5s & if GPS line has been sent toggle LED & print to USB
        // if (t.read_ms() >= 5000 && c == '\n') {
        //     pc.printf("task #%d\n", 2);
        //     led_B = !led_B;
        //     t.reset();
        // }

    }
}

