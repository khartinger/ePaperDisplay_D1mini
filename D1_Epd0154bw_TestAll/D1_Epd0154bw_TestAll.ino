//_____D1_Epd0154bw_TestAll.ino_______________180401-180417_____
// Demo program to display text and shapes on a 2-color
// e-paper display (1,54 inch, 200x200 px, black amd white)

#include "libs/D1_class_Epd_1in54bw.h"
#include "libs/D1_class_EpdPainter.h"

EpdConnection connection(D6,D4,D3,D8,1);    //busy,reset,dc,cs,busyLevel
Epd_ epd(connection);                       //the ePaperDisplay
EpdPainter epdPainter(epd);                 //print methods

int color1 = BLACK;
int color2 = BLACK;

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
 //-----check clear and paint edge "lines"----------------------
 epdPainter.clearDisplay();
 epdPainter.clear(WHITE);
 epdPainter.drawRectangle(2,2,197,197,color2);
  //-----draw circle, triangle, text-----------------------------
 epdPainter.drawCircle(99,99,68,color1);
 epdPainter.drawFilledCircle(99,99,64,color2);
 epdPainter.drawTriangle(99,45,55,125,143,125,color1);
 epdPainter.drawTriangle(99,35,46,130,152,130,WHITE);
 epdPainter.setFont(&Font24);
 epdPainter.drawStringAt(73,99,"CKH",WHITE);
 //-----"left eye"----------------------------------------------
 epdPainter.drawBigPixel(93,71,WHITE);
 //-----"right eye"---------------------------------------------
 epdPainter.drawPixel(105,70,WHITE);
 epdPainter.drawPixel(106,70,WHITE);
 epdPainter.drawPixel(105,71,WHITE);
 epdPainter.drawPixel(106,71,WHITE);
 epdPainter.drawPixel(105,72,WHITE);
 epdPainter.drawPixel(106,72,WHITE);
 //-----"mouth"-------------------------------------------------
 epdPainter.drawLine(85,80,92,88,WHITE);
 epdPainter.drawLine(92,88,106,88,WHITE);
 epdPainter.drawLine(106,88,113,80,WHITE);
 epdPainter.drawLine(85,81,92,89,WHITE);
 epdPainter.drawLine(92,89,106,89,WHITE);
 epdPainter.drawLine(106,89,113,81,WHITE);
 epdPainter.drawLine(93,90,105,90,WHITE);
 //-----lines top v, botton ^ ----------------------------------
 epdPainter.drawLine(10,10,99,22,color1);
 epdPainter.drawLine(99,22,189,10,color1);
 epdPainter.drawLine(10,189,99,177,color1);
 epdPainter.drawLine(99,177,189,189,color1);
 //----- big lines > < -----------------------------------------
 epdPainter.drawBigLine(10,10,22,99,color1);
 epdPainter.drawBigLine(22,99,10,189,color1);
 epdPainter.drawBigLine(189,10,177,99,color1);
 epdPainter.drawBigLine(177,99,189,189,color1);
 //-----draw some big characters--------------------------------
 epdPainter.drawBigCharAt(84,0,'A', BLACK);
 epdPainter.drawBigStringAt(48,165,"CKH", BLACK);
 //-----fraw filled triangles-----------------------------------
 epdPainter.drawFilledTriangle(15,184,35,144,55,164,color2);
 epdPainter.drawFilledTriangle(184,184,144,164,164,144,color2);
 epdPainter.display();
 Serial.println("END");
}

void loop() {
 Serial.println("LOOP :) ");
 delay(5000);
}
