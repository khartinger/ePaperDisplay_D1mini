//_____D1_class_BME280.cpp____________________180326-180326_____
// D1 mini class for temperature, humidity and pressure/altitude 
// sensor BME280.
// * temperature -40°C...85°C +-1°, 0,01°C resolution
// * humidity    0%...100% +-3%RH absolut, 0,008%RH resolution
// * pressure    300...1100hPa +-1,0hPa 
// Default i2c address is 0x76 (other 0x77)
// Code based on Adafruit_BME280.h/.cpp and SparkFunBME280.h/.cpp
#include "D1_class_BME280.h"

//**************************************************************
//    constructor & co
//**************************************************************

//_____constructor (default 0x5C)_______________________________
BME280::BME280() { i2cAddress=BME280_ADDR; setup(); }

//_____constructor with i2c Address_____________________________
BME280::BME280(int i2c_address) {
 i2cAddress=i2c_address; 
 setup();
}

//_____setup device BME280______________________________________
void BME280::setup()
{
 Wire.begin();                    //(4,5): SDA on 4, SCL on 5
 iHum_=BME280_ERR_H;              // value on error humidity
 iPre_=BME280_ERR_P;              // value on error pressure
 iTmp_=BME280_ERR_T;              // value on error temperature
 seaLevel_hPa=SEALEVELPRESSURE_HPA;
 //-----Settings for weather monitoring (see datasheet 3.5.1)---
 regCtrlHum=SAMPLING_X1;
 regCtrlMeas=(SAMPLING_X1<<5)|(SAMPLING_X1<<2)|MODE_NORMAL; //MODE_FORCED
 regConfig=(STANDBY_MS_1000<<5)|(FILTER_OFF<<2);
 //-------------------------------------------------------------
 status=BME280_ERR_NO_MEAS;
 waitMeasuring_=WAIT_MEASURING_MS;
 nextMeasuring_=millis();
 begin();
}

//_____set parameter____________________________________________
bool BME280::setParams(bme280_mode     mode,
                       bme280_sampling temp,
                       bme280_sampling pres,
                       bme280_sampling humi,
                       bme280_filter   filter,
                       bme280_standby  standby) {
 regCtrlHum=humi;
 regCtrlMeas=(temp<<5)|(pres<<2)|mode;
 regConfig=(standby<<5)|(filter<<2);
 return true;
}
  
//_____check sensor, start (first) measuring____________________
bool BME280::begin() {
 bool ok=true;
 if(!checkID()) return false;          //check chip id (0x60)
 softReset();                          //reset sensor, IIR off...
 readCompensationParams();             //i2c: read chip values
 //-----set measuring parameters--------------------------------
 ok &= write8(BME280_REG_CTRL_HUM, regCtrlHum);
 ok &= write8(BME280_REG_CTRL_MEAS, regCtrlMeas);
 ok &= write8(BME280_REG_CONFIG, regConfig);
 if(!ok) return false;
 //-------------------------------------------------------------
 //measuring();
 if(status!=BME280_OK) return false;
 return true;
}

//_____soft reset (set IIR off etc.)____________________________
// return: true=success, false=error
bool BME280::softReset() {
 int i;
 bool ok=write8(BME280_REG_RESET, BME280_RESET);
 if(!ok) { status=BME280_ERR_RESET; return false; }
 //-----wait while chip is still reading calibration------------
 for(i=10; i>0; i--)
 { if(isReadingCalibration()) delay(100);
   else i=-99;
 }
 if(i==0) { status=BME280_ERR_RESET; return false; }
 status=BME280_OK;
 return true;
}

//_____check chip id (must be 0x60)_____________________________
// return: true = chip ID OK, false = error
bool BME280::checkID() {
 uint8_t id=read8(BME280_REG_CHIPID);
 if(id!=BME280_CHIPID) { status=BME280_ERR_ID; return false; }
 status=BME280_OK;
 return true;
}

//_____check, if sensor is busy copying cal data________________
// return: true chip is busy
bool BME280::isReadingCalibration() {
 uint8_t reg = read8(BME280_REG_STATUS);
 return ((reg & 1)!=0);
}

