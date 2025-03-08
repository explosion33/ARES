/* 
* Contains methods of the BMP280 Class and related enums and structs 
*/

#include <cstdint> 
#include "mbed.h"
#include "USBSerial.h"
using namespace std;

struct BMP280_Values {
    double press_psi; // Pressure in PSI
    double temp_f; // Temperature in Farenheit 
};


struct BMP280_Calibration {
            // Temperature 
            uint16_t dig_T1;
            int16_t dig_T2, dig_T3;

            //Pressure 
            uint16_t dig_P1;
            int16_t dig_P2, dig_P3, dig_P4, dig_P5, dig_P6, dig_P7, dig_P8, dig_P9;
};


// Error Status Handling
enum class BMP280_Status {
    Ok,
    PressErr, 
    TempErr,
};

/*
*   Data Acquisition 
*/ 

class BMP280 { 
    public:
        /* Constructor 
        * @param Pin address of the SDA Pin on the processor 
        * @param Pin address of the SCL Pin on the processor 
        * @param The I2C address for the BMP280
        */
        BMP280(PinName SDA, PinName SCL, char addr); 

        /* Destructor 
        * Deletes I2C for this object
        */ 
        ~BMP280();

        // Returns the temperature and pressure values from the sensor 

        // I2C functional methods
        int writeData(char regaddr, char data);
        int readData(char regaddr, char* data, uint8_t len);

        float getPressure();
        float getTemperature(); 
        int updateValues(); 

        

    private: 

        int updatePressureData();
        int updateTemperatureData();
        BMP280_Status getTemperatureStatus();
        BMP280_Status getPressureStatus(); 

        BMP280_Status status; 
        BMP280_Values values;
        BMP280_Calibration c;

        bool owned;
        char addr; 
        I2C* i2c; 
        int32_t t_fine;

        float readTemperatureData();
        float readPressureData();
        double convert_temp(int32_t adc_T); 
        double BMP280_compensate_P_double(int32_t adc_T);
        int BMP280_CalibrateTemp();
        int BMP280_CalibratePress();

};