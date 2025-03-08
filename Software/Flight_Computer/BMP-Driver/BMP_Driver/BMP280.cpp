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
    int32_t rawTemperature = ((int32_t)msb << 12) | ((int32_t)lsb << 4 ) | ((int32_t)xlsb >> 4);
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
    values.press_psi = BMP280_compensate_P_double(rawPressure); 

    return total; 
}

double BMP280::convert_temp(int32_t adc_T){
    int err = BMP280_CalibrateTemp();

    double var1, var2, T;
    var1 = (((double)adc_T)/16384.0 - ((double)c.dig_T1)/1024.0) * ((double)c.dig_T2);
    var2 = ((((double)adc_T)/131072.0 - ((double)c.dig_T1)/8192.0) * 
    (((double)adc_T)/131072.0 - ((double)c.dig_T1)/8192.0)) * ((double)c.dig_T3);
    t_fine = (int32_t)(var1 + var2);
    T = (var1 + var2)/5120.0;
    return T;
}

// @breif returns pressure data
float BMP280::getPressure(){
    return values.press_psi;
}

double BMP280::BMP280_compensate_P_double(int32_t adc_P){
    int err = BMP280_CalibratePress(); 

    double var1, var2, p;
    var1 = ((double)t_fine/2.0) - 64000.0;
    var2 = var1 * var1 * ((double)c.dig_P6) / 32768.0;
    var2 = var2 + var1 * ((double)c.dig_P5) * 2.0;
    var2 = (var2/4.0)+(((double)c.dig_P4) * 65536.0);
    var1 = (((double)c.dig_P3) * var1 * var1 / 524288.0 + ((double)c.dig_P2) * var1) / 524288.0;
    var1 = (1.0 + var1 / 32768.0)*((double)c.dig_P1);
    if (var1 == 0.0){
        return 0; // avoid exception caused by division by zero
    }
    p = 1048576.0 - (double)adc_P;
    p = (p - (var2 / 4096.0)) * 6250.0 / var1;
    var1 = ((double)c.dig_P9) * p * p / 2147483648.0;
    var2 = p * ((double)c.dig_P8) / 32768.0;
    p = p + (var1 + var2 + ((double)c.dig_P7)) / 16.0;
    return p;
}



// @brief returns temperature value
float BMP280::getTemperature(){
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

//@briif calibrates temperature values
// - Array Calib: each calibration number comes in a lsb and msb pair 
int BMP280::BMP280_CalibrateTemp(){
    char calib[6];
    readData(0x88, calib, 6);
    c.dig_T1 = calib[1] << 8 | calib[0];
    c.dig_T2 = (calib[3] << 8 | calib[2]);
    c.dig_T3 = (calib[5] << 8 | calib[4]);
    return 0;
}

int BMP280::BMP280_CalibratePress(){
    char calib[18];

    // Read Calibration data 
    readData(0x8E, calib, 18);

    // int t1 = readData(0x8E, &calib[0],1);
    // int t2 = readData(0x8F, &calib[1], 1);
    // int t3 = readData(0x90, &calib[2],1);
    // int t4 = readData(0x91, &calib[3], 1);
    // int t5 = readData(0x92, &calib[4],1);
    // int t6 = readData(0x93, &calib[5], 1);
    // int t7 = readData(0x94, &calib[6], 1);
    // int t8 = readData(0x95, &calib[7], 1);
    // int t9 = readData(0x96, &calib[8], 1);
    // int t10 = readData(0x97, &calib[9], 1);
    // int t11 = readData(0x98, &calib[10],1);
    // int t12 = readData(0x99, &calib[11], 1);
    // int t13 = readData(0x9A, &calib[12], 1);
    // int t14 = readData(0x9B, &calib[13], 1);
    // int t15 = readData(0x9C, &calib[14], 1);
    // int t16 = readData(0x9D, &calib[15], 1);
    // int t17 = readData(0x9E, &calib[16], 1);
    // int t18 = readData(0x9F, &calib[17], 1);

    // int err = t1 + t2 + t3 + t4 + t5 + t6 + t7 + t8 + t9 + t10 +
    //         t11 + t12 + t13 + t14 + t15 + t16 + t17 + t18;

    //Store Calibration data
    c.dig_P1 = calib[0] | calib[1] << 8;
    c.dig_P2 = calib[2] | calib[3] << 8;
    c.dig_P3 = calib[4] | calib[5] << 8;
    c.dig_P4 = calib[6] | calib[7] << 8;
    c.dig_P5 = calib[8] | calib[9] << 8;
    c.dig_P6 = calib[10] | calib[11] << 8;
    c.dig_P7 = calib[12] | calib[13] << 8;
    c.dig_P8 = calib[14] | calib[15] << 8;
    c.dig_P9 = calib[16] | calib[17] << 8;

    return 0;

}