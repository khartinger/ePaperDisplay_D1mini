//_____D1_Epd0290bw_Text90.ino________________180415-180415_____
// Demo program to display text with different size on a 2-color
// e-paper display: 2,9 inch, 128x296 px, black/white, landscape
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
 //-----change rotation-----------------------------------------
 epdPainter.setRotation(ROTATE_90);
}

void loop() {
 //-----clear display buffer and print text---------------------
 int y=10;
 epdPainter.clearDisplay();
 epdPainter.drawLine(0,y,295,y); y=y+2; //----------------------
 epdPainter.setFont(&Font8);
 epdPainter.drawStringAt(0,y,"Font8g5|25char|67890abcdefghijABCDEFGHIJ1234567890klmnopqrst");
 y=y+8+1; 
 epdPainter.drawLine(0,y,295,y); y=y+2; //----------------------
 epdPainter.setFont(&Font12);
 epdPainter.drawStringAt(0,y,"Font12g7|18char|7890abcdefghijABCDEFGHIJklmn");
 y=y+12+1;
 epdPainter.drawLine(0,y,295,y); y=y+2; //----------------------
 epdPainter.setFont(&Font16);
 epdPainter.drawStringAt(0,y,"Font16g11|27char|890abcdefgh");
 y=y+16+1; epdPainter.drawLine(0,y,295,y); y=y+2; //--------------
 epdPainter.setFont(&Font20);
 epdPainter.drawStringAt(0,y,"Font20g14|21char|890ab");
 y=y+20+1; epdPainter.drawLine(0,y,295,y); y=y+2; //--------------
 epdPainter.setFont(&Font24);
 epdPainter.drawStringAt(0,y,"Font24g17|17char|8");
 y=y+24+1; epdPainter.drawLine(0,y,295,y); y=y+2; //--------------
 //-----display buffer------------------------------------------
 epdPainter.display();
 delay(8000);
 //-----clear display buffer and print big text-----------------
 y=1;
 epdPainter.clearDisplay();
  //-----big text------------------------------------------------
 epdPainter.drawLine(0,y,295,y); y=y+2; //----------------------
 epdPainter.setFont(&Font16);
 epdPainter.drawBigStringAt(0,y,"Big16|13char|3");
 y=y+2*16-2; epdPainter.drawLine(0,y,295,y); y=y+2; //----------
 epdPainter.setFont(&Font20);
 epdPainter.drawBigStringAt(0,y,"Big20|10char|4");
 y=y+2*20-2; epdPainter.drawLine(0,y,295,y); y=y+2; //----------
 epdPainter.setFont(&Font24);
 epdPainter.drawBigStringAt(0,y,"Big24|8char");
 y=y+2*24-2; epdPainter.drawLine(0,y,295,y); //-----------------
 //-----display buffer------------------------------------------
 epdPainter.display();
 delay(8000);
}
