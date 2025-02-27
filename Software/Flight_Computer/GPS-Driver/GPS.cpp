#include "EUSBSerial.h"
#include "GPS.h"
#include <cmath>

GPS::GPS() {}

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
gpsDebug GPS::update(int msgType, const char* msg){

    
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
            return notype;
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
                printf("State Succesfully Updated");
                // logState()
                return chillin;
            } else {
                // log failure
                printf("Failed to parse GGA message\n");
                return uhoh;
            }
            break;
        }

        case NMEA_GSA: { // GSA
            // use sscanf to update state
            return womp;
            break;
        }

        case NMEA_RMC: { // RMC
            // use sscanf to update state
            return womp;
            break;
        }

        case NMEA_VTG: { // VTG
            // use sscanf to update state
            return womp;
            break;
        }
    }
    return womp;
}

// posECEFg GPS::getPosECEFg() { 

//     posECEFg z = state.lat

//     return posECEFg;
// }

posLTP GPS::getPosLTP() {
    // state.lat = ddmm.mmmm ... state.lat/100 = dd.mmmmmm
    // Determine lat/lon coords as North or South & East or West
    int lat_sign;
    int lon_sign;
    switch(state.latNS) {
        case('N'): {
            lat_sign = 1;
            break;
        }
        case('S'): {
            lat_sign = -1;
            break;
        }
        printf("something is super wrong... latNS = '%c'", state.latNS);
    }
    switch(state.lonEW) {
        case('E'): {
            lon_sign = 1;
            break;
        }
        case('W'): {
            lon_sign = -1;
            break;
        }
        printf("something is super wrong... latEW = '%c'", state.lonEW);
    }

    double pi = 3.1415926535898;

    // get longitude and latitude into radians
    int lat_deg = floor(state.lat/100);
    double lat_min = fmod(state.lat, 100);
    double lat_rad = lat_sign*( (lat_deg + lat_min/60) * pi/180 );

    int lon_deg = floor(state.lat/1000);
    double lon_min = fmod(state.lat, 1000);
    double lon_rad = lon_sign*( (lon_deg + lon_min/60) * pi/180 );

    
    double a = 6378137; // earth semi-major axis        (m)
    double b = 6356752.3142; // earth semi-minor axis   (m)
    double f = (a-b)/a; // ellipsoid flatness 
    double e = sqrt(f*(2-f)); // eccentricity
    // distance from the earth's surface to the z-axis along the ellipsoid normal
    double N = a/sqrt( 1 - pow(e, 2)* pow(sin(lat_rad), 2) ); 

    // get current position in ECEF-r coordinates
    double h = state.alt;
    double x = (h + N)*cos(lat_rad)*cos(lon_rad);
    double y = (h + N)*cos(lat_rad)*sin(lon_rad);
    double z = (h + N*(1 - pow(e,2)))*sin(lat_rad);
    
    // subtract the origin ECEF-r coordinate to get relative position vector
    double x_p = x - origin.x;
    double y_p = y - origin.y;
    double z_p = z - origin.z;

    // rotate to allign y-axis w/ LTP
    double x_pp = -x_p*sin(lon_rad) + y_p*cos(lon_rad);
    double y_pp = x_p*cos(lon_rad) + y_p*sin(lon_rad);
    double z_pp = z_p;

    // final rotation to allign z_axis with up
    pos.e = x_pp;
    pos.n = -y_pp*sin(lat_rad) + z_pp*cos(lat_rad);
    pos.u = y_pp*cos(lat_deg) + z_pp*sin(lat_rad);

    return pos;
}