# D1 mini with EPD 1.54inch: Counter
Sketch: D1_Epd0154bw_Counter.ino, Version 2018-04-15   
[Deutsche Version](./LIESMICH.md "Deutsche Version")   

Demo program to display a counter value on a 2-color epd (e-paper display: 1.54 inch, 200x200 px, black, white).
* The counter counts in a loop from 100 to 999 and the counter value is printed to Serial.
* Every 10th value is tried to be shown on the ep-display.
* When a value is displayed successfully, !D! is added to Serial output.

## Hardware
* WeMos D1 Mini
* Waveshare e-paper display (1.54 inch, 200x200 px, black and white).  
* Upon request: protoboard with spi and i2c connection

![D1 epd0154bw Counter](./images/D1_epd0154bw_counter.png "D1mini mit ePaper display 1,54inch Counter")   

## Example for Serial output

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

