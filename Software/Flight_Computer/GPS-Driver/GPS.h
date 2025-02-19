#ifndef GPS_H
#define GPS_H

#include "mbed.h"

struct gpsState{
    double lat;
    double lon;
    double alt;
    double hdop;
    double heading; 
    double gspeed;
    double utc;
    char latNS;
    char lonEW;
    int fix;
};

typedef enum {
    NMEA_NA,    // 0
    NMEA_GGA,   // 1
    NMEA_GSA,   // 2
    NMEA_RMC,   // 3
    NMEA_VTG    // 4
} NMEA_Type;


class GPS {
    private:
        gpsState state;
    
    public:
        GPS();
        gpsState getState() const;
        NMEA_Type getMsgType(const char* msg);
        void update(int msgType, const char* msg);
};

#endif // GPS_H