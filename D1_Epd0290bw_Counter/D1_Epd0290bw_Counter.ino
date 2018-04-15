//_____D1_Epd0290bw_Counter.ino_______________180415-180415_____
// Demo program to display a counter value on a 2-color epd
// (e-paper display: 2,9 inch, 128x296 px, black, white).
// * The counter counts in a loop from 100 to 999 and the counter 
//   value is printed to Serial.
// * Every 10th value is tried to be shown on the ep-display.
// * When a value is displayed successfully, !D! is added to
//   Serial output.

#include "libs/D1_class_Epd_2in9bw.h"
#include "libs/D1_class_EpdPainter.h"
#define   DELAY_LOOP         500            // 0,5s counter inc
#define   DELAY_EPD           73            // ms needs display

EpdConnection connection(D6,D4,D3,D8,1);    //busy,reset,dc,cs,busyLevel
Epd_ epd(connection);                       //the ePaperDisplay
EpdPainter epdPainter(epd);                 //print methods

int    counter=1000;                        // counter 100..999
String sCounter;                            // value as string
String sDisplayState;                       // SHOW, busy, ' '

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
 epdPainter.setFont(&Font24);               // font
 //epdPainter.setRotation(ROTATE_270);        // x=long side
}

//_____loop_____________________________________________________
void loop() {
 //-----update counter and display status-----------------------
 counter++; if(counter>999) counter=100;
 sCounter=String(counter);
 sDisplayState="";
 //-----try to print counter value on e-paper display-----------
 if(counter%10!=0) delay(DELAY_LOOP);       // just wait
 else {                                     // print display?
  if(connection.isBusy()) {                 // NO, display is...
   sDisplayState="_Busy";                   // ...not ready
   delay(DELAY_LOOP);                       // just wait
  }
  else { //-----YES, show counter value on display--------------
   sDisplayState="_!D! ";
   epdPainter.clearDisplay();               // clear buffer
   int x = epd.width/2 - 3*Font24.Width;    // center position
   int y = epd.height/2 - Font24.Height + 2;// +2: long tail
   //x=0; y=0;
   epdPainter.drawBigStringAt(x,y,sCounter);// print value
   epdPainter.display();                    // display value
   if(DELAY_LOOP-DELAY_EPD > 0)             // display 235ms,
    delay(DELAY_LOOP-DELAY_EPD);            // remaining time
  }
 }
 //-----print counter value on Serial---------------------------
 Serial.print(sCounter+sDisplayState+"  ");
 if(counter%10==0) { Serial.println(); }    // new line every 10
}

