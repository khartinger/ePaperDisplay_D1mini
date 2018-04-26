//_____D1_Epd0154bw_pmfont24x11_255.ino_______180421-180426_____
// Demo program to display characters from 32 to 255 from  
// program memory alternating (10sec) on a 2-color
// e-paper display (1,54 inch, 200x200 px, black and white)
#include "libs/D1_class_Epd_1in54bw.h"
#include "libs/D1_class_EpdPainter.h"
#define S(x) String((char)(x))

EpdConnection connection(D6,D4,D3,D8,1);    //busy,reset,dc,cs,busyLevel
Epd_ epd(connection);                       //the ePaperDisplay
EpdPainter epdPainter(epd);                 //print methods

String s1;
int i,j;
int y, dy=pmFont24x11_255.Height;
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
 //epdPainter.setFont(&Font16);               // font
 epdPainter.setFont(&pmFont24x11_255);      // font
}

void loop() {
 //-----ascii 32...127------------------------------------------
 charBegin=32; charEnd=127; charPerLine=16;
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
