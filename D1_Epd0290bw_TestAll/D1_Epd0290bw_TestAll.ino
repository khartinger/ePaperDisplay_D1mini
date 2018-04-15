//_____D1_Epd0290bw_TestAll.ino_______________180415-180415_____
// Demo program to display text and shapes on a 2-color
// e-paper display (2,9 inch, 128x296 px, black and white)

#include "libs/D1_class_Epd_2in9bw.h"
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
 epdPainter.drawRectangle(2,2,125,293,color2);
  //-----draw circle, triangle, text-----------------------------
 epdPainter.drawCircle(63,147,41,color1);
 epdPainter.drawFilledCircle(63,147,38,color2);
 epdPainter.drawTriangle(62,109,34,171,92,171,WHITE);
 //-----draw some small characters------------------------------
 epdPainter.setFont(&Font12);
 epdPainter.drawStringAt(33,215,"15.4.2018", BLACK);
 epdPainter.setFont(&Font20);
 epdPainter.drawStringAt(42,154,"CKH",WHITE);
 //-----"left eye"----------------------------------------------
 epdPainter.drawBigPixel(59,137,WHITE);
 //-----"right eye"---------------------------------------------
 epdPainter.drawPixel(65,137,WHITE);
 epdPainter.drawPixel(66,137,WHITE);
 epdPainter.drawPixel(66,138,WHITE);
 epdPainter.drawPixel(67,137,WHITE);
 epdPainter.drawPixel(67,138,WHITE);
 epdPainter.drawPixel(68,137,WHITE);
 epdPainter.drawPixel(69,137,WHITE);
 epdPainter.drawPixel(70,137,WHITE);
 //-----"mouth"-------------------------------------------------
 epdPainter.drawLine(55,143,59,147,WHITE);
 epdPainter.drawLine(59,147,67,147,WHITE);
 epdPainter.drawLine(67,147,71,143,WHITE);
 epdPainter.drawLine(55,143,58,148,WHITE);
 epdPainter.drawLine(58,148,68,148,WHITE);
 epdPainter.drawLine(68,148,71,143,WHITE);
 epdPainter.drawLine(59,149,67,149,WHITE);
 //-----lines top v, botton ^ ----------------------------------
 epdPainter.drawBigLine( 10,  6, 63, 16,color1);
 epdPainter.drawBigLine( 63, 16,117,  6,color1);
 epdPainter.drawBigLine( 10,289, 63,279,color1);
 epdPainter.drawBigLine( 63,279,117,289,color1);
 //----- big lines > < -----------------------------------------
 epdPainter.drawLine( 10,  6, 20,147,color1);
 epdPainter.drawLine( 20,147, 10,289,color1);
 epdPainter.drawLine(117,  6,107,147,color1);
 epdPainter.drawLine(107,147,117,289,color1);
 //-----draw some big characters--------------------------------
 epdPainter.drawBigCharAt(51,20,'A', BLACK);
 epdPainter.drawBigStringAt(22,230,"CKH", BLACK);
 //-----fraw filled triangles-----------------------------------
 epdPainter.drawFilledTriangle(6,8,16,147,6,287,color2);
 epdPainter.drawFilledTriangle(121,8,111,147,121,287,color2);
 epdPainter.display();
 Serial.println("END");
}

void loop() {
 Serial.println("LOOP :) ");
 delay(5000);
}
