//_____D1_class_EpdI2cConnection.cpp__________180514-180514_____
// D1 mini class to connect an e-paper display (EPD) via 
// i2c-spi-bridge SC18IS602B to a D1 mini.
// Hardware: (1) WeMos D1 mini
//           (2) IS602 shield (self made)
//
// Created by Karl Hartinger, May 14, 2018.
// Last modified: -
// Released into the public domain.
#include "D1_class_EpdI2cConnection.h"

//**************************************************************
//    constructor & co
//**************************************************************

//_____constructor with default values__________________________
EpdConnection::EpdConnection() {
 i2cAddress=IS602_ADDR;
 wait_idle_sec=CONN_WAIT_IDLE;
 epd_busy_level=EPD_BUSY_LEVEL;
 connect();
}

//_____constructor with busy level______________________________
EpdConnection::EpdConnection(int epdBusyLevel) {
 i2cAddress=IS602_ADDR;
 wait_idle_sec=CONN_WAIT_IDLE;
 epd_busy_level=epdBusyLevel;
 connect();
} 

//**************************************************************
//     setter and getter methods
//**************************************************************

void EpdConnection::setWaitIdle(int sec) { wait_idle_sec=sec; }

void EpdConnection::setEpdBusyLevel(int level) { epd_busy_level=level; }

bool EpdConnection::isBusy() {
 unsigned char gpioIn=read8(IS602_ID_GPIO_READ);
 if(((gpioIn&8)>>3)==epd_busy_level) return true;
 return false;
}

//**************************************************************
//     working methods
//**************************************************************

//_____called by constructor!___________________________________
bool EpdConnection::connect() {
 //-----config i2c-spi-bridge SC18IS602B for e-paper display----
 Wire.begin();                    //(4,5): SDA on 4, SCL on 5
 //-----configure SPI: MSByte first, mode 0, 1843kHz------------
 if(!write8(IS602_ID_CONFIG, IS602_CONFIG))
 { status=IS602_ERR_CONFIG; return false; }
 //-----enable GPIO (0x0E=1110): ss3|ss2|ss1|ss0=no GPIO--------
 if(!write8(IS602_ID_GPIO_ENABLE, IS602_GPIO_ENABLE))
 { status=IS602_ERR_IO_ENABLE; return false; }
 //-----configure types of GPIO---------------------------------
 if(!write8(IS602_ID_GPIO_CONFIG, IS602_GPIO_CONFIG))
 { status=IS602_ERR_IO_CONFIG; return false; }
 //-----set GPIO pins to no reset, command mode-----------------
 if(!write8(IS602_ID_GPIO_WRITE, IS602_CMD)) return false;
 return true;
}

bool EpdConnection::sendCommand(unsigned char command) {
 if(!write8(IS602_ID_GPIO_WRITE, IS602_CMD)) return false;
 if(!write8(IS602_ID_WRITE0, command)) return false;
 delay(1);
 return true;
}

bool EpdConnection::sendData(unsigned char data) {
 if(!write8(IS602_ID_GPIO_WRITE, IS602_DATA)) return false;
 if(!write8(IS602_ID_WRITE0, data)) return false;
 delay(1);
 return true;
}

bool EpdConnection::sendDatablock(const unsigned char* image_buffer,
 int image_width, int xmax, int ymax) {
 int w=image_width/8;
 int num=0;
 if(!write8(IS602_ID_GPIO_WRITE, IS602_DATA)) return false;
 for (int j = 0; j < ymax; j++) {
  for (int i = 0; i < xmax / 8; i++) {
   if(!write8(IS602_ID_WRITE0, image_buffer[i + j * w]))  return false;
  }
 }
 return true;
}

//_____reset display____________________________________________
// return: true=success, false=error
bool EpdConnection::resetDisplay()
{
 if(!write8(IS602_ID_GPIO_WRITE, IS602_RST)) return false;
 delay(200);
 if(!write8(IS602_ID_GPIO_WRITE, IS602_CMD))
 { delay(200); return false; }
 delay(200);
 return true;
}

//_____wait until finished (idle) with default value____________
// return: true=epd ready, false=timeout
bool EpdConnection::waitUntilIdle() {
 return waitUntilIdle(wait_idle_sec);
}

//_____wait until finished (idle) OR timeout (return false)_____
// return: true=epd ready, false=timeout
bool EpdConnection::waitUntilIdle(int sec) {
 int i=10*sec;
 while(isBusy() && (i>0)) {
  delay(100);
  i--;
 }      
 if(i<=0) return false;
 return true;
}

//**************************************************************
//    helper methods: i2c-access
//**************************************************************

//_____i2c: write 1 byte________________________________________
bool EpdConnection::write8(unsigned char reg, unsigned char value) {
 Wire.beginTransmission(i2cAddress);
 Wire.write(reg);
 Wire.write(value);
 status=Wire.endTransmission();
 if(status!=IS602_OK) return false;
 return true;
}

//_____i2c: read 1 byte_________________________________________
uint8_t EpdConnection::read8(unsigned char reg) {
 Wire.beginTransmission(i2cAddress);
 Wire.write(reg);
 status=Wire.endTransmission();
 if(status!=IS602_OK) return 0xFF;
 Wire.requestFrom(i2cAddress, 1);
 if(Wire.available()!=1) 
 {
  Wire.endTransmission();
  status=IS602_ERR_NUM_BYTES;
  return 0xFF;
 }
 uint8_t value=Wire.read();
 //Wire.endTransmission();
 return value;
}