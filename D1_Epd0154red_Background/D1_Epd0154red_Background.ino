//_____D1__Epd0154red_Background.ino__________180401-180413_____
// Demo program to display text and shapes on a 3-color
// e-paper display (1,54 inch, 200x200 px, black, white, red)

#include "libs/D1_class_Epd_1in54red.h"
#include "libs/D1_class_EpdPainter.h"
#include "imagedata3.h"

EpdConnection connection(D6,D4,D3,D8,0);    //busy,reset,dc,cs,busyLevel
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
 epdPainter.drawBackgroundFromProgmem(IMAGE_BLACK, IMAGE_RED);
 epdPainter.display();
 Serial.println("END");
}

void loop() {
 Serial.println("LOOP :) ");
 delay(5000);
}
