/* 
* Contains methods of the BMP280 Class and related enums and structs 
*/

#include <cstdint> 
#include "mbed.h"
#include "USBSerial.h"
using namespace std;

struct BMP280_Values {
    int32_t press_psi; // Pressure in PSI
    int32_t temp_f; // Temperature in Farenheit 
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

        BMP280_Values getPressureData();
        BMP280_Values getTemperatureData();  

    private: 

        bool owned;
        char addr; 
        I2C* i2c; 

        float readTemperatureData();
        float readPressureData();

};