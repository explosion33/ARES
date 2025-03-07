#include"mbed.h"
#include"BMP280.h" 
#include"BMP280_const.h" 
#include<map> 
#include<iostream>
using namespace std;

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
int BMP280::updateTemperatureData(){
    char xlsb, lsb, msb; 
    int msbErr, lsbErr, xlsbErr; 
    xlsbErr = readData(BMP280_TEMP_XLSB, &xlsb, 1);
    lsbErr = readData(BMP280_TEMP_LSB, &lsb, 1);
    msbErr = readData(BMP280_TEMP_MSB, &msb, 1);
    
    int total = xlsbErr + lsbErr + msbErr;
    //Shifts each byte into useful position
    uint32_t rawTemperature = ((uint32_t)msb << 12) | ((uint32_t)lsb << 4 ) | ((uint32_t)xlsb >> 4);
    // float temp = ((float)rawTemperature * 9.0/5.0) + 32.0; // Convert from native Celcius to Farienheit
    values.temp_f = convert_temp(rawTemperature);
    return total; 
}

/* @brief Retrieves and organizes pressure data
 * @return calulated pressure value
 */ 
int BMP280::updatePressureData(){
    char xlsb, lsb, msb;
    int msbErr, lsbErr, xlsbErr; 
    xlsbErr = readData(BMP280_PRESS_XLSB, &xlsb, 1); // Extra least significant byte 
    lsbErr = readData(BMP280_PRESS_LSB, &lsb, 1);// Least significant byte 
    msbErr = readData(BMP280_PRESS_MSB, &msb, 1); // Most significant byte 
    int total = xlsbErr + lsbErr + msbErr;

    // Shifts each byte into useful position 
    uint32_t rawPressure = ((uint32_t)msb << 12) | ((uint32_t)lsb << 4 ) | ((uint32_t)xlsb >> 4);
    // float press = (float)rawPressure * 0.0145037738; // Convert from native Hectopascal to psi 
    values.press_psi = (float)rawPressure * .0016 * 0.0145038; 

    return total; 
}

float convert_temp(int32_t adc_T){
    char calib[6];
    readData(0x88, calib, 6);
    uint16_t dig_T1 = calib[1] << 8 | calib[0];
    int16_t dig_T2 = (calib[3] << 8 | calib[2]);
    int16_t dig_T3 = (calib[5] << 8 | calib[4]);


    int32_t var1, var2, t_fine;

    var1 = ((((adc_T >> 3) - ((int32_t)dig_T1 << 1))) * ((int32_t)dig_T2)) >> 11;
    var2 = (((((adc_T >> 4) - ((int32_t)dig_T1)) * ((adc_T >> 4) - ((int32_t)dig_T1))) >> 12) * ((int32_t)dig_T3)) >> 14;

    t_fine = var1 + var2;
    float temperature = (t_fine * 5 + 128) >> 8;
    return temperature / 100.0f; // Convert to Celsius
}

// @breif returns pressure data
uint32_t BMP280::getPressure(){
    return values.press_psi;
}

// @brief returns temperature value
uint32_t BMP280::getTemperature(){
    return values.temp_f;
}

// @breif updates sensor values 
int BMP280::updateValues(){
    int errPress  = updatePressureData();
    int errTemp = updateTemperatureData();
    return(errPress + errTemp);
}

/* @breif check for error from the pressure reading
*/
BMP280_Status BMP280::getPressureStatus() {
 return BMP280_Status::Ok; 
}
