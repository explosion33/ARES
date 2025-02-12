#include "EUSBSerial.h"

struct {
  double lat;
  double lon;
  double alt;
  double dop;
  double heading; 
  double gspeed;
  double time;
} gpsState;

typedef enum {
    NEMA_NA,    // 0
    NEMA_GGA,   // 1
    NEMA_GSA,   // 2
    NEMA_RMC,   // 3
    NEMA_VTG    // 4
} NEMA_Type;



// say const so that it can't be modified (only reading it)
// msg will be the entire line 
NEMA_Type getMsgType(const char* msg) {

    switch (msg[4]) {

        case 'G': { // GGA
            return NEMA_GGA; // Might need NEMA_Type::NEMA_GGA;
            break;
        }

        case 'S': { // GSA
            return NEMA_GSA;
            break;
        }

        case 'M': { // RMC
            return NEMA_GGA;
            break;
        }

        case 'T': { // VTG
            return NEMA_VTG;
            break;
        }
    }

    return NEMA_NA; // No type found
}

/*
struct {
  double lat;
  double lon;
  double alt;
  double dop;
  double heading; 
  double gspeed;
  double time;
} gpsState;
*/


gpsState getStateGPS(int type, const char* msg){


    switch (type) {
        
        case 0: {

        }

        case 1: {

        }

        case 1: {
            
        }

        case 1: {
            
        }

    }
        sscanf(msg, "$GPGGA,%d.%d,%d.%d,%c,%d.%d,%c,%d,%d,%d.%d,%d.%d", &utc, &utcd, &_, &_, &_);
        float utc = utc + utcd/10

}

int _ = 0;
int utc = 0;
int utcd = 0;







/*
main() {
    
    switch(getMsgType()){
        case NEMA_GGA: {
            womp
        }
    }

}
*/



class GPS {

};