# D1 mini mit EPD 1.54inch: Z&auml;hler
Sketch: D1_Epd0154bw_Counter.ino, Version 2018-04-15      
[English Version](./README.md "English Version")   

Dieses Programm zeigt einen Z&auml;hlerstand auf einem e-Paper Display an (1,54 inch, 200x200 Pixel, schwarz-wei&szlig;).
* Der Z&auml;hler z&auml;hlt in einer Schleife von 100 bis 999 und der Z&auml;hlerstand wird auf der seriellen Schnittstelle angezeigt.
* Bei jedem 10. Wert wird versucht, diesen auf dem ePaper-Display anzuzeigen.
* Kann der Wert erfolgreich angezeigt werden, wird dies auf der seriellen Schnittstelle durch Hinzuf&uuml;gen von !D! zum Z&auml;hlerwert angezeigt.

## Hardware
* WeMos D1 Mini
* Waveshare e-paper display (1.54 inch, 200x200 px, schwarz-wei&szlig;).
* Eventuell Protoboard mit Anschl&uuml;ssen f&uuml;r den SPI-Bus

![D1 epd0154bw Counter](./images/D1_Epd0154bw_counter.png "D1mini mit ePaper display 1,54inch Counter")   

## Beispiel f&uuml;r die Ausgabe auf der seriellen Schnittstelle

```
 Init e-Paper Display: INIT OK
 100_!D!
 101  102  103  104  105  106  107  108  109  110_!D!
 111  112  113  114  115  116  117  118  119  120_!D!
 121  122  123  124  125  126  127  128  129  130_!D!
 131  132  133  134  135  136  137  138  139  140_!D!
 141  142  143  144  145  146  147  148  149  150_!D!
 151  152  153  154  155  156  157  158  159  160_!D!
 161  162  163  164  165  166  167  168  169  170_!D!
```

