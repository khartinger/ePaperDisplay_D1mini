//_____D1_oop42_Epd0154bw_BME280_mqtt.ino_____180405-180429_____
// Measure temperature, humidity pressure/altitude with a BME280
// every 5 seconds and display values on a 2-color
// e-paper display (1,54 inch, 200x200 px, black amd white)
// MQTT: * Send values with topic "BME280_1: " to a broker.
//       * Send topic "getDate" and wait for an answer with 
//         topic date, payload YYYYmmdd HHMMSS
// Hardware: (1) WeMos D1 Mini
//           (2) e-paper display connected to spi: 3.3V, GND;
//               BUSY=D6, RST=D4, DC=D3, CS=D8, CLK=D5, DIN=D7.
//           (3) BME280 connected to I2C: SCL=D1,SDA=D2,GND,3V3
//               (I2C-address 0x76)
// Important: Example needs a broker!

#include "D1_class_MqttClientKH.h"
#include "D1_class_BME280.h"
#include "src/D1_class_Epd_1in54bw.h"
#include "src/D1_class_EpdPainter.h"
#define  CLIENT_NAME              "BME280b"
#define  TOPIC_IN1                "date"
#define  TOPIC_OUT1               "getDate"
#define  TOPIC_OUT2               CLIENT_NAME
#define  DELAY_LOOP               5000           // ms
#define  DELAY_EPD                61             // ms
#define  EPD_UPDATE_EVERY_N_MQTT  2

// ***** change this values to your own WLAN data!!!************
//MqttClientKH client("..ssid..", "..password..","mqttservername");
MqttClientKH client("Raspi10", "12345678","10.1.1.1");

EpdConnection connection(D6,D4,D3,D8,1);    //busy,reset,dc,cs,busyLevel
Epd_ epd(connection);                       //the ePaperDisplay
EpdPainter epdPainter(epd);                 //print methods
BME280 bme280;                              // sensor object
String datetime="?";                        // mqtt date, time

//_____process all subscribed incoming messages_________________
void callback(char* topic, byte* payload, unsigned int length)
{
//-----convert topic and payload to String, print to Serial-----
 String sTopic=String(topic);
 String sPayload="";
 for (int i=0; i<length; i++) sPayload+=(char)payload[i];
 //Serial.print("Message received for topic "+sTopic+"=");
 //Serial.println(sPayload);
 if(sTopic=="date") {datetime=sPayload; Serial.println(datetime);}
}

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

void displayValues(String values)
{
 //-----clear display-------------------------------------------
 epdPainter.clearDisplay();
 epdPainter.setFont(&Font24);
 //-----print title---------------------------------------------
 int y=3;
  String s1=CLIENT_NAME;
 int x=(200-s1.length()*Font20.Width)/2;
 if(x<0) x=0;
 epdPainter.drawFilledRectangle(0,0,199,24,BLACK);
 epdPainter.drawStringAt(0,y,s1,WHITE);
 //-----print measure values------------------------------------
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
 //-----line, mqtt date and time YYYYmmdd HHMMSS----------------
 epdPainter.setFont(&Font20);
 String sDate="?";
 String sTime="";
 if(datetime.length()==15)
 {
  sDate=datetime.substring(6,8)+"."+datetime.substring(4,6);
  sDate+="."+datetime.substring(0,4);
  sTime=datetime.substring(9,11)+":"+datetime.substring(11,13);
  sTime+=":"+datetime.substring(13);
 }
 y=y+24;
 epdPainter.drawBigLine(0,y,199,y);
 y=y+2;
 epdPainter.drawStringAt(0,y,"Date & Time:");
 y=y+20;
 epdPainter.drawStringAt(0,y,sDate);
 y=y+20;
 epdPainter.drawStringAt(0,y,sTime);
 //datetime="?";
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
 //-----connect to wifi, setup mqtt-----------------------------
 client.setClientName(String(CLIENT_NAME));
 client.addSubscribe("date");
 //client.addSubscribe(String(TOPIC_OUT1));
 client.setCallback(callback);
 client.reconnect();  
}

int counter=EPD_UPDATE_EVERY_N_MQTT;
//_____loop_____________________________________________________
void loop() {
 //-----update counter------------------------------------------
 counter++; if(counter>EPD_UPDATE_EVERY_N_MQTT) counter=0;
 //-----get all values at once----------------------------------
 String values=bme280.getsValues("|",1,1,0,1);
 //-----if connected: publish values----------------------------
 if(client.isConnected())
 {
  client.publishString(TOPIC_OUT1, "?", true);   // true=retain
  client.publishString(TOPIC_OUT2, values, true);// true=retain
  Serial.println(values+" published!");
  delay(50);
 }
 //-----time to display data?-----------------------------------
 if((!epd.isBusy()) && (counter==0))
 {
  displayValues(values);
  Serial.println(" - ePaper Display updated.");
  if(DELAY_LOOP-DELAY_EPD>0)
   delay(DELAY_LOOP-DELAY_EPD);             // display: 61ms
 }
 else
 {
  Serial.println(" - ePaper Display NOT updated.");
  delay(DELAY_LOOP);
 }
}
