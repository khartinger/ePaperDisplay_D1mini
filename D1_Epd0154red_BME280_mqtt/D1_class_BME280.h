//_____D1_class_BME280.h______________________180326-190326_____
// D1 mini class for temperature, humidity and pressure/altitude 
// sensor BME280.
// * temperature -40°C...85°C +-1°, 0,01°C resolution
// * humidity    0%...100% +-3%RH absolut, 0,008%RH resolution
// * pressure    300...1100hPa +-1,0hPa 
// Default i2c address is 0x76 (other 0x77)
// Code based on Adafruit_BME280.h/.cpp and SparkFunBME280.h/.cpp

#ifndef D1_CLASS_BME280_H
#define D1_CLASS_BME280_H
#include "Arduino.h"                   // 
#include <Wire.h>                      // I2C library
#define BME280_ADDR          (0x76)    // 7-bit address
#define WAIT_MEASURING_MS    500       // time between 2 measure
#define SEALEVELPRESSURE_HPA (1013.25) // Hektopascal

//-----convert 32-bit-value to float----------------------------
#define BME280_FT(tmp)       ((((tmp)*5+128)>>8)/100.0F)
#define BME280_FH(hum)       (((hum)>>12)/1024.0F)
#define BME280_FP(pre)       ((pre)/256.0F)
#define BME280_FA(pre,sea)   (44330.0*(1.0-pow(pre/sea,0.1903)))

//-----error values of measurement values-----------------------
#define BME280_ERR_T         0x80000
#define BME280_ERR_P         0x80000
#define BME280_ERR_H         0x8000
#define BME280_ERR_FLOAT     NAN
//-----status---------------------------------------------------
#define BME280_OK            0    // success
#define BME280_ERR_TOO_LONG  1    // i2c data too long to fit in transmit buffer 
#define BME280_ERR_NACK_ADDR 2    // i2c NACK transmit address
#define BME280_ERR_NACK_DATA 3    // i2c NACK transmit data
#define BME280_ERR_OTHER     4    // i2c other error
#define BME280_ERR_NUM_BYTES 5    // i2c wrong number of bytes
#define BME280_ERR_NO_BYTE   6    // no byte received
#define BME280_ERR_NUM_CHARS 7    // wrong number of chars
#define BME280_ERR_RANGE     8    // wrong range
#define BME280_ERR_NO_MEAS   10   // no measure until now
#define BME280_ERR_ID        11   // wrong id
#define BME280_ERR_RESET     12   // reset error

//-----register-------------------------------------------------
#define BME280_REG_DIG_T1    0x88
#define BME280_REG_DIG_T2    0x8A
#define BME280_REG_DIG_T3    0x8C

#define BME280_REG_DIG_P1    0x8E
#define BME280_REG_DIG_P2    0x90
#define BME280_REG_DIG_P3    0x92
#define BME280_REG_DIG_P4    0x94
#define BME280_REG_DIG_P5    0x96
#define BME280_REG_DIG_P6    0x98
#define BME280_REG_DIG_P7    0x9A
#define BME280_REG_DIG_P8    0x9C
#define BME280_REG_DIG_P9    0x9E

#define BME280_REG_DIG_H1    0xA1
#define BME280_REG_DIG_H2    0xE1
#define BME280_REG_DIG_H3    0xE3
#define BME280_REG_DIG_H4    0xE4
#define BME280_REG_DIG_H5    0xE5
#define BME280_REG_DIG_H6    0xE7

#define BME280_REG_CHIPID    0xD0 //Chip ID Register (0x60)
#define BME280_REG_VERSION   0xD1 
#define BME280_REG_RESET     0xE0 //Write 0xB6 resets device

#define BME280_REG_CALIB00   0x88 //Calibration reg 0x88-0xA1
#define BME280_REG_CALIB26   0xE1 //Calibration reg 0xE1-0xF0

#define BME280_REG_CTRL_HUM  0xF2 //Humidity Control Register
#define BME280_REG_STATUS    0XF3 //Status Register
#define BME280_REG_CTRL_MEAS 0xF4 //Control Measure Register
#define BME280_REG_CONFIG    0xF5 //Configuration Register
#define BME280_REG_PRESDATA  0xF7 //Pressure    MSB-LSB-XLSB
#define BME280_REG_TEMPDATA  0xFA //Temperature MSB-LSB-XLSB
#define BME280_REG_HUMIDATA  0xFD //Humidity    MSB-LSB

//-----register content-----------------------------------------
#define BME280_CHIPID        0x60 //Chip ID
#define BME280_RESET         0xB6 //Write 0xB6 resets device

