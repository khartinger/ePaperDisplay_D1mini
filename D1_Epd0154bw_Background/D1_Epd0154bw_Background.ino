//_____D1_Epd0154bw_Background.ino____________180401-180505_____
// Demo program to display a image on a 2-color
// e-paper display (1,54 inch, 200x200 px, black and white)
// Created by Karl Hartinger, April 01, 2018,
// Last Change May 05, 2018: Move class files to /src/...
// Released into the public domain.

#include "src/epd/D1_class_Epd_1in54bw.h"
#include "src/epd/D1_class_EpdPainter.h"
//#include "src/img/imagedata.h"
#include "src/img/maex200x200.h"

EpdConnection connection(D6,D4,D3,D8,1);    //busy,reset,dc,cs,busyLevel
Epd_ epd(connection);                       //the ePaperDisplay
EpdPainter epdPainter(epd);                 //print methods

//_____setup____________________________________________________
void setup() {
 Serial.begin(115200); Serial.println();    // init Serial
 //-----try to init e-paper display-----------------------------
 Serial.print("Init e-Paper Display: ");
 if (!epd.init()) {
  Serial.print("init failed. STOP!");
  while(true) { yield(); };                 // endless loop
 }
 Serial.println("INIT OK");                 // init OK
 //-----wait until display ready, print points to Serial--------
 int i=100;
 while(connection.isBusy()&&(i>0))
 {
  delay(100);
  Serial.print(".");
  i--; if(i%30==0) Serial.println(".");
 }
 if(i<=0) Serial.println("\nDisplay not ready!");
 //-----clear display and print picture (background)------------
 epdPainter.clearDisplay();
 //epdPainter.drawBackgroundFromProgmem(IMAGE_DATA);
 epdPainter.drawBackgroundFromProgmem(IMAGE_MAEX200X200);
 epdPainter.display();
 Serial.println("END");
}

void loop() {
 Serial.println("LOOP :) ");
 delay(5000);
}
