//_____D1_class_EpdConnection.cpp_____________180401-180401_____
// D1 mini class to connect a e-paper display via spi and gpios
// to a D1 mini.
#include "D1_class_EpdConnection.h"

//**************************************************************
//    constructor & co
//**************************************************************

//_____constructor with default values__________________________
EpdConnection::EpdConnection() {
 busy_pin =BUSY_PIN;
 reset_pin=RST_PIN;
 dc_pin   =DC_PIN;
 cs_pin   =CS_PIN;	
 wait_idle_sec=CONN_WAIT_IDLE;
 epd_busy_level=EPD_BUSY_LEVEL;
 connect();
}

//_____constructer to set some spi/e-paper display pins_________
EpdConnection::EpdConnection(int busyPin, int resetPin,
  int dcPin, int csPin) {
 if(busyPin>=0)  busy_pin=busyPin; else busy_pin=BUSY_PIN;
 if(resetPin>=0) reset_pin=resetPin; else reset_pin=RST_PIN;
 if(dcPin>=0)    dc_pin=dcPin; else dc_pin=DC_PIN;
 if(csPin>=0)    cs_pin=csPin; else cs_pin=CS_PIN;
 wait_idle_sec=CONN_WAIT_IDLE;
 epd_busy_level=EPD_BUSY_LEVEL;
 connect();
}

//_____constructer to set some spi/e-paper display pins_________
EpdConnection::EpdConnection(int busyPin, int resetPin,
  int dcPin, int csPin,  int epdBusyLevel) {
 if(busyPin>=0)  busy_pin=busyPin; else busy_pin=BUSY_PIN;
 if(resetPin>=0) reset_pin=resetPin; else reset_pin=RST_PIN;
 if(dcPin>=0)    dc_pin=dcPin; else dc_pin=DC_PIN;
 if(csPin>=0)    cs_pin=csPin; else cs_pin=CS_PIN;
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
 if(digitalRead(busy_pin)==epd_busy_level) return true;
 return false;
}

//**************************************************************
//     working methods
//**************************************************************

bool EpdConnection::connect() {
 pinMode(busy_pin, INPUT);
 pinMode(reset_pin, OUTPUT);
 pinMode(dc_pin, OUTPUT);
 pinMode(cs_pin, OUTPUT);
 SPI.beginTransaction(SPISettings(2000000, MSBFIRST, SPI_MODE0));
 SPI.begin();
 //-----if D6 is not used as MISO-------------------------------
 if(busy_pin==D6)  pinMode(busy_pin, INPUT);
 if(reset_pin==D6) pinMode(reset_pin, OUTPUT);
 if(dc_pin==D6) pinMode(dc_pin, OUTPUT);
 if(cs_pin==D6) pinMode(cs_pin, OUTPUT);
 return true;
}

void EpdConnection::sendCommand(unsigned char command) {
 digitalWrite(dc_pin, LOW);
 spiTransfer(command);
}

void EpdConnection::sendData(unsigned char data) {
 digitalWrite(dc_pin, HIGH);
 spiTransfer(data);
}

void EpdConnection::sendDatablock(const unsigned char* image_buffer,
 int image_width, int xmax, int ymax) {
 spiTransferDatablock(image_buffer, image_width, xmax, ymax);
}

//_____reset display____________________________________________
// return: true=success, false=error
bool EpdConnection::resetDisplay()
{
 digitalWrite(reset_pin, LOW);
 delay(200);
 digitalWrite(reset_pin, HIGH);
 delay(200);
 return true;
}

//_____wait until finished (idle) with default value____________
bool EpdConnection::waitUntilIdle() {
 return waitUntilIdle(wait_idle_sec);
}

//_____wait until finished (idle) OR timeout (return false)_____
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
//     Protected: spi access
//**************************************************************

void EpdConnection::spiTransfer(unsigned char data) {
 digitalWrite(cs_pin, LOW);
 SPI.transfer(data);
 digitalWrite(cs_pin, HIGH);
}

void EpdConnection::spiTransferDatablock(
    const unsigned char* image_buffer, int image_width,
    int xmax, int ymax) {
 int w=image_width/8;
 digitalWrite(dc_pin, HIGH);                // send sendData
 digitalWrite(cs_pin, LOW);                 // select spi device
 for (int j = 0; j < ymax; j++) {
  for (int i = 0; i < xmax / 8; i++) {
   SPI.transfer(image_buffer[i + j * w]);   
  }
 }
 digitalWrite(cs_pin, HIGH);                // unselect spi device
}