//-----parts of register content--------------------------------
 typedef enum {
  MODE_SLEEP  = 0b00,
  MODE_FORCED = 0b01,
  MODE_NORMAL = 0b11
 } bme280_mode;
 
typedef enum {
  SAMPLING_NONE = 0b000,
  SAMPLING_X1   = 0b001,
  SAMPLING_X2   = 0b010,
  SAMPLING_X4   = 0b011,
  SAMPLING_X8   = 0b100,
  SAMPLING_X16  = 0b101
 } bme280_sampling;

 typedef enum {
  FILTER_OFF = 0b000,
  FILTER_X2  = 0b001,
  FILTER_X4  = 0b010,
  FILTER_X8  = 0b011,
  FILTER_X16 = 0b100
 } bme280_filter;

//-----standby durations in ms----------------------------------
 typedef enum {
  STANDBY_MS_0_5  = 0b000,
  STANDBY_MS_10   = 0b110,
  STANDBY_MS_20   = 0b111,
  STANDBY_MS_62_5 = 0b001,
  STANDBY_MS_125  = 0b010,
  STANDBY_MS_250  = 0b011,
  STANDBY_MS_500  = 0b100,
  STANDBY_MS_1000 = 0b101
 } bme280_standby;

//-----structure to save calibration data-----------------------
typedef struct
{
 uint16_t dig_T1;
 int16_t  dig_T2;
 int16_t  dig_T3;
 uint16_t dig_P1;
 int16_t  dig_P2;
 int16_t  dig_P3;
 int16_t  dig_P4;
 int16_t  dig_P5;
 int16_t  dig_P6;
 int16_t  dig_P7;
 int16_t  dig_P8;
 int16_t  dig_P9;
 uint8_t  dig_H1;
 int16_t  dig_H2;
 uint8_t  dig_H3;
 int16_t  dig_H4;
 int16_t  dig_H5;
 int8_t   dig_H6;
} bme280_calib;
  
class BME280 {
 //-----properties----------------------------------------------
 protected:
  int i2cAddress;                 // i2c address
  int status;                     // state of measuring
  unsigned long nextMeasuring_;   // time of next measuring [ms]
  unsigned long waitMeasuring_;   // waiting time between 2 meas
  int32_t iHum_;                  // humidity [%]
  int64_t iPre_;                  // pressure [pascal]
  int32_t iTmp_;                  // fine resolution temperature
                                  // for press/humid compensation
  double  altitude_;              // unit: meter
  double  seaLevel_hPa;           // unit: pascal
  bme280_calib calib_;            // calibration data
  byte regCtrlHum;                // 2-0 samplH
  byte regCtrlMeas;               // 7-5 sampT 4-2 sampP 1-0 mode
  byte regConfig;                 // 7-5 standby 4-2 filter
    
 //-----constructor & co----------------------------------------
 public:
  BME280();
  BME280(int i2c_address);
 protected:
  void   setup();
 public:
  bool   setParams(bme280_mode mode, bme280_sampling temp,
          bme280_sampling pres,bme280_sampling humi,
          bme280_filter filter, bme280_standby  standby);
  bool   begin();
  bool   softReset();
 protected:
  bool   checkID();
  bool   isReadingCalibration();
  bool   readCompensationParams(void);
 //-----setter and getter methods-------------------------------
 public:
  void   setWaitMeasuring(unsigned long wait_ms);
  int    getStatus();
  String getsStatus();
  int    getValues(float &temperature, float &humidity, 
                   float &pressure,    float &altitude);
  String getsValues();
  String getsValues(String sep);
  String getsValues(String sep,int dt,int dh,int dp,int da);
  String getsJson();
  String getsJson(int dt,int dh,int dp,int da);
  float  getTemperature();
  float  getHumidity();
  float  getPressure();
  float  getAltitude();
 
 //-----helper functions----------------------------------------
 protected:
  void    measuring();
  String  float2String(float f, int len, int decimals);
 //-----helper functions: i2c-access----------------------------
 bool     write8(byte reg, byte value);     // write 1 byte
 uint8_t  read8(byte reg);        // read 1 byte
 uint16_t read16(byte reg);       // read 2 byte
 uint32_t read24(byte reg);       // read 3 byte
 int16_t  readS16(byte reg);      // read 16bit signed
 uint16_t read16_LE(byte reg);    // little endian (lo-hi byte)
 int16_t  readS16_LE(byte reg);   // signed little endian
};
#endif
