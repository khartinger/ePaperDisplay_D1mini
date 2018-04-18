//_____D1_class_EpdPainter.h__________________180401-180412_____
// D1 mini class for painting text and shapes on a waveshare
// e-paper display.

#ifndef D1_CLASS_EPDPAINTER_H
#define D1_CLASS_EPDPAINTER_H
#include "Arduino.h"                   // 
#include "D1_class_Epd_1in54bw.h"      // class Edp_
#include "D1_class_EpdPaint.h"         // class EdpPaint

#define WHITE           0
#define BLACK           1
#define RED             2

class EpdPainter {
 //-----properties----------------------------------------------
 protected:
  Epd_ epd;                            //e-paper display object
  unsigned char* frame1;               // frame 1 (black)
  unsigned char* frame2;               // frame 2 (red, ...)
  EpdPaint* paintBlack;
  EpdPaint* paintColor;
  sFONT* font;

 //-----constructor & co----------------------------------------
 public:
  EpdPainter(Epd_ &epd);
  ~EpdPainter();
 
 //-----draw methods--------------------------------------------
 public:
  void clear(int color);
  void setRotation(int rotation);
  void setFont(sFONT* font);
  void drawBackgroundFromProgmem(const unsigned char* imageBlack);
  void drawBackgroundFromProgmem(const unsigned char* imageBlack, const unsigned char* imageColor);
  void drawPixel(int x0, int y0, int color=BLACK);
  void drawLine(int x0, int y0, int x1, int y1, int color=BLACK);
  void drawTriangle(int x0, int y0, int x1, int y1, int x2, int y2, int color=BLACK);
  void drawFilledTriangle(int x0, int y0, int x1, int y1, int x2, int y2, int color=BLACK);
  void drawRectangle(int x0, int y0, int x1, int y1, int color=BLACK);
  void drawFilledRectangle(int x0, int y0, int x1, int y1, int color=BLACK);
  void drawCircle(int x, int y, int radius, int color=BLACK);
  void drawFilledCircle(int x, int y, int radius, int color=BLACK);
  void drawCharAt(int x, int y, char ascii_char, int color=BLACK);
  void drawStringAt(int x, int y, String text, int color=BLACK);
  void drawBigPixel(int x, int y, int color=BLACK);
  void drawBigLine(int x0, int y0, int x1, int y1, int color=BLACK);
  void drawBigCharAt(int x, int y, char ascii_char, int color=BLACK);
  void drawBigStringAt(int x, int y, String text, int color=BLACK);


//-----display methods------------------------------------------
  void clearDisplay();
  void display();
};
#endif
