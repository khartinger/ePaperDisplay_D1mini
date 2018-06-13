# D1 mini mit EPD 2,9inch: Z&auml;hler
Sketch: D1_Epd0290bw_Counter.ino, Version 2018-04-15      
[English Version](./README.md "English Version")   

Dieses Programm zeigt einen Z&auml;hlerstand auf einem e-Paper Display an (2,9 inch, 128x296 Pixel, schwarz-wei&szlig;).
* Der Z&auml;hler z&auml;hlt in einer Schleife von 100 bis 999 und der Z&auml;hlerstand wird auf der seriellen Schnittstelle angezeigt.
* Bei jedem 10. Wert wird versucht, diesen auf dem ePaper-Display anzuzeigen.
* Kann der Wert erfolgreich angezeigt werden, wird dies auf der seriellen Schnittstelle durch Hinzuf&uuml;gen von !D! zum Z&auml;hlerwert angezeigt.

## Hardware
* WeMos D1 Mini
* Waveshare e-paper display (2,9 inch, 128x296 px, schwarz-wei&szlig;).
* Eventuell Protoboard mit Anschl&uuml;ssen f&uuml;r den SPI-Bus
