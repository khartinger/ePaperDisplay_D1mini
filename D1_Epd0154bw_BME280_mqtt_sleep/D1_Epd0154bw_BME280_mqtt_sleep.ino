//_____D1_Epd0154bw_BME280_mqtt_sleep.ino_____180405-180501_____
// Measure temperature, humidity pressure/altitude with a BME280
// display values on a 2-color
// e-paper display (1,54 inch, 200x200 px, black amd white)
// MQTT: * Send values with topic CLIENT_NAME to a broker.
//       * Send topic "getDate" and wait for an answer with 
//         topic date, payload YYYYmmdd HHMMSS
// After sending the message D1mini goes to sleep for 10 minutes
//
// Hardware: (1) WeMos D1 Mini
//           (2) e-paper display connected to spi: 3.3V, GND;
//               BUSY=D6, RST=D4, DC=D3, CS=D8, CLK=D5, DIN=D7.
//           (3) BME280 connected to I2C: SCL=D1,SDA=D2,GND,3V3
//               (I2C-address 0x76)
// Important: Example needs a broker!

#include "D1_class_MqttClientKH.h"
#include "D1_class_BME280.h"
#include "D1_class_Ain.h"
#include "src/D1_class_Epd_1in54bw.h"
#include "src/D1_class_EpdPainter.h"
#define  CLIENT_NAME              "BME280s1"
#define  TOPIC_IN1                "date"
#define  TOPIC_OUT1               "getDate"
#define  TOPIC_OUT2               CLIENT_NAME
#define  DELAY_EPD                78             // ms
//#define  SLEEP_SEC                600            // 600=10min
#define  SLEEP_SEC                10            // 10sec
#define  DATA_SEP                 "|"
#define  AWAKE_MAX_SEC            10
#define  DEBUG3                   true

// ***** change this values to your own WLAN data!!!************
//MqttClientKH client("..ssid..", "..password..","mqttservername");
MqttClientKH client("Raspi10", "12345678","10.1.1.1");

EpdConnection connection(D6,D4,D3,D8,1);    //busy,reset,dc,cs,busyLevel
Epd_ epd(connection);                       //the ePaperDisplay
EpdPainter epdPainter(epd);                 //print methods
BME280 bme280;                              // sensor object
String datetime="?";                        // mqtt date, time
int countdown_s=AWAKE_MAX_SEC;              // sec
Ain ain;                                    // analog object for
String vsupply;                             // 5V supply voltage
long   millis_;

//_____process all subscribed incoming messages_________________
void callback(char* topic, byte* payload, unsigned int length)
{
//-----convert topic and payload to String, print to Serial-----
 String sTopic=String(topic);
 String sPayload="";
 for (int i=0; i<length; i++) sPayload+=(char)payload[i];
 if(DEBUG3)
 {
  Serial.print("Message received for topic "+sTopic+"=");
  Serial.println(sPayload);
 }
 if(sTopic=="date") { datetime=sPayload; }
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
 epdPainter.setFont(&Font20);
 //-----print title---------------------------------------------
 int y=3;
 String s1=CLIENT_NAME;
 int x=(200-s1.length()*Font20.Width)/2;
 if(x<0) x=0;
 epdPainter.drawFilledRectangle(0,0,199,24,BLACK);
 epdPainter.drawStringAt(x,y,s1,WHITE);
 //-----print measure values------------------------------------
 y=y+24;
 s1="T="+partof(values, DATA_SEP, 0)+"*C";
 epdPainter.drawStringAt(0,y,s1);
 y=y+20;
 s1="H="+partof(values, DATA_SEP, 1)+"%";
 epdPainter.drawStringAt(0,y,s1);
 y=y+20;
 s1="p="+partof(values, DATA_SEP, 2)+" hPa";
 epdPainter.drawStringAt(0,y,s1);
 y=y+20;
 s1="a="+partof(values, DATA_SEP, 3)+"m";
 epdPainter.drawStringAt(0,y,s1);
 //-----volage 5V power supply----------------------------------
 y=y+20;
 epdPainter.drawBigLine(0,y,199,y);
 y=y+4;
 s1="U="+vsupply+"V";
 epdPainter.drawStringAt(0,y,s1);
 //-----line, mqtt date and time YYYYmmdd HHMMSS----------------
 //epdPainter.setFont(&Font20);
 String sDate="?";
 String sTime="";
 if(datetime.length()==15)
 {
  sDate=datetime.substring(6,8)+"."+datetime.substring(4,6);
  sDate+="."+datetime.substring(0,4);
  sTime=datetime.substring(9,11)+":"+datetime.substring(11,13);
  sTime+=":"+datetime.substring(13);
 }
 y=y+20;
 epdPainter.drawBigLine(0,y,199,y);
 y=y+4;
 epdPainter.drawStringAt(0,y,"Date & Time:");
 y=y+20;
 epdPainter.drawStringAt(0,y,sDate);
 y=y+20;
 epdPainter.drawStringAt(0,y,sTime);
 //-----display buffer------------------------------------------
 epdPainter.display();
}