//_____read compensation parameters from sensor non-volatile mem
bool BME280::readCompensationParams(void) {
 calib_.dig_T1 = read16_LE (BME280_REG_DIG_T1);
 calib_.dig_T2 = readS16_LE(BME280_REG_DIG_T2);
 calib_.dig_T3 = readS16_LE(BME280_REG_DIG_T3);

 calib_.dig_P1 = read16_LE (BME280_REG_DIG_P1);
 calib_.dig_P2 = readS16_LE(BME280_REG_DIG_P2);
 calib_.dig_P3 = readS16_LE(BME280_REG_DIG_P3);
 calib_.dig_P4 = readS16_LE(BME280_REG_DIG_P4);
 calib_.dig_P5 = readS16_LE(BME280_REG_DIG_P5);
 calib_.dig_P6 = readS16_LE(BME280_REG_DIG_P6);
 calib_.dig_P7 = readS16_LE(BME280_REG_DIG_P7);
 calib_.dig_P8 = readS16_LE(BME280_REG_DIG_P8);
 calib_.dig_P9 = readS16_LE(BME280_REG_DIG_P9);

 calib_.dig_H1 = read8(BME280_REG_DIG_H1);
 calib_.dig_H2 = readS16_LE(BME280_REG_DIG_H2);
 calib_.dig_H3 = read8(BME280_REG_DIG_H3);
 calib_.dig_H4 = (read8(BME280_REG_DIG_H4) << 4) 
  | (read8(BME280_REG_DIG_H4+1) & 0xF);
 calib_.dig_H5 = (read8(BME280_REG_DIG_H5+1) << 4)
  | (read8(BME280_REG_DIG_H5) >> 4);
 calib_.dig_H6 = (int8_t)read8(BME280_REG_DIG_H6);
 if(status!=BME280_OK) return false;
 return true;
}

//**************************************************************
//    setter and getter methods
//**************************************************************

//_____set waiting time between two measurements in ms__________
void BME280::setWaitMeasuring(unsigned long wait_ms)
{ waitMeasuring_=wait_ms; }

//_____system status as int_____________________________________
int BME280::getStatus() { return status; }

//_____system status as german text string______________________
String BME280::getsStatus() 
{ 
 switch(status)
 {
  case BME280_OK:
   return String("OK");
  case BME280_ERR_TOO_LONG:
   return String("I2C Sendepuffer ist zu klein für die Daten");
  case BME280_ERR_NACK_ADDR:
   return String("Keine Bestaetigung (ACK) nach Adressbyte");
  case BME280_ERR_NACK_DATA:
   return String("Keine Bestaetigung (ACK) nach Datenbyte");
  case BME280_ERR_OTHER:
   return String("I2C Fehler");
  case BME280_ERR_NUM_BYTES:
   return String("Falsche Anzahl Bytes");
  case BME280_ERR_NO_BYTE:
   return String("Kein Byte empfangen");
  case BME280_ERR_NUM_CHARS:
   return String("Falsche Anzahl an Bytes");
  case BME280_ERR_RANGE:
   return String("Wert ausserhalb des zulaessigen Bereichs");
  case BME280_ERR_NO_MEAS:
   return String("Es wurde noch keine Messung durchgefuehrt");
  case BME280_ERR_ID:
   return String("Fehler beim Einlesen der Sensor-ID");
  case BME280_ERR_RESET:
   return String("Fehler beim Reset des Sensors");
  default:
   return String("Unbekannter Fehler Nummer ")+String(status);
 }
 return String("Schwerwiegender, unbekannter Fehler");
}

//_____get all measured values as float_________________________
int BME280::getValues(float &temperature,  float &humidity, 
                      float &pressure_hPa, float &altitude)
{
 measuring();
 if(status!=BME280_OK)
 {
  temperature =BME280_ERR_FLOAT;
  humidity    =BME280_ERR_FLOAT;
  pressure_hPa=BME280_ERR_FLOAT;
  altitude    =BME280_ERR_FLOAT;
 }
 else
 {
  temperature =BME280_FT(iTmp_);
  humidity    =BME280_FH(iHum_);
  pressure_hPa=BME280_FP(iPre_)/100.0F;
  altitude    =BME280_FA(pressure_hPa, seaLevel_hPa);
 }
 return status;
}

//_____get all measured values as String separated by ,_________
String BME280::getsValues() { return getsValues(","); }

//_____get all measured values as String separated by sep_______
String BME280::getsValues(String sep) {
 float t,h,p,a;
 int d1=1;
 getValues(t, h, p, a);
 String s1=float2String(t,d1,d1)+sep+float2String(h,d1,d1)+sep+
           float2String(p,d1,d1)+sep+float2String(a,d1,d1);
 return s1;
}

//_____get all measured values as String separated by sep_______
// dt=decimals temperature, dh=decimals humidity, ...
String BME280::getsValues(String sep,int dt,int dh,int dp,int da)
{
 float t,h,p,a;
 getValues(t, h, p, a);
 String s1=float2String(t,dt,dt)+sep+float2String(h,dh,dh)+sep+
           float2String(p,dp,dp)+sep+float2String(a,da,da);
 return s1;
}

//_____get all measured values as JSON-Object___________________
String BME280::getsJson() { return getsJson(1,1,1,1); }

