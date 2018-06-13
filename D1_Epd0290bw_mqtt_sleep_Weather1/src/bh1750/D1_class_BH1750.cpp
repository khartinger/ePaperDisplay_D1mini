//_____D1_class_BH1750.cpp____________________170414-180519_____
// D1 mini class for i2c digital light sensor BH1750.
// Default i2c address is 0x23 (other option 0x5C).
// Created by Karl Hartinger, April 14, 2017
// Last modified 2018-05-19: Move class files to /src/...
// Released into the public domain.
#include "D1_class_BH1750.h"      // 

//_____constructor (default 0x23)_______________________________
BH1750::BH1750() { i2cAddress=BH1750_ADDR; setup(); }

//_____constructor with i2c Address_____________________________
BH1750::BH1750(int i2c_address) {
 i2cAddress=i2c_address; 
 setup(); 
}

//_____setup device BH1750______________________________________
void BH1750::setup()
{
 //-----wake up sensor------------------------------------------
 Wire.begin();                    //(4,5): SDA on 4, SCL on 5
 Wire.beginTransmission(i2cAddress);
 Wire.write(0x01);                // power on, waiting for cmd
 status=Wire.endTransmission();
 if(status!=BH1750_OK) return;
 //-----Reset Data register-------------------------------------
 Wire.beginTransmission(i2cAddress);
 Wire.write(0x07);                // reset data register
 status=Wire.endTransmission();   
 if(status!=BH1750_OK) return;
 measuring();
}

//_____system status____________________________________________
// 0: success
// 1: data too long to fit in transmit buffer
// 2: received NACK on transmit of address
// 3: received NACK on transmit of data
// 4: other error 
// 9: first status after setup
int BH1750::getStatus() { return status; }

//_____get temperature as double________________________________
double BH1750::getBrightness() { return(getBi()); }

//_____get temperature as integer_______________________________
long BH1750::getBi() { 
 measuring();
 return brightness; 
}

//_____read temperature and humidity from sensor________________
void BH1750::measuring()
{
 //-----set Continuously H-Resolution Mode, Resolution 1 lx-----
 Wire.beginTransmission(i2cAddress);
 Wire.write(0x10);                // Continuously H-Resol. Mode
 status=Wire.endTransmission();
 if(status!=BH1750_OK)            // try a second time
 {
  delay(120);                     //Measurement Time(data sheet)
  Wire.beginTransmission(i2cAddress);
  Wire.write(0x10);               // Continuously H-Resol. Mode
  status=Wire.endTransmission();
  if(status!=BH1750_OK) return;
 }
 //-----read 8 bytes from sensor (6xdata, 2xcrc)----------------
 Wire.requestFrom(i2cAddress, 2);
 if(Wire.available())             // (read) data bytes ready?
 {
  int temp_=Wire.read()<<8;
  temp_|=Wire.read();
  brightness=temp_;
 }
 status=Wire.endTransmission(); 
}
