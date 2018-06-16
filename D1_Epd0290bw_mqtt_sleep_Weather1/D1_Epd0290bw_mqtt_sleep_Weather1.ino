//_____D1_Epd0290bw_mqtt_sleep_Weather1.ino___180513-180513_____
// Little weather station with 2 sensors and e-paper display.
// * BME280 (temperature, humidity, air pressure)
// * BH1750 (light sensor) and a 
// * 2-color e-paper display (2.9inch, 128x296px, black, white).
// MQTT: * Send values with topic CLIENT_NAME to a broker.
//       * Send topic "getDate" and wait for an answer with 
//         topic date, payload YYYYmmdd HHMMSS
// After sending the message D1mini goes to sleep for 10 minutes
//
// Hardware: (1) WeMos D1 Mini
//           (2) e-paper display (2.9", 128x296 px, black, white).
//               connected to spi: 3.3V, GND;
//               BUSY=D6, RST=D4, DC=D3, CS=D8, CLK=D5, DIN=D7.
//           (3) BME280 connected to I2C: SCL=D1,SDA=D2,GND,3V3
//               (I2C-address 0x76)
// Important: (1) Example needs a broker!!!
//            (2) Connect D0 to RST to wake up D1 mini!!!
// Created by Karl Hartinger, June 13, 2018,
// Last Change 2018-06-13: -
// Released into the public domain.
#include "src/mqtt/D1_class_MqttClientKH.h"
#include "src/bh1750/D1_class_BH1750.h"
#include "src/bme280/D1_class_BME280.h"
#include "src/ain/D1_class_Ain.h"
#include "src/epd/D1_class_Epd_2in9bw.h"
#include "src/epd/D1_class_EpdPainter.h"
#include "src/epd/D1_class_EpdPainterBar.h"
#define  CLIENT_NAME              "Weather290a"
#define  TOPIC_IN1                "date"
#define  TOPIC_OUT1               "getDate"
#define  TOPIC_OUT2               CLIENT_NAME
#define  SLEEP_SEC                600            // 600=10min
//#define  SLEEP_SEC                10            // 10sec
#define  DATA_SEP                 " | "
#define  AWAKE_MAX_SEC            10            // goto sleep
#define  DEG                      String((char)248)
#define  DEBUG4                   true

// ***** change this values to your own WLAN data!!!************
//MqttClientKH client("..ssid..", "..password..","mqttservername");
MqttClientKH client("Raspi10", "12345678","10.1.1.1");

EpdConnection connection(D6,D4,D3,D8,1);//busy,reset,dc,cs,busyLevel
Epd_          epd(connection);         //the ePaperDisplay
EpdPainter    epdPainter(epd);         //print methods
EpdPainterBar bar(&epdPainter);
BH1750 bh1750;                         // b brightness
BME280 bme280;                         // t,h,p sensor object
String datetime="?";                   // date, time
int countdown_s=AWAKE_MAX_SEC;         // sec
Ain ain;                               // analog object for
String vsupply;                        // 5V supply voltage
long   millis_;

//_____process all subscribed incoming messages_________________
void callback(char* topic, byte* payload, unsigned int length)
{
//-----convert topic and payload to String, print to Serial-----
 String sTopic=String(topic);
 String sPayload="";
 for (int i=0; i<length; i++) sPayload+=(char)payload[i];
 if(DEBUG4)
 {
  Serial.print("Message received for topic "+sTopic+"=");
  Serial.println(sPayload);
 }
 if(sTopic=="date") { datetime=sPayload; }
}

//_____float to String__________________________________________
String float2String(float f, int len, int decimals)
{
 String s1=String(f,decimals);
 int i_=s1.length();
 for(;i_<len; i_++) s1=" "+s1;
 return s1;
}

//_____show weather values on epd_______________________________
bool displayValuesEpd(float ft_, float fh_, float fp_, float fb_,
     String sVsupply, String sInfo)
{
 //-----check if epd is ready-----------------------------------
 if(epd.isBusy()) return false;
 //-----draw bar graph frame------------------------------------
 epdPainter.clearDisplay();                 // clear buffer
 epdPainter.setFont(&Font20x11_255);        // font
 bar.setParams(54,10,74,162, 4,1, -20.0,50.0,10.0);
 bar.drawFrame();
 bar.drawScale(&Font20x11_255, "3.0", BAR_SCALE_LEFT);
 bar.drawBar(ft_);
 //-----draw values---------------------------------------------
 String s1; 
 int dy=19, y=275-5*dy;
 epdPainter.drawBigLine(0,y-5,127,y-5);
 epdPainter.setFont(&Font20x11_255);
 s1=float2String(ft_,6,1);
 epdPainter.drawStringAt(11,y,     s1+" "+DEG+"C");
 s1=float2String(fh_,6,1);
 epdPainter.drawStringAt(11,y+  dy,s1+" %");
 s1=float2String(fp_,6,1);
 epdPainter.drawStringAt(11,y+2*dy,s1+" hPa");
 s1=float2String(fb_,6,0);
 epdPainter.drawStringAt(11,y+3*dy,s1+" lx");
 epdPainter.drawStringAt(11,y+4*dy," "+sVsupply+" V");
 epdPainter.setFont(&Font16x8_255);
 epdPainter.drawFilledRectangle(0,275,127,295,BLACK); // info
 epdPainter.drawStringAt(5,278,sInfo, WHITE);
 //-----text rotation test--------------------------------------
 epdPainter.setFont(&Font20x11_255);        // font
 epdPainter.setRotation(ROTATE_270);
 epdPainter.drawStringAt(155,85,"Temperatur");
 epdPainter.setFont(&Font12);              // font
 epdPainter.setRotation(ROTATE_270);
 epdPainter.drawStringAt(134,114,"13.5.2018 K. Hartinger");
 epdPainter.setRotation(ROTATE_0);          // y=long side
 epdPainter.display();                      // display value
 return true;
}