//_____get all measured values as JSON-Object___________________
String BME280::getsJson(int dt,int dh,int dp,int da) {
 float t,h,p,a;
 getValues(t, h, p, a);
 String s1="{\"T\":" + float2String(t,dt,dt) +
           ",\"H\":" + float2String(h,dh,dh) +
           ",\"p\":" + float2String(p,dp,dp) +
           ",\"a\":" + float2String(a,da,da) + "}";
 return s1;
}

//_____measure temperature and return it as float_______________
// return: temperature or BME280_ERR_FLOAT (on error)
float BME280::getTemperature()
{ 
 measuring();
 if(status!=BME280_OK) return BME280_ERR_FLOAT;
 return BME280_FT(iTmp_);
}

//_____measure humidity and return it as float__________________
// return: humidity or BME280_ERR_FLOAT (on error)
float BME280::getHumidity()
{ 
 measuring();
 if(status!=BME280_OK) return BME280_ERR_FLOAT;
 return BME280_FH(iHum_);
}

//_____measure pressure and return it as float__________________
// return: pressure or BME280_ERR_FLOAT (on error)
float BME280::getPressure()
{ 
 measuring();
 if(status!=BME280_OK) return BME280_ERR_FLOAT;
 return BME280_FP(iPre_)/100.0F;
}

//_____measure altitude and return it as float__________________
// return: altitude or BME280_ERR_FLOAT (on error)
float BME280::getAltitude()
{ 
 measuring();
 if(status!=BME280_OK) return BME280_ERR_FLOAT;
 float pressure_hPa=BME280_FP(iPre_)/100.0F;
 return BME280_FA(pressure_hPa, seaLevel_hPa);
}

//**************************************************************
//    helper functions
//**************************************************************

//_____read pressure, temperature and humidity from sensor______
// v[0]..v[2] pressure, v[3]..v[5] temperature, v[6]..v[7] humi
void BME280::measuring()
{
 uint32_t value;
 int32_t iP, iT, iH;
 int32_t v1, v2;
 int64_t p1, v3, v4;
 //-----check the delay time between two measurements-----------
 if(millis()<=nextMeasuring_) return;
 nextMeasuring_=millis()+WAIT_MEASURING_MS;
 //-----read all data (3+3+2 = 8 data bytes)--------------------
 Wire.beginTransmission(i2cAddress);
 Wire.write(BME280_REG_PRESDATA);
 status=Wire.endTransmission();
 if(status!=BME280_OK) return;
 Wire.requestFrom(i2cAddress, 8);
 if(Wire.available()!=8) 
 {
  Wire.endTransmission();
  status=BME280_ERR_NUM_BYTES;
  return;
 }
 value=Wire.read();
 value=(value<<8) | Wire.read();
 value=(value<<8) | Wire.read();
 iP=(int32_t)value;
 value=Wire.read();
 value=(value<<8) | Wire.read();
 value=(value<<8) | Wire.read();
 iT=(int32_t)value;
 value=Wire.read();
 value=(value<<8) | Wire.read();
 iH=(int32_t)value;
 //status=Wire.endTransmission();
 //if(status!=BME280_OK) return;
 //-----build temperature value-----------------------------------
 if(iT!=BME280_ERR_T) //value in case temp meas. was disabled
 {
  iT >>= 4;
  v1 = ((((iT>>3) - ((int32_t)calib_.dig_T1 <<1))) *
       ((int32_t)calib_.dig_T2)) >> 11;
  v2 = (((((iT>>4) - ((int32_t)calib_.dig_T1)) *
       ((iT>>4) - ((int32_t)calib_.dig_T1))) >> 12) *
       ((int32_t)calib_.dig_T3)) >> 14;
  iTmp_ = v1 + v2;
 }
 //Serial.print("T="); Serial.print(BME280_FT(iTmp_));
 //-----build pressure value--------------------------------------
 if(iP!=BME280_ERR_P) //value in case pressure meas. was disabled
 {
  iP >>= 4;
  v3 = ((int64_t)iTmp_) - 128000;
  v4 = v3 * v3 * (int64_t)calib_.dig_P6;
  v4 = v4 + ((v3*(int64_t)calib_.dig_P5)<<17);
  v4 = v4 + (((int64_t)calib_.dig_P4)<<35);
  v3 = ((v3 * v3 * (int64_t)calib_.dig_P3)>>8) +
       ((v3 * (int64_t)calib_.dig_P2)<<12);
  v3 = (((((int64_t)1)<<47)+v3))*((int64_t)calib_.dig_P1)>>33;
  if (v3 != 0) {
   p1 = 1048576 - iP;
   p1 = (((p1<<31) - v4)*3125) / v3;
   v3 = (((int64_t)calib_.dig_P9) * (p1>>13) * (p1>>13)) >> 25;
   v4 = (((int64_t)calib_.dig_P8) * p1) >> 19;
   iPre_ = ((p1 + v3 + v4) >> 8) + (((int64_t)calib_.dig_P7)<<4);
  }
 }
 //Serial.print(", P="); Serial.print(BME280_FP(iiPre_));
 //-----build humidity value------------------------------------
 if(iH!=BME280_ERR_H) //value in case humidity meas. was disabled
 {
  v1 = (iTmp_ - ((int32_t)76800));
  v1 = (((((iH << 14) - (((int32_t)calib_.dig_H4) << 20) -
       (((int32_t)calib_.dig_H5)*v1)) + ((int32_t)16384))>>15)*
       (((((((v1 * ((int32_t)calib_.dig_H6)) >> 10) *
       (((v1 * ((int32_t)calib_.dig_H3)) >> 11) + 
       ((int32_t)32768))) >> 10) +
       ((int32_t)2097152))*((int32_t)calib_.dig_H2)+8192)>>14));
  v1 = (v1 - (((((v1 >> 15) * (v1 >> 15)) >> 7) *
       ((int32_t)calib_.dig_H1)) >> 4));
  v1 = (v1 < 0) ? 0 : v1;
  v1 = (v1 > 419430400) ? 419430400 : v1;  
  iHum_=v1;  
 }
 //Serial.print(", H="); Serial.print(BME280_FH(iHum_));
 float sea=SEALEVELPRESSURE_HPA;
 float atmospheric = BME280_FP(iPre_) / 100.0F;
 altitude_= 44330.0 * (1.0 - pow(atmospheric / sea, 0.1903));
 //Serial.print(", h="); Serial.println(altitude_);
}

