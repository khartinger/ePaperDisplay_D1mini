//_____D1_Epd0290bw_font24x11_255.ino_________180421-180424_____
// Demo program to display (ASCII-)characters from 32 to 127 
// and 128 to 255 alternating (10sec) on a 2-color
// e-paper display (2,9 inch, 128x296 px, black and white)
#include "libs/D1_class_Epd_2in9bw.h"
#include "libs/D1_class_EpdPainter.h"
#define S(x) String((char)(x))

EpdConnection connection(D6,D4,D3,D8,1);    //busy,reset,dc,cs,busyLevel
Epd_ epd(connection);                       //the ePaperDisplay
EpdPainter epdPainter(epd);                 //print methods

String s1;
int i,j;
int y, dy=Font24x11_255.Height;
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
 //-----some more display inits---------------------------------
 connection.setWaitIdle(0);                 // 0=dont wait
 epdPainter.setFont(&Font24x11_255);        // font
}

void loop() {
 //-----ascii 32...127------------------------------------------
 charBegin=32; charEnd=127; charPerLine=11;
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
 Serial.println("LOOP :) ");
 
}
