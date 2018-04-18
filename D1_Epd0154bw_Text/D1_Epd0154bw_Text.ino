//_____D1_oop37_Epd0154bw_Text.ino____________180401-180413_____
// Demo program to display text with different size on a 2-color
// e-paper display (1,54 inch, 200x200 px, black amd white)

#include "libs/D1_class_Epd_1in54bw.h"
#include "libs/D1_class_EpdPainter.h"

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
 //-----clear display buffer and print text---------------------
 epdPainter.clearDisplay();
 int y=0;
 epdPainter.drawLine(0,y,199,y); y=y+1; //----------------------
 epdPainter.setFont(&Font8);
 epdPainter.drawStringAt(0,y,"Font8g5|40char|67890abcdefghijklmnopqrst");
 y=y+8; 
 epdPainter.drawLine(0,y,199,y); y=y+1; //----------------------
 epdPainter.setFont(&Font12);
 epdPainter.drawStringAt(0,y,"Font12g7|28char|7890ABCDEFGH");
 y=y+12;
 epdPainter.drawLine(0,y,199,y); y=y+1; //----------------------
 epdPainter.setFont(&Font16);
 epdPainter.drawStringAt(0,y,"Font16g11|18char|89");
 y=y+16; epdPainter.drawLine(0,y,199,y); y=y+1; //--------------
 epdPainter.setFont(&Font20);
 epdPainter.drawStringAt(0,y,"Font20g14|14char|");
 y=y+20; epdPainter.drawLine(0,y,199,y); y=y+1; //--------------
 epdPainter.setFont(&Font24);
 epdPainter.drawStringAt(0,y,"Font24g17|12char");
 y=y+24; epdPainter.drawLine(0,y,199,y); y=y+1; //--------------
 //-----big text------------------------------------------------
 epdPainter.drawBigLine(0,y,199,y); y=y+2; 
 epdPainter.setFont(&Font16);
 epdPainter.drawBigStringAt(0,y,"Big16|9char");
 y=y+2*16-4; epdPainter.drawLine(0,y,199,y); y=y+1; //----------
 epdPainter.setFont(&Font20);
 epdPainter.drawBigStringAt(0,y,"Big20|7char");
 y=y+2*20-4; epdPainter.drawLine(0,y,199,y); y=y+1; //----------
 epdPainter.setFont(&Font24);
 epdPainter.drawBigStringAt(0,y,"Big24|6");
 y=y+2*24-4; epdPainter.drawLine(0,y,199,y); //-----------------
 //-----display buffer------------------------------------------
 epdPainter.display();
 Serial.println("END");
}

void loop() {
 Serial.println("LOOP :) ");
 delay(5000);
}
