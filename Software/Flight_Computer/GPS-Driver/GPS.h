#ifndef GPS_H
#define GPS_H

#include "mbed.h"


struct posECEFg {   // Earth-Centered Earth-Fixed Geodic Coordinates
    double lat; // lattitude    (radians)
    double lon; // longitude    (radians)
    double alt; // altitude     (meters)
};


struct originECEFr {   // Earth-Centered Earth-Fixed Rectangular Coordinates
    double x;       // get origin of linear tangent plan in ECEF-r coordinates
    double y;       // (will likely be the launch pad)
    double z;
};

struct posLTP { // Local Tangent Plane Coordinates
    double e;   // east     (m)
    double n;   // north    (m)
    double u;   // up       (m)
};


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


typedef enum {
    chillin,    // 0
    uhoh,       // 1
    uhoh2,      // 2
    womp,       // 3
    notype      // 4
} gpsDebug;


class GPS {
    private:
        gpsState state;
        posLTP pos;
        originECEFr origin;
        double dms2rad();
        
        // getMsgType()
        // update()
    
    public:
        GPS();
        gpsState getState() const;
        NMEA_Type getMsgType(const char* msg);
        gpsDebug update(int msgType, const char* msg);
        posLTP getPosLTP();
        // functino: start (GPS LOOOP)
        // initialize buffered serial object
        // make wrapper function for getMsgType() & update()
};

#endif // GPS_H