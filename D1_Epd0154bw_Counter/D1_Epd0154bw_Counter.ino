//_____D1_Epd0154bw_Counter.ino_______________180401-180505_____
// Demo program to display a counter value on a 2-color epd
// (e-paper display: 1,54 inch, 200x200 px, black, white).
// * The counter counts in a loop from 1 to 120 every 0,5s.
// * The counter value ist printed to Serial.
// * When a value is displayed, it stays displayed for 3,34sec.
// * When a value is displayed, !D! is added to Serial output.

#include "src/epd/D1_class_Epd_1in54bw.h"
#include "src/epd/D1_class_EpdPainter.h"
#define   DELAY_LOOP         500       // 0,5s counter inc
#define   COUNTER_MIN          1
#define   COUNTER_MAX        120
#define   SHOW_MS           3340       // display counter value

long millisNextDisplay=0;              // time of last led on
long loop_delay=DELAY_LOOP;            // loop delay [ms]
long loop_millis=0;                    // loop duration [ms]

EpdConnection connection(D6,D4,D3,D8,1);    //busy,reset,dc,cs,busyLevel
Epd_ epd(connection);                       //the ePaperDisplay
EpdPainter epdPainter(epd);                 //print methods

int    counter=COUNTER_MIN-1;               // counter 101..220
String sCounter;                            // value as string
String sDisplayState;                       // SHOW, busy, ' '


//_____setup____________________________________________________
void setup() {
 Serial.begin(115200); Serial.println("\n");// init Serial
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
}

//_____loop_____________________________________________________
void loop() {
 loop_millis=millis();                      // get start "time"
 //-----update counter and display status-----------------------
 counter++; if(counter>COUNTER_MAX) counter=COUNTER_MIN;
 sCounter="   "+String(counter);
 sCounter=sCounter.substring(sCounter.length()-3);
 sDisplayState="";
 //-----try to print counter value on e-paper display-----------
 if(connection.isBusy()) {                  // NO, display is...
  sDisplayState="_busy";                    // ...not ready
 }
 else                                       // print display?
 {   
  if(loop_millis > millisNextDisplay)
  {
   sDisplayState="_SHOW";
   millisNextDisplay=loop_millis+SHOW_MS;   // next display time
   epdPainter.clearDisplay();               // clear buffer
   int x = epd.width/2 - 3*Font24.Width;    // center position
   int y = epd.height/2 - Font24.Height + 2;// +2: long tail
   epdPainter.drawBigStringAt(x,y,sCounter);// print value
   epdPainter.displayNoWait();              // display value
  }
  else
  {
   sDisplayState="_wait";                   // ...not ready
  }
 }
 //-----print counter value on Serial---------------------------
 Serial.print(sCounter+sDisplayState+"  ");
 if(counter%10==0) { Serial.println(); }    // new line every 10
 //-----prepare next state and wait-----------------------------
 loop_millis=millis()-loop_millis;          // loop duration
 loop_delay=loop_delay-loop_millis;         // rest delay
 if(loop_delay<0) loop_delay=0;             // time >= 0
 delay(loop_delay);                         // wait
 loop_delay=DELAY_LOOP;                     // set next delay
}