//_____setup____________________________________________________
void setup() {
 millis_=millis();                          // start "time"
 Serial.begin(115200); Serial.println();    // init Serial
 //-----try to init e-paper display-----------------------------
 if(DEBUG4) Serial.print("Init e-Paper Display: ");
 if (!epd.init()) {
  countdown_s=0;                            // 0=loop goto sleep
  if(DEBUG4) Serial.println("init failed.");
 }
 //-----some more display inits---------------------------------
 connection.setWaitIdle(0);                 // 0=dont wait
 epdPainter.setFont(&Font16x8_255);         // font
 epdPainter.setRotation(ROTATE_0);          // y=long side
 epdPainter.clearDisplay();                 // clear buffer
 if(DEBUG4) Serial.println("OK");           // init OK
 epdPainter.drawFilledRectangle(0,275,127,295,BLACK); // info
 bool err=false;
 //-----check for BH1750----------------------------------------
 int status=bh1750.getStatus();
 if(status!=BH1750_OK)
 {
  Serial.println("Init BH1750 failed!");
  epdPainter.drawStringAt(0,279,"BH1750 ERROR",WHITE);
  err=true;
 }
 //-----check for BME280----------------------------------------
 if (!bme280.begin())
 {
  if(DEBUG4) Serial.println("BME280 missing - check wiring!");
  countdown_s=0;                            // 0=loop goto sleep
  epdPainter.drawStringAt(0,279,"BME280 ERROR",WHITE);
  err=true;
 }
 //-----connect to wifi, setup mqtt-----------------------------
 client.setClientName(String(CLIENT_NAME));
 client.addSubscribe("date");
 //client.addSubscribe(String(TOPIC_OUT1));
 client.setCallback(callback);
 client.reconnect();  
 //-----setup volage measurement--------------------------------
 ain.setRefPoints(0, 0.0, 840, 4.90);       // new ref values
 if(DEBUG4) Serial.println("setup ready!"); // init OK
 if(err) epdPainter.display();              // display value
}

float t_,h_,p_,a_;        // temperature, humidity, pressure
float b_;                 // brightness
String sValues;           // measurement values

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
   //-----get all weather values--------------------------------
   bme280.getValues(t_, h_, p_, a_);
   b_=(float) bh1750.getBi();
   vsupply=ain.getsVoltage(3);
   String sValues=String(t_,1)+"*C"+DATA_SEP+String(h_,1)+"%"+DATA_SEP;
   sValues+=String(p_,1)+"hPa"+DATA_SEP+String(b_,0)+"LX"+DATA_SEP+vsupply+"V";
   //-----(try to) publish values-------------------------------
   if(client.isConnected()&&(!bPub_))
   {
    client.publishString(TOPIC_OUT2, sValues, true);
    bPub_=true;
    if(DEBUG4) Serial.println("Published: "+sValues);
    delay(100);
   }
   //-----(try to) display values-------------------------------
   if((!epd.isBusy())&&(!bEpd_)) {
    displayValuesEpd(t_, h_, p_, b_, vsupply, datetime);
    bEpd_=true;
    if(DEBUG4) Serial.println("EPD displayed: "+sValues);
   }
   //-----wait for next trial-------------------------------- 
   if(bPub_ && bEpd_) countdown=0;
   else { countdown--; delay(100); }
  }
 }
 else
 {
  displayValuesEpd(t_, h_, p_, b_, vsupply, "No connection!");
  if(DEBUG4) Serial.println("EPD: No Connection");
 }
 //-----display sleep, D1mini sleep----------------------------
 connection.sendCommand(DEEP_SLEEP_MODE);
 connection.sendData(1);
 if(DEBUG4) {
  Serial.print(String((millis()-millis_)));
  Serial.println(" ms awake - Goto Sleep...");
 }
 ESP.deepSleep(SLEEP_SEC * 1000000L);   // usec
 delay(200);
}

