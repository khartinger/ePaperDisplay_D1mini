//_____D1_Epd0290bw_font16x8_255.ino__________180421-180616_____
// Demo program to display (ASCII-)characters from 32 to 127 
// and 128 to 255 alternating (10sec) on a 2-color
// e-paper display (2,9 inch, 128x296 px, black and white)
// Created by Karl Hartinger, June 16, 2018
// Last Change 2018-06-16: -
// Released into the public domain.
#include "src/epd/D1_class_Epd_2in9bw.h"
#include "src/epd/D1_class_EpdPainter.h"
#include "src/epd/D1_class_EpdPainterBar.h"
#define S(x) String((char)(x))

EpdConnection connection(D6,D4,D3,D8,1);    //busy,reset,dc,cs,busyLevel
Epd_ epd(connection);                       //the ePaperDisplay
EpdPainter epdPainter(epd);                 //print methods

String s1;
int i,j;
int y, dy;
int charBegin, charEnd, charPerLine;

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
 Serial.print("dy="); Serial.println(dy);
 
 //-----some more display inits---------------------------------
 connection.setWaitIdle(0);                 // 0=dont wait
 //epdPainter.setFont(&Font20x11_255);        // font
 epdPainter.setFont(&Font16x8_255);         // font
 // Font16x8_255: 128/8 => charPerLine=16;
 // Font20x11_255, Font24x11_255: 128/11 => charPerLine=11;
 charPerLine=128/epdPainter.getFontWidth();
 dy=epdPainter.getFontHeight();

 //-----ascii 32...127------------------------------------------
 charBegin=32; charEnd=127;
 y=0;
 epdPainter.clearDisplay();
 for(i=charBegin; i<=charEnd; i=i+charPerLine)
 {
  s1="";
  for(j=0;  j<charPerLine; j++)
  {
   if((i+j)<=charEnd) s1=s1+S(i+j); //String(char(i+j));
  }
  epdPainter.drawStringAt(0,y,s1);
  y=y+dy;
 }
 //-----ascii 128...255-----------------------------------------
 charBegin=128; charEnd=255;
 for(i=charBegin; i<=charEnd; i=i+charPerLine)
 {
  s1="";
  for(j=0;  j<charPerLine; j++)
  {
   if((i+j)<=charEnd) s1=s1+S(i+j); //String(char(i+j));
  }
  epdPainter.drawStringAt(0,y,s1);
  y=y+dy;
 }
 epdPainter.display();

}

void loop() {
/* 
 //-----ascii 32...127------------------------------------------
 charBegin=32; charEnd=127;
 y=0;
 epdPainter.clearDisplay();
 for(i=charBegin; i<=charEnd; i=i+charPerLine)
 {
  s1="";
  for(j=0;  j<charPerLine; j++)
  {
   if((i+j)<=charEnd) s1=s1+S(i+j); //String(char(i+j));
  }
  epdPainter.drawStringAt(0,y,s1);
  y=y+dy;
 }
 epdPainter.display();
 delay(10000);
 
 //-----ascii 128...255-----------------------------------------
 charBegin=128; charEnd=255;
 y=0;
 epdPainter.clearDisplay();
 for(i=charBegin; i<=charEnd; i=i+charPerLine)
 {
  s1="";
  for(j=0;  j<charPerLine; j++)
  {
   if((i+j)<=charEnd) s1=s1+S(i+j); //String(char(i+j));
  }
  epdPainter.drawStringAt(0,y,s1);
  y=y+dy;
 }
 epdPainter.display();
 delay(10000);
*/
}
