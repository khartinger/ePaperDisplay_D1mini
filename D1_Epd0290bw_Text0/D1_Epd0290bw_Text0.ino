//_____D1_Epd0290bw_Text0.ino_________________180415-180415_____
// Demo program to display text with different size on a 2-color
// e-paper display: 2,9 inch, 128x296 px, black/white, portrait
#include "libs/D1_class_Epd_2in9bw.h"
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
 epdPainter.drawStringAt(0,y,"Font8g5|25char|67890abcdefghijklmnopqrst");
 y=y+8; 
 epdPainter.drawLine(0,y,199,y); y=y+1; //----------------------
 epdPainter.setFont(&Font12);
 epdPainter.drawStringAt(0,y,"Font12g7|18char|7890ABCDEFGH");
 y=y+12;
 epdPainter.drawLine(0,y,199,y); y=y+1; //----------------------
 epdPainter.setFont(&Font16);
 epdPainter.drawStringAt(0,y,"Font16g11|11char|89");
 y=y+16; epdPainter.drawLine(0,y,199,y); y=y+1; //--------------
 epdPainter.setFont(&Font20);
 epdPainter.drawStringAt(0,y,"Font20g14|9char|");
 y=y+20; epdPainter.drawLine(0,y,199,y); y=y+1; //--------------
 epdPainter.setFont(&Font24);
 epdPainter.drawStringAt(0,y,"Font24g17|7char");
 y=y+24; epdPainter.drawLine(0,y,199,y); y=y+1; //--------------
 //-----big text------------------------------------------------
 epdPainter.drawBigLine(0,y,199,y); y=y+2; 
 epdPainter.setFont(&Font16);
 epdPainter.drawBigStringAt(0,y,"Big16|5char");
 y=y+2*16-4; epdPainter.drawLine(0,y,199,y); y=y+1; //----------
 epdPainter.setFont(&Font20);
 epdPainter.drawBigStringAt(0,y,"Big20|4char");
 y=y+2*20-4; epdPainter.drawLine(0,y,199,y); y=y+1; //----------
 epdPainter.setFont(&Font24);
 epdPainter.drawBigStringAt(0,y,"Big24|4");
 y=y+2*24-4; epdPainter.drawLine(0,y,199,y); //-----------------
 //-----display buffer------------------------------------------
 epdPainter.display();
 Serial.println("END");
}

void loop() {
 Serial.println("LOOP :) ");
 delay(5000);
}
