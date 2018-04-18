# D1 mini mit EPD 1,54inch: Anzeige von Temperatur, Feuchtigkeit und Druck eines BME280
Sketch: D1_Epd0154bw_BME280.ino, Version 2018-04-15      
[English Version](./README.md "English Version")   

Messung von Temperatur, Luftfeuchtigkeit, Luftdruck/H&ouml;he mit einem BME280-Sensor alle 5 Sekunden und Anzeige der Werte auf einem 2-Farben e-Paper Display (1,54 inch, 200x200 px, schwarz/wei&szlig;)

## Hardware
* WeMos D1 Mini
* Waveshare e-paper display (1,54 inch, 200x200 px, schwarz-wei&szlig;).
* BME280 am I2C: SCL=D1,SDA=D2,GND,3V3 (I2C-Adresse 0x76)
* Eventuell Protoboard mit Anschl&uuml;ssen f&uuml;r den SPI- und I2C-Bus

![D1 epd0154bw MBE280](./images/D1_epd0154bw_bme280.png "D1mini mit e-Paper-Display 1,54inch und BME280")  

## Beispiel f&uuml;r die Ausgabe auf der seriellen Schnittstelle

```
Init e-Paper Display: INIT OK
Sensor BME280 ready.
22.9|0.0|633|3798.4
26.4|38.3|985|237.7
26.4|38.0|985|238.3
```