//_____setup____________________________________________________
void setup() {
 millis_=millis();
 Serial.begin(115200); Serial.println();    // init Serial
 //-----try to init e-paper display-----------------------------
 if(DEBUG3) Serial.print("Init e-Paper Display: ");
 if (!epd.init()) {
  countdown_s=0;                            // 0=loop goto sleep
  if(DEBUG3) Serial.println("init failed.");
 }
 //-----clear display-------------------------------------------
 epdPainter.clearDisplay();
 epdPainter.setFont(&Font20);
 if(DEBUG3) Serial.println("INIT OK");      // init OK
 //-----init sensor BME280--------------------------------------
 if (!bme280.begin()) {
  if(DEBUG3) Serial.println("BME280 missing - check wiring!");
  countdown_s=0;                            // 0=loop goto sleep
 }
 if(DEBUG3) Serial.println("Sensor BME280 ready.");  // init OK
 //-----connect to wifi, setup mqtt-----------------------------
 client.setClientName(String(CLIENT_NAME));
 client.addSubscribe("date");
 //client.addSubscribe(String(TOPIC_OUT1));
 client.setCallback(callback);
 client.reconnect();  
 //-----setup volage measurement--------------------------------
 ain.setRefPoints(0, 0.0, 840, 4.90);       // new ref values
 if(DEBUG3) Serial.println("setup ready!"); // init OK
}

//_____loop_____________________________________________________
void loop() {
 if(client.isConnected()) //-----connect successfull------------
 {
  client.publishString(TOPIC_OUT1, "?", false);  // true=retain
  //-----wait max. half time for date&time----------------------
  int countdown=countdown_s*5;
  bool wait_=true;
  while((countdown>0)&&wait_)
  {
   delay(50);
   client.isConnected();
   delay(50);
   if(datetime!="?") wait_=false;
   countdown--;
  }
  countdown+=countdown_s*5;
  //-----use remaining time for sending measurement values------
  bool bPub_=false, bEpd_=false;
  while(countdown>0)
  {
   //-----get all values at once--------------------------------
   String values=bme280.getsValues(DATA_SEP,1,1,0,1);
   vsupply=ain.getsVoltage(3);
   //-----(try to) publish values-------------------------------
   if(client.isConnected()&&(!bPub_))
   {
    String s1=values+DATA_SEP+vsupply;
    client.publishString(TOPIC_OUT2, s1, true); // true=retain
    bPub_=true;
    if(DEBUG3) Serial.println("Published: "+values);
   }
   //-----(try to) display values-------------------------------
   if((!epd.isBusy())&&(!bEpd_)) {
    displayValues(values);
    bEpd_=true;
    if(DEBUG3) Serial.println("EPD displayed: "+values);
   }
   //-----wait for next trial-------------------------------- 
   if(bPub_ && bEpd_) countdown=0;
   else { countdown--; delay(100); }
  }
 }
 else
 {
  if(DEBUG3) Serial.println("EPD: No Connection");
  epdPainter.clearDisplay();
  epdPainter.setFont(&Font20);
  epdPainter.drawStringAt(5,5,"NO Connection");
  epdPainter.drawStringAt(5,30,"to WLAN/MQTT");
  epdPainter.display();
 }
 //-----display sleep, D1mini sleep----------------------------
 connection.sendCommand(DEEP_SLEEP_MODE);
 connection.sendData(1);
 if(DEBUG3) {
  Serial.print(String((millis()-millis_)));
  Serial.println(" ms awake - Goto Sleep...");
 }
 ESP.deepSleep(SLEEP_SEC * 1000000L);   // usec
 delay(200);
}
