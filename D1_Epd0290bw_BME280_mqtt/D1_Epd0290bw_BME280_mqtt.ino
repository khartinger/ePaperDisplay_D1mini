//_____D1_Epd0290bw_BME280_mqtt.ino___________180405-180501_____
// Measure temperature, humidity pressure/altitude with a BME280
// every 5 seconds and display values on a 2-color
// e-paper display (2,9 inch, 128x296 px, black and white)
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
#include "src/D1_class_Epd_2in9bw.h"
#include "src/D1_class_EpdPainter.h"
#define  CLIENT_NAME              "BME280b"
#define  TOPIC_IN1                "date"
#define  TOPIC_OUT1               "getDate"
#define  TOPIC_OUT2               CLIENT_NAME
#define  DEBUG2                   true 
#define  DELAY_LOOP               (500-5)        // ms
#define  DELAY_EPD                78             // ms
#define  STATE_MAX                10             // state 0..9
#define  STATE_PUB_DATE           1
#define  STATE_PUB_MEAS           4
#define  STATE_DISPLAY_EDP        5
#define  DEG                      String((char)248)

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
 if(sTopic=="date")
 {
  String sPayload="";
  for (int i=0; i<length; i++) sPayload+=(char)payload[i];
  datetime=sPayload;
  if(DEBUG2) { 
   Serial.print("Message received for topic '"+sTopic+"': ");
   Serial.println(sPayload);
  }
 }
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
 //epdPainter.setFont(&Font24);
 epdPainter.setFont(&Font24x11_255);
 //-----print title---------------------------------------------
 int y=3;
  String s1=CLIENT_NAME;
 int x=(128-s1.length()*Font24x11_255.Width)/2;
 if(x<0) x=0;
 epdPainter.drawFilledRectangle(0,0,199,24,BLACK);
 epdPainter.drawStringAt(x,y,s1,WHITE);
 //-----print measure values------------------------------------
 y=y+28;
 s1="T="+partof(values, "|", 0)+DEG+"C";
 epdPainter.drawStringAt(6,y,s1);
 y=y+24;
 s1="H="+partof(values, "|", 1)+"%";
 epdPainter.drawStringAt(6,y,s1);
 y=y+24;
 s1="p="+partof(values, "|", 2)+" hPa";
 epdPainter.drawStringAt(6,y,s1);
 y=y+24;
 s1="a="+partof(values, "|", 3)+"m";
 epdPainter.drawStringAt(6,y,s1);
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
 y=y+24;
 epdPainter.drawBigLine(0,y,199,y);
 y=y+2;
 epdPainter.drawStringAt(6,y,"Date & Time:");
 y=y+24;
 epdPainter.drawStringAt(6,y,sDate);
 y=y+24;
 epdPainter.drawStringAt(6,y,sTime);
 //datetime="?";
 //-----display buffer------------------------------------------
 epdPainter.displayNoWait();
}

//_____setup____________________________________________________
void setup() {
 Serial.begin(115200); Serial.println();    // init Serial
 //-----try to init e-paper display-----------------------------
 if(DEBUG2) Serial.print("Init e-Paper Display: ");
 if (!epd.init()) {
  if(DEBUG2) Serial.println("init failed. STOP!");
  while(true) { yield(); };                 // endless loop
 }
 if(DEBUG2)Serial.println("INIT OK");       // init OK
 //-----init sensor BME280--------------------------------------
 if (!bme280.begin()) {
  if(DEBUG2)Serial.println("BME280 missing - check wiring! STOP!");
  while (true) { yield(); };
 }
 if(DEBUG2)Serial.println("Sensor BME280 ready.");    // init OK
 //-----connect to wifi, setup mqtt-----------------------------
 client.setClientName(String(CLIENT_NAME));
 client.addSubscribe("date");
 client.setCallback(callback);
 client.reconnect();  
}

int state=STATE_MAX;
int state_display_epd=STATE_DISPLAY_EDP;
int delay_loop;
String values;
long m;

//_____loop_____________________________________________________
void loop() {
 //-----update state counter------------------------------------
 state++; if(state>=STATE_MAX) state=0;
 if(DEBUG2) if(state==0) Serial.println("--------------------");
 if(DEBUG2) Serial.print("State "+String(state)+": ");
 if(DEBUG2) m=millis();
 delay_loop=DELAY_LOOP;
 //-----check for wlan/mqtt-connection (does mqtt loop!)--------
 if(client.isConnected())
 {
  if(state==STATE_PUB_DATE) //==================================
  {
   client.publishString(TOPIC_OUT1, "?", false); // true=retain
   if(DEBUG2) Serial.print(String(TOPIC_OUT1)+" published! ");
  }
  if(state==STATE_PUB_MEAS) //==================================
  {
   values=bme280.getsValues("|",1,1,0,1);
   client.publishString(TOPIC_OUT2, values, true);// true=retain
   if(DEBUG2) Serial.print(values+" published! ");
  }
  if(state==state_display_epd) //===============================
  {
   if(epd.isBusy())
   {//...try to do dispay in next state.........................
    if((++state_display_epd)>=STATE_MAX) state_display_epd=0;
   }
   else
   {//.....display measurement values on epd....................
    state_display_epd=STATE_DISPLAY_EDP;
    displayValues(values);
    if(DEBUG2) Serial.print("ePaper Display updated. ");
    if(DELAY_LOOP-DELAY_EPD>0) delay_loop=DELAY_LOOP-DELAY_EPD;
                          else delay_loop=0;
   }
  }
 }
 else //-----no wlan/mqtt connection: "reset" loop--------------
 {
  state=STATE_MAX;                          // because of error
 }
 if(DEBUG2) { Serial.print(millis()-m); Serial.println(" ms"); }
 delay(delay_loop);
}

