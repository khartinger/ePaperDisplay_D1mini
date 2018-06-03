//_____D1_Epd0290bw_EpdPainterBar1.ino________180603-180603_____
// Test bar graphics on a 
// 2-color e-paper display (2,9 inch, 128x296 px, black, white).
// Created by Karl Hartinger, June 03, 2018,
// Last Change: none
// Released into the public domain.

#include "src/epd/D1_class_Epd_2in9bw.h"
#include "src/epd/D1_class_EpdPainter.h"
#include "src/epd/D1_class_EpdPainterBar.h"

EpdConnection connection(D6,D4,D3,D8,1);    //busy,reset,dc,cs,busyLevel
Epd_          epd(connection);              //the ePaperDisplay
EpdPainter    epdPainter(epd);              //print methods

EpdPainterBar bar(&epdPainter);             //default bar
EpdPainterBar bar2(&epdPainter);            // user bar

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
 //-----some more display inits---------------------------------
 connection.setWaitIdle(0);                 // 0=dont wait
 epdPainter.setFont(&Font24x11_255);        // font
 epdPainter.setRotation(ROTATE_0);          // y=long side
 epdPainter.clearDisplay();                 // clear buffer
 //-----set bar parameter---------------------------------------
 bar2.setParams(44,150,84,290, 4,1, -20.0,50.0,10.0);
 //-----draw bar frames (no bar)-------------------------------- 
 bar.drawFrame();
 bar.setFont(&Font12);
 bar.drawScale();
 bar2.drawFrame();
 bar2.setFont(&Font20);
 bar2.drawScale(BAR_SCALE_LEFT | BAR_SCALE_Y_FRAMELIMIT);
 //-----draw bar with specified value---------------------------
 bar.drawBar(10);
 bar2.drawBar(45);
 epdPainter.display();                      // display value
}

//_____loop_____________________________________________________
void loop() {
}

