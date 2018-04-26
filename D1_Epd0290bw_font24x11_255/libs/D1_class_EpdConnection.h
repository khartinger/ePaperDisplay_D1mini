//_____D1_class_EpdConnection.h_______________180401-180401_____
// D1 mini class to connect a e-paper display via spi and gpios
// to a D1 mini.
#ifndef D1_CLASS_EPDISPLAYCONNECTION_H
#define D1_CLASS_EPDISPLAYCONNECTION_H
#include "Arduino.h"                   // 
#include <SPI.h>

//-----Default pin definition-----------------------------------
#define BUSY_PIN        D6   //orig. D2, RX after prog possible
#define RST_PIN         D4
#define DC_PIN          D3
#define CS_PIN          D8
#define CONN_WAIT_IDLE   8   //wait until idle in sec
#define EPD_BUSY_LEVEL   0   //2color 1, 3color 0

class EpdConnection {
 //-----properties----------------------------------------------
 protected:
  int busy_pin;
  int reset_pin;
  int dc_pin;
  int cs_pin;
  int wait_idle_sec;
  int epd_busy_level;
 //-----constructor & co----------------------------------------
 public:
  EpdConnection();
  EpdConnection(int busyPin, int resetPin,
                int dcPin, int csPin);
  EpdConnection(int busyPin, int resetPin,
                int dcPin, int csPin, int epdBusyLevel);
  //-----setter and getter methods------------------------------
  void setWaitIdle(int sec);
  void setEpdBusyLevel(int level);
  bool isBusy();
  //-----working methods----------------------------------------
  bool connect();
  bool disconnect() { return true; };
  void sendCommand(unsigned char command);
  void sendData(unsigned char data);
  void sendDatablock(const unsigned char* image_buffer,
       int image_width, int xmax, int ymax);
  bool resetDisplay();
  bool waitUntilIdle();
  bool waitUntilIdle(int sec);
 //-----Protected: spi access-----------------------------------
 protected:
  void spiTransfer(unsigned char data);
  void spiTransferDatablock(const unsigned char* image_buffer,
       int image_width, int xmax, int ymax);

};
#endif
