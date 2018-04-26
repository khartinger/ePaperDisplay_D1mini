//_____D1_Epd0154bw_font24x11_specials.ino____180424-180424_____
// Demo program to display special characters on a 2-color
// e-paper display (1,54 inch, 200x200 px, black amd white)
#include "libs/D1_class_Epd_1in54bw.h"
#include "libs/D1_class_EpdPainter.h"
#define S(x) String((char)(x))

EpdConnection connection(D6,D4,D3,D8,1);    //busy,reset,dc,cs,busyLevel
Epd_ epd(connection);                       //the ePaperDisplay
EpdPainter epdPainter(epd);                 //print methods

String s1;
int y, dy=Font24x11_255.Height, dy2=dy*1.5;

String _pi=String((char)227);
String _root=String((char)251);

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
 epdPainter.clearDisplay();
 //-----text with special chars---------------------------------
 y=0;
 s1="German Specials:";
 epdPainter.drawStringAt(0,y,s1);
 y=y+dy;
 epdPainter.drawLine(0,y,s1.length()*Font24x11_255.Width,y); 
 y=y+4;
 s1="äöü ÄÖÜ ß";
 s1=epdPainter.utf8ToEpd(s1);
 epdPainter.drawStringAt(0,y,s1);
 //-----text with special chars 2-------------------------------
 y=y+dy2;
 s1="Special Keys:";
 epdPainter.drawStringAt(0,y,s1);
 y=y+dy;
 epdPainter.drawLine(0,y,s1.length()*Font24x11_255.Width,y); 
 y=y+4;
 s1=S(241)+"5°C 2µA x²=4 1€";
 s1=epdPainter.utf8ToEpd(s1);
 epdPainter.drawStringAt(0,y,s1);
 //-----draw char value directly--------------------------------
 y=y+dy2;
 s1="Direct Printing:";
 epdPainter.drawStringAt(0,y,s1); 
 y=y+dy;
 epdPainter.drawLine(0,y,s1.length()*Font24x11_255.Width,y); 
 y=y+4;
 s1="2"+S(250)+_pi+"*"+_root+"3 Mag."+S(166);
 epdPainter.drawStringAt(0,y,s1);
 y=y+dy;
 //-----display buffer------------------------------------------
 epdPainter.display();
 Serial.println("END");
}

void loop() {
 Serial.println("LOOP :) ");
 delay(5000);
}
