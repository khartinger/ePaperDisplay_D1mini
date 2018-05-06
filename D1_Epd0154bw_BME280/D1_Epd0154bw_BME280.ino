//_____D1_oop41_Epd0154bw_BME280.ino__________180405-180505_____
// Measure temperature, humidity pressure/altitude with a BME280
// every 5 seconds and display values on a 2-color
// e-paper display (1,54 inch, 200x200 px, black and white)
// Hardware: (1) WeMos D1 Mini
//           (2) e-paper display connected to spi: 3.3V, GND;
//               BUSY=D6, RST=D4, DC=D3, CS=D8, CLK=D5, DIN=D7.
//           (3) BME280 connected to I2C: SCL=D1,SDA=D2,GND,3V3
//               (I2C-address 0x76)
#include "src/bme280/D1_class_BME280.h"
#include "src/epd/D1_class_Epd_1in54bw.h"
#include "src/epd/D1_class_EpdPainter.h"

EpdConnection connection(D6,D4,D3,D8,1);    //busy,reset,dc,cs,busyLevel
Epd_ epd(connection);                       //the ePaperDisplay
EpdPainter epdPainter(epd);                 //print methods
BME280 bme280;                              // sensor object

//_____get part of the String s1, devided by sep________________
// num...number of part (starting with 0)
String partof(String s1, String sep, int num)
{
 String s_="";
 if(num<0) return s_;
 int i, iold=0, iadd=sep.length();
 int numsep=0;
 while((i=s1.indexOf(sep,iold))>=0)
 {
  if(num==numsep) { return s1.substring(iold,i); }
  numsep++;
  iold=i+iadd;
 }
 if(num==numsep) s_=s1.substring(iold);
 return s_;
}

//_____display measured values on e-paper display_______________
void displayValues(String values)
{
 //-----check clear and paint edge "lines"----------------------
 epdPainter.clearDisplay();
 epdPainter.setFont(&Font24);
 int y=3;
 String s1="   BME280   ";
 epdPainter.drawFilledRectangle(0,0,199,26,BLACK);
 epdPainter.drawStringAt(0,y,s1,WHITE);
 y=y+28;
 s1="T="+partof(values, "|", 0)+"*C";
 epdPainter.drawStringAt(0,y,s1);
 y=y+24;
 s1="H="+partof(values, "|", 1)+"%";
 epdPainter.drawStringAt(0,y,s1);
 y=y+24;
 s1="p="+partof(values, "|", 2)+" hPa";
 epdPainter.drawStringAt(0,y,s1);
 y=y+24;
 s1="a="+partof(values, "|", 3)+"m";
 epdPainter.drawStringAt(0,y,s1);
 //-----display buffer------------------------------------------
 epdPainter.display();
}

//_____setup____________________________________________________
void setup() {
 Serial.begin(115200); Serial.println();    // init Serial
 //-----try to init e-paper display-----------------------------
 Serial.print("Init e-Paper Display: ");
 if (!epd.init()) {
  Serial.println("init failed. STOP!");
  while(true) { yield(); };                 // endless loop
 }
 Serial.println("INIT OK");                 // init OK
 //-----init sensor BME280--------------------------------------
 if (!bme280.begin()) {
  Serial.println("BME280 missing - check wiring! STOP!");
  while (true) { yield(); };
 }
 Serial.println("Sensor BME280 ready.");    // init OK
}

void loop() {
  //-----get all values at once----------------------------------
 String values=bme280.getsValues("|",1,1,0,1);
 Serial.println(values);
 displayValues(values);
 delay(5000);
}
