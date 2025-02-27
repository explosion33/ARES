#include "mbed.h"
#include "EUSBSerial.h"
#include "GPS.h"
#include <string> // only need these for testing w/ artificial messages

int main(){
    DigitalOut led_G(PA_15);
    DigitalOut led_B(PA_8);

    EUSBSerial pc(true); // new class Extended USB Serial
    BufferedSerial gps_serial(PA_2, PA_3);

    led_B.write(1);
    
    // setup timer for use in secondary actions
    Timer t;
    t.start();

    GPS gps; // instantiate

    char buf[256] = {0};
    int index = 0;
    while(true){
        // // read each character of the message and send to the corresponding buffer index
        // if (gps_serial.readable()) {
        //     gps_serial.read(&buf[index], 1);
        //     index ++;
        // }


        // TESTING SENDING MESSAGES
        if (t.read_ms() >= 5000){
            // artifical GGA message
            const char* msg = "$GPGGA,091626.000,2236.2791,N,12017.2818,E,1,10,1.00,8.8,M,18.7,M,0.5,*66";
            NMEA_Type msgType = gps.getMsgType(msg);
            gpsDebug debug = gps.update(msgType, msg);
            pc.printf("==================================\n");
            pc.printf("%s", msg); // write the message to serial port
            pc.printf("Message Type: %d\n", msgType); // write the message type
            gpsState state = gps.getState();
            pc.printf("UTC\t\t: %f \nFix\t\t: %d \nLattitude\t: %f %c \nLongitude\t: %f %c\nAltitude\t: %f\n", \
                            state.utc, state.fix, state.lat, state.latNS, state.lon, state.lonEW, state.alt);
            pc.printf("DEBUG\t\t: %d\n", debug);
            t.reset();
        }


        // if (index != 0 && buf[index-1] == '\n') {
        //     buf[index] = 0; // terminate the string to negate leftovers
        //     NMEA_Type msgType = gps.getMsgType(buf);
        //     gpsDebug debug = gps.update(msgType, buf); // TODO: combine getMsgType() & udpate() w/ wrapper
        //     pc.printf("==================================\n");
        //     pc.printf("%s", buf); // write the message to serial port
        //     pc.printf("Message Type: %d\n", msgType); // write the message type
        //     gpsState state = gps.getState();
        //     pc.printf("UTC\t\t: %f \nFix\t\t: %d \nLattitude\t: %f %c \nLongitude\t: %f %c\nAltitude\t: %f\n", \
        //                     state.utc, state.fix, state.lat, state.latNS, state.lon, state.lonEW, state.alt);
        //     pc.printf("DEBUG\t\t: %d\n", debug);
        //     index = 0; // reset 
        // }
    }

    // char c;

    // while(true) {
    //     // if gps has characters, read into c and write it to USB
    //     if (gps.readable()){ 
    //         gps.read(&c, 1);
    //         pc.write(&c, 1);
    //     }

    //     // every 5s & if GPS line has been sent toggle LED & print to USB
    //     if (t.read_ms() >= 5000 && c == '\n') {
    //         pc.printf("task #%d\n", 2);
    //         led_B = !led_B;
    //         t.reset();
    //     }
    // }

}