//_____convert float to String with given decimals_______________
String BME280::float2String(float f, int len, int decimals)
{
 char carray[24];                           // convert helper
 String s1="";                              // help string
 if((len<0)||(len>24)||(decimals<0)) return s1;
 dtostrf(f,len,decimals,carray);            // format float l.d
 s1=String(carray);                         // array to string
 if(len<=decimals) s1.trim();               // remove blanks
 return s1;
}

//**************************************************************
//    helper functions: i2c-access
//**************************************************************

//_____i2c: write 1 byte________________________________________
bool BME280::write8(byte reg, byte value) {
 Wire.beginTransmission(i2cAddress);
 Wire.write(reg);
 Wire.write(value);
 status=Wire.endTransmission();
 if(status!=BME280_OK) return false;
 return true;
}

//_____i2c: read 1 byte_________________________________________
uint8_t BME280::read8(byte reg) {
 Wire.beginTransmission(i2cAddress);
 Wire.write(reg);
 status=Wire.endTransmission();
 if(status!=BME280_OK) return 0xFF;
 Wire.requestFrom(i2cAddress, 1);
 if(Wire.available()!=1) 
 {
  Wire.endTransmission();
  status=BME280_ERR_NUM_BYTES;
  return 0xFF;
 }
 uint8_t value=Wire.read();
 //Wire.endTransmission();
 return value;
}

//_____i2c: read 2 byte_________________________________________
uint16_t BME280::read16(byte reg) {
 uint16_t value;
 Wire.beginTransmission(i2cAddress);
 Wire.write(reg);
 status=Wire.endTransmission();
 if(status!=BME280_OK) return 0xFFFF;
 Wire.requestFrom(i2cAddress, 2);
 if(Wire.available()!=2) 
 {
  Wire.endTransmission();
  status=BME280_ERR_NUM_BYTES;
  return 0xFFFF;
 }
 value=Wire.read();
 value=(value<<8) | Wire.read();
 //Wire.endTransmission();
 return value;
}

//_____i2c: read 3 byte_________________________________________
uint32_t BME280::read24(byte reg) {
 uint32_t value;
 Wire.beginTransmission(i2cAddress);
 Wire.write(reg);
 status=Wire.endTransmission();
 if(status!=BME280_OK) return 0xFFFFFFFF;
 Wire.requestFrom(i2cAddress, 3);
 if(Wire.available()!=3)
 {
  Wire.endTransmission();
  status=BME280_ERR_NUM_BYTES;
  return 0xFFFFFFFF;
 }
 value=Wire.read();
 value=(value<<8) | Wire.read();
 value=(value<<8) | Wire.read();
 //Wire.endTransmission();
 return value;
}

//_____i2c: read 16bit signed___________________________________
int16_t BME280::readS16(byte reg) {
 return (int16_t)read16(reg);
}

//_____i2c: 16bit little endian (low-hiht byte)_________________
uint16_t BME280::read16_LE(byte reg) {
 uint16_t temp = read16(reg);
 return (temp >> 8) | (temp << 8);
}

//_____i2c: 16bit signed little endian (low-hiht byte)__________
int16_t BME280::readS16_LE(byte reg) {
 return (int16_t)read16_LE(reg);
}
