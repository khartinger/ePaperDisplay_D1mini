# D1 mini with EPD 1inch54: Display temperature, humidity and pressure measured by a BME280 and publish them
Sketch: D1_Epd0154bw_BME280_mqtt.ino, Version 2018-04-15   
[Deutsche Version](./LIESMICH.md "Deutsche Version")   

Measure temperature, humidity, pressure/altitude with a BME280 every 5 seconds and display values on a 2-color e-paper display (1.54 inch, 200x200 px, black and white).   
Publish values via MQTT:
* Send values with topic "BME280_1: " to a broker
* Send topic "getDate" and wait for an answer with topic "date", payload `YYYYmmdd HHMMSS` (YYYY year, mm month, dd day, HH hour, MM minute, SS seconds)   

After sending the message D1mini goes to sleep for 10 minutes

**Important: Example needs a broker! (eg Mosquitto)**   

__*Don't forget to change WiFi data to your network values*__ in line   
`MqttClientKH client("..ssid..", "..password..","mqtt server name");`  

## Hardware
* WeMos D1 Mini
* Waveshare e-paper display (1.54 inch, 200x200 px, black and white).  
* BME280 connected to I2C: SCL=D1,SDA=D2,GND,3V3 (I2C-address 0x76)
* Upon request: protoboard with spi and i2c connection

![D1 epd0154bw MBE280 MQTT](./images/D1_epd0154bw_bme280_mqtt.png "D1mini with ePaper display 1.54inch, BME280 and MQTT Publisher")   

## Example for Serial output

```
Connecting to Raspi11
.......
Connected! IP address is 192.168.1.174
MQTT: Not connected - reconnect...
=====BME280_1 connected======
-----publish topic list (0)-------
-----subscribe topic list (1)-----
date
==============================
26.9|38.5|985|234.9 published!
 - ePaper Display updated.
27.0|38.4|985|235.4 published!
 - ePaper Display NOT updated.
20180417 202727
27.0|37.4|985|236.1 published!
 - ePaper Display NOT updated.
20180417 202734
27.1|38.8|985|235.8 published!
 - ePaper Display updated.
20180417 202739
```
