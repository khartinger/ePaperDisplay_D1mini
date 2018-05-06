# D1 mini with EPD 1.54inch: Counter
Sketch: D1_Epd0154bw_Counter.ino, Version 2018-05-05   
[Deutsche Version](./LIESMICH.md "Deutsche Version")   

E-paper displays (EPD) need a lot of time to change the displayed content (from one  to over 10 seconds).   
This demo program contents a counter and tries to display the counter value on a 2-color epd (e-paper display: 1.54 inch, 200x200 px, black, white).   
__In detail:__   
* The counter counts in 0.5s steps from 1 to 120.
* Is the EDP ready, the counter value is displayed and stays stabil for 3.34s.
* Every counter value is printed to Serial and a word for the action is added (`_SHOW`, `_busy` or `_wait`)

## Hardware
* WeMos D1 Mini
* Waveshare e-paper display (1.54 inch, 200x200 px, black and white).  
* Upon request: protoboard with spi and i2c connection

![D1 epd0154bw Counter](./images/D1_Epd0154bw_counter.png "D1mini mit ePaper display 1,54inch Counter")   

## Example for Serial output

```
Init e-Paper Display: INIT OK
  1_SHOW    2_busy    3_busy    4_wait    5_wait    6_wait    7_wait    8_SHOW    9_busy   10_busy  
 11_wait   12_wait   13_wait   14_wait   15_SHOW   16_busy   17_busy   18_wait   19_wait   20_wait  
 21_wait   22_SHOW   23_busy   24_busy   25_wait   26_wait   27_wait   28_wait   29_SHOW   30_busy  
 31_busy   32_wait   33_wait   34_wait   35_wait   36_SHOW   37_busy   38_busy  
```
