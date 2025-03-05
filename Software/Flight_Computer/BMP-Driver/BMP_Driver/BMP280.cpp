#include"mbed.h"
#include"BMP280.h" 
#include"BMP280_const.h" 
#include<map> 

/* Constructor 
* @param Pin address of the SDA Pin on the processor 
* @param Pin address of the SCL Pin on the processor    
* @param The I2C address for the BMP280
*/
BMP280::BMP280(PinName SDA, PinName SCL, char addr){ 
    owned = true; 
    BMP280::i2c = new I2C(SDA,SCL);
    BMP280::addr = addr; 
}

/* Destructor
* Deletes I2C if created in object
*/ 
BMP280::~BMP280() {
    if(owned){
        delete i2c; 
    }
}

/*
 * @brief Reads a chunk of data from the BNO055 over I2C.
 * @param regaddr The register address to read from
 * @param data Pointer to a buffer for storing data
 * @param len Number of bytes to read
 * @return 0 on success, non-zero on failure
 */
int BMP280::readData(char regaddr, char* data, uint8_t len) {
    i2c->write(addr, &regaddr, 1);
    return i2c->read(addr, data, len);
}

/* @brief Writes data to a BNO055 register over I2C.
 * @param regaddr The register address to write to
 * @param data The value to be written
 * @return 0 on success, non-zero on failure
 */
int BMP280::writeData(char regaddr, char data) {
    char buffer[2];
    buffer[0] = regaddr;
    buffer[1] = data;
    return i2c->write(addr, buffer, 2);
}

/* @brief Retrieves and organizes temperature data
* @return calulated temperature value
*/ 
BMP280_Values BMP280::getTemperatureData(){
    char xlsb, lsb, msb; 
    readData(BMP280_TEMP_XLSB, &xlsb, 1);
    readData(BMP280_TEMP_LSB, &lsb, 1);
    readData(BMP280_TEMP_MSB, &msb, 1);

    //Shifts each byte into useful position
    int32_t rawTemperature = ((int32_t)msb << 12) | ((int32_t)lsb << 4 ) | ((int32_t)xlsb >> 4);
    rawTemperature = (rawTemperature * 9/5) + 32; // Convert from native Celcius to Farienheit

    BMP280_Values values;
    values.temp_f = rawTemperature;
    return values;
}

/* @brief Retrieves and organizes pressure data
* @return calulated pressure value
*/ 
BMP280_Values BMP280::getPressureData(){
    char xlsb, lsb, msb;
    readData(BMP280_PRESS_XLSB, &xlsb, 1); // Extra least significant byte 
    readData(BMP280_PRESS_LSB, &lsb, 1); // Least significant byte 
    readData(BMP280_PRESS_MSB, &msb, 1); // Most significant byte 

    // Shifts each byte into useful position 
    int32_t rawPressure = ((int32_t)msb << 12) | ((int32_t)lsb << 4 ) | ((int32_t)xlsb >> 4);
    rawPressure = rawPressure * 0.0145037738; // Convert from native Hectopascal to psi 

    BMP280_Values values;
    values.press_psi = rawPressure; 
    return values; 
}





