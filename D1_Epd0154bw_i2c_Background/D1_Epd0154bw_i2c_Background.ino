//_____D1_Epd0154bw_i2c_Background.ino________180514-180514_____
// Demo program to display a image on a 2-color
// e-paper display (1,54 inch, 200x200 px, black and white)
// and measure the duration, how long it needs
// Created by Karl Hartinger, May 14, 2018
// Last Change: -
// Released into the public domain.

#include "src/epd/D1_class_Epd_1in54bw.h"
#include "src/epd/D1_class_EpdPainter.h"
#include "src/img/imagedata.h"
#include "src/img/maex200x200.h"

EpdConnection connection(1);                //busyLevel 1"54: 1
Epd_ epd(connection);                       //the ePaperDisplay
EpdPainter epdPainter(epd);                 //print methods

//_____setup____________________________________________________
void setup() {
 Serial.begin(115200); Serial.println();    // init Serial
 //-----try to init e-paper display-----------------------------
 Serial.print("Init e-Paper Display: ");
 if (!epd.init()) {
  Serial.println("init failed. STOP!");
  while(true) { yield(); };                 // endless loop
 }
 Serial.println("INIT OK");                 // init OK
 //-----wait until display ready, print points to Serial--------
 int i=100;
 while(connection.isBusy()&&(i>0))
 {
  delay(10);
  Serial.print(".");
  i--; if(i%30==0) Serial.println(".");
 }
 if(i<=0) Serial.println("Display NOT ready!");
 else Serial.println("Display ready!");
 //-----clear display and print picture (background)------------
long milli=millis();
 epdPainter.clearDisplay();
 //epdPainter.drawBackgroundFromProgmem(IMAGE_DATA);
 epdPainter.drawBackgroundFromProgmem(IMAGE_MAEX200X200);
 epdPainter.display();                      // 2963 ms
 //epdPainter.displayNoWait();                // 1657 ms
Serial.print("\nmillis=");Serial.println((millis()-milli)); 
 Serial.println("\nDisplay finished");
}

void loop() {
 Serial.println("LOOP :) ");
 delay(5000);
}
