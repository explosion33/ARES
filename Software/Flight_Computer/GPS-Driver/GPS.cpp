#include "EUSBSerial.h"
#include "GPS.h"


gpsState GPS::getState() const{
    return state; // return a copy of the state (can't be modified bc its private)
}

// say const so that it can't be modified (only reading it)
// msg will be the entire line 
NMEA_Type GPS::getMsgType(const char* msg) {

    switch (msg[4]) {

        case 'G': { // GGA
            return NMEA_GGA; // Might need NMEA_Type::NMEA_GGA;
            break;
        }

        case 'S': { // GSA
            return NMEA_GSA;
            break;
        }

        case 'M': { // RMC
            return NMEA_GGA;
            break;
        }

        case 'T': { // VTG
            return NMEA_VTG;
            break;
        }
    }
    return NMEA_NA; // No type found
}

/*
typedef enum {
    NMEA_NA,    // 0
    NMEA_GGA,   // 1
    NMEA_GSA,   // 2
    NMEA_RMC,   // 3
    NMEA_VTG    // 4
} NMEA_Type;
*/




// Q: should I call getMsgType inside update() or leave them separate?
void GPS::update(int msgType, const char* msg){

    // initialize state variables
    int _;
    double utc;
    double lat;
    char latNS;
    double lon;
    char lonEW;
    int fix;
    double alt;
    double hdop;
    double heading; 
    double gspeed;

    // currently unused vars
    int nsats;
    char altUnits;
    double gsep;
    char gsepUnits;
    double ageCorrection;
    int checksum;

    switch (msgType) {
        
        case NMEA_NA: { // type not recognized
            printf("Invalid NMEA message type\n");
            break;
        }

        case NMEA_GGA: { // GGA
            int result = sscanf(msg, "$GPGGA,%lf,%lf,%c,%lf,%c,%d,%d,%lf,%lf,%c,%lf,%c,%lf,*%d", &utc, &lat, &latNS, &lon, \
                        &lonEW, &fix, &nsats, &hdop, &alt, &altUnits, &gsep, &gsepUnits, &ageCorrection, &checksum);
            
            if (result == 14) {
                // If parsing was successful, assign values to the state
                this->state.utc = utc;
                this->state.lat = lat;
                this->state.latNS = latNS;
                this->state.lon = lon;
                this->state.lonEW = lonEW;
                this->state.fix = fix;
                this->state.hdop = hdop;
                this->state.alt = alt;
                // logState()
            } else {
                // log failure
                printf("Failed to parse GGA message\n");
            }
        
            break;
        }

        case NMEA_GSA: { // GSA
            // use sscanf to update state
            break;
        }

        case NMEA_RMC: { // RMC
            // use sscanf to update state
            break;
        }

        case NMEA_VTG: { // VTG
            // use sscanf to update state
            break;
        }
    }
}