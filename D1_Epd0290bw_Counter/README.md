# D1 mini with EPD 2inch9: Counter
Sketch: D1_Epd0290bw_Counter.ino, Version 2018-04-15   
[Deutsche Version](./LIESMICH.md "Deutsche Version")   

Demo program to display a counter value on a 2-color epd (e-paper display: 2,9 inch, 128x296 px, black, white).
* The counter counts in a loop from 100 to 999 and the counter value is printed to Serial.
* Every 10th value is tried to be shown on the ep-display.
* When a value is displayed successfully, !D! is added to Serial output.

## Hardware
* WeMos D1 Mini
* Waveshare e-paper display (2,9 inch, 128x296 px, black and white).  
* Upon request: protoboard with spi and i2c connection
 