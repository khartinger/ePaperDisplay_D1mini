//_____D1_class_MqttClientKH.h________________170721-180504_____
// The class MqttClient extends the class PubSubClient vor an
//  easy use of mqtt.
// You can use all commands from class PubSubClient as well.
// When PubSubClient lib is installed, delete directory /libs!
//
// Hardware: D1 mini
#ifndef D1_CLASS_MQTTCLIENTKH_H
#define D1_CLASS_MQTTCLIENTKH_H
#include "Arduino.h"                   // 
#include <ESP8266WiFi.h>               // WiFi
#include "PubSubClient.h"              // use with /src
#ifndef  MQTT_SSID
 #define MQTT_SSID      "ssid"         // wlan name
#endif
#ifndef  MQTT_PASS
 #define MQTT_PASS      ""             // no password
#endif
#ifndef  MQTT_SERVER
 #define MQTT_SERVER    "localhost"    // default server name
#endif
#ifndef  MQTT_PORT
 #define MQTT_PORT      1883           // default mqtt port
#endif
#define  SSID_SIZE      20             // max.len ssid
#define  PASS_SIZE      20             // max.len password
#define  MQTT_SIZE      20             // max.len mqttservername
#define  TOPIC_MAX      8              // max. topics to sub
#define  MQTT_RECONNECT_MS        4000
#define  TIMEOUT_WIFI_CONNECT_MS  8000 // wait for WLAN
#define  MESSAGE_MAXLEN            127
#ifndef  DEBUG_MQTT
 #define DEBUG_MQTT               true // true=Serial output
#endif


class MqttClientKH : public PubSubClient {
 //-----properties----------------------------------------------
 protected:
  char ssid_[SSID_SIZE+1];             //
  char pass_[PASS_SIZE+1];             // 
  char mqtt_[MQTT_SIZE+1];             // 
  int  port_;                          // mqtt port (def 1883)
  String aTopicSub_[TOPIC_MAX];        // subscribed topics
  String aTopicPub_[TOPIC_MAX];        // topics to publish
  String aPayloadPub_[TOPIC_MAX];      // value on (re)start
  bool   aRetainPub_[TOPIC_MAX];       // retain true|false
  int    numSub_;                      // number subscribed topics
  int    numPub_;                      // number topics to publish
  WiFiClient d1miniClient;             // WLAN client for MQTT
  String sClientName;                  // MQTT client name
  long millis_lastConnected;           // last connection time [ms]
 //-----constructor & co----------------------------------------
 public:
  MqttClientKH();
  MqttClientKH(char* ssid, char* pwd, char* mqtt_server);
  MqttClientKH(char* ssid, char* pwd, char* mqtt_server, int port);
 protected:
  void setup();
 //-----setter and getter methods-------------------------------
 public:
  int  getNumSub() { return numSub_; };
  int  getNumPub() { return numPub_; };
  void clrSubscribe() { numSub_=0; };
  void clrPublish() { numPub_=0; };
  void setClientName(String sName) {sClientName=sName;};
  String getClientName() { return sClientName; };
  String getsClientState(int client_state);        // NEW 180428
  //-----methods to setup WLAN and mqtt connection--------------
  bool setup_wifi();
  bool reconnect();
  bool isConnected();
  bool sendPubSubTopics();
  void printPubSubTopics2Serial(String clientId);
  //-----methods to define mqtt topics--------------------------
  bool addSubscribe(String topic);
  bool delSubscribe(String topic);
  bool addPublish(String topic, String payload, bool retain);
  bool delPublish(String topic);
  void publishString(String topic, String payload);
  void publishString(String topic, String payload, bool retain);
  //-----seldom used-------------------------------------------
  int  setSubscribe(String aTopicSub[], int num);
  int  setPublish(String aTopicPub[], String aPayload[], int num);
  void subscribeString(String topic);
};
#endif
