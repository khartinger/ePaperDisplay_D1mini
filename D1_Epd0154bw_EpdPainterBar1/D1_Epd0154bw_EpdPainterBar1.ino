//_____D1_Epd0154bw_EpdPainterBar1.ino________180608-180608_____
// Test bar graphics on a 
// 2-color e-paper display (1,54 inch, 200x200px, black, white).
// Created by Karl Hartinger, June 08, 2018,
// Last Change: none
// Released into the public domain.

#include "src/epd/D1_class_Epd_1in54bw.h"
#include "src/epd/D1_class_EpdPainter.h"
#include "src/epd/D1_class_EpdPainterBar.h"

EpdConnection connection(D6,D4,D3,D8,1);    //busy,reset,dc,cs,busyLevel
Epd_          epd(connection);              //the ePaperDisplay
EpdPainter    epdPainter(epd);              //print methods

EpdPainterBar bar(&epdPainter);             //default bar
EpdPainterBar bar8(&epdPainter);            // user bar 8px text
EpdPainterBar barX(&epdPainter);            // user bar text

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
  //-----draw default bar---------------------------------------
 bar.drawFrame();
 bar.drawScale();
 bar.drawBar(10);
 //-----bar with scale in Font8, 2 decimals, left side----------
 int x0=150, dx=20;
 int y0=100, dy=90;
 bar8.setParams(x0,y0,x0+dx,y0+dy, 4,1, 0.0,1.0,0.25);
 bar8.drawFrame();
 bar8.drawScale(&Font8,"4.2",BAR_SCALE_LEFT|BAR_SCALE_Y_FRAMELIMIT);
 bar8.drawBar(0.7);
 //-----bar with text scaling-----------------------------------
 x0=5, dx=30;
 y0=145, dy=50;
 epdPainter.setFont(&Font12);               // font
 barX.setParams(x0,y0,x0+dx,y0+dy, 4,1, 0,4,1);
 barX.drawFrame();
 int xright=40;
 int y=barX.getY(4) - Font12.Height/2;
 epdPainter.drawStringAt(xright,y,"full");
 y=barX.getY(2) - Font12.Height/2;
 epdPainter.drawStringAt(xright,y,"half");
 y=barX.getY(0) - Font12.Height/2;
 epdPainter.drawStringAt(xright,y,"empty");
 barX.drawBar(3);
 //-----display buffer------------------------------------------
 epdPainter.display();                      // display value
}

//_____loop_____________________________________________________
void loop() {
}

