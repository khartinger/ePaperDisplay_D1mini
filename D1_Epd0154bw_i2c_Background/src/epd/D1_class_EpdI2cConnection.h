//_____D1_class_EpdI2cConnection.h____________180514-180514_____
// D1 mini class to connect an e-paper display (EPD) via 
// i2c-spi-bridge SC18IS602B to a D1 mini.
// Hardware: (1) WeMos D1 mini
//           (2) IS602 shield (self made)
//
// Connection SC18IS602B - EPD Module 
// 12 3V3    --> 8 3V3
//  4 GND    --- 7 GND
//  6 MOSI   --> 6 DIN
// 11 SPICLK --> 5 CLK
//  1 SS0    --> 4 CS
//  2 SS1    --> 3 DC
// 10 SS2    --> 2 RST
// 13 SS3    <-- 1 BUSY
// Created by Karl Hartinger, May 14, 2018.
// Last modified: -
// Released into the public domain.
#ifndef D1_CLASS_EPDI2CCONNECTION_H
#define D1_CLASS_EPDI2CCONNECTION_H
#include "Arduino.h"              // 
#include "Wire.h"                 // I2C
//#include <SPI.h>

//-----default values-------------------------------------------
#define CONN_WAIT_IDLE        8   //wait until idle in sec
#define EPD_BUSY_LEVEL        1   //2color 1, 3color 0

#define IS602_ADDR           0x28
//-----SC18IS602B register ("ID Function")----------------------
#define IS602_ID_CONFIG      0xF0
#define IS602_ID_GPIO_ENABLE 0xF6
#define IS602_ID_GPIO_CONFIG 0xF7
#define IS602_ID_GPIO_WRITE  0xF4
#define IS602_ID_GPIO_READ   0xF5
#define IS602_ID_WRITE0      0x01
//-----SC18IS602B register content------------------------------
#define IS602_CONFIG         0x00
#define IS602_GPIO_ENABLE    0x0E
#define IS602_GPIO_CONFIG    0x94
#define IS602_CMD            0x04 //6=data, 4=command
#define IS602_DATA           0x06 //6=data, 4=command
#define IS602_RST            0x00 //0=reset, 4=HIGH=OK
//-----status---------------------------------------------------
#define IS602_OK             0    // success
#define IS602_ERR_TOO_LONG   1    // i2c data too long to fit in transmit buffer 
#define IS602_ERR_NACK_ADDR  2    // i2c NACK transmit address
#define IS602_ERR_NACK_DATA  3    // i2c NACK transmit data
#define IS602_ERR_OTHER      4    // i2c other error
#define IS602_ERR_NUM_BYTES  5    // i2c wrong number of bytes
#define IS602_ERR_NO_BYTE    6    // no byte received
#define IS602_ERR_NUM_CHARS  7    // wrong number of chars
#define IS602_ERR_CONFIG     8    // error while config
#define IS602_ERR_IO_ENABLE  9    // error while gpio enable
#define IS602_ERR_IO_CONFIG  10   // error while gpio config

class EpdConnection {
 //-----properties----------------------------------------------
 protected:
  int i2cAddress;                 // i2c address
  int status;                     // state of system
  int wait_idle_sec;
  int epd_busy_level;
 //-----constructor & co----------------------------------------
 public:
  EpdConnection();
  EpdConnection(int epdBusyLevel);            // NEW 180508
  //-----setter and getter methods------------------------------
  void setWaitIdle(int sec);
  void setEpdBusyLevel(int level);
  bool isBusy();
  int  getStatus() { return status; }
  //-----working methods----------------------------------------
  bool connect();
  bool disconnect() { return true; };
  bool sendCommand(unsigned char command);
  bool sendData(unsigned char data);
  bool sendDatablock(const unsigned char* image_buffer,
       int image_width, int xmax, int ymax);
  bool resetDisplay();
  bool waitUntilIdle();
  bool waitUntilIdle(int sec);
  //-----helper methods: i2c-access-----------------------------
  bool     write8(unsigned char reg,
                  unsigned char value);     // write 1 byte
  uint8_t  read8(unsigned char reg);        // read 1 byte
};
#endif
