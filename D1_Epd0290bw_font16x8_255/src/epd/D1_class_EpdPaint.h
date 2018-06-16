/**
 *  @filename: D1_class_Paint.h (epdpaint.h)
 *  @brief   : Header file for epdpaint.cpp
 *  @author  : Yehui from Waveshare
 *  @updates : by Christian & Karl Hartinger
 *             2018-04-01 Add DrawLine, DrawTriangle, DrawBig...
 *             2018-06-01 Add DrawEllipse
 *
 *  Copyright (C) Waveshare  July 28 2017
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documnetation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to  whom the Software is
 * furished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS OR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#ifndef EPDPAINT_H
#define EPDPAINT_H

//-----Display orientation--------------------------------------
#define ROTATE_0             0
#define ROTATE_90            1
#define ROTATE_180           2
#define ROTATE_270           3

// Color inverse. 1 or 0 = set or reset a bit if set a colored pixel
#define IF_INVERT_COLOR      0

//#include "fonts.h"              // NEW 180401
#include "../fonts/fonts.h"       // NEW 180401

// NEW 180401: class name changed from Paint to EpdPaint
class EpdPaint {                  // NEW 180401
 public:
  EpdPaint(unsigned char* image, int width, int height); // NEW 180401
  ~EpdPaint();                    // NEW 180401
  void Clear(int colored);
  int  GetWidth(void);
  void SetWidth(int width);
  int  GetHeight(void);
  void SetHeight(int height);
  int  GetRotate(void);
  void SetRotate(int rotate);
  unsigned char* GetImage(void);
  void DrawAbsolutePixel(int x, int y, int colored);
  void DrawPixel(int x, int y, int colored);
  void DrawCharAt(int x, int y, char ascii_char, sFONT* font, int colored);
  void DrawStringAt(int x, int y, const char* text, sFONT* font, int colored);
  void DrawHorizontalLine(int x, int y, int width, int colored);
  void DrawVerticalLine(int x, int y, int height, int colored);
  void DrawRectangle(int x0, int y0, int x1, int y1, int colored);
  void DrawFilledRectangle(int x0, int y0, int x1, int y1, int colored);
  void DrawCircle(int x, int y, int radius, int colored);
  void DrawFilledCircle(int x, int y, int radius, int colored);
  //-----NEW 180401---------------------------------------------
  void DrawLine(int x0, int y0, int x1, int y1, int colored);
  void DrawBigPixel(int x0, int y0, int colored);
  void DrawBigLine(int x0, int y0, int x1, int y1, int colored);
  void DrawBigCharAt(int x, int y, char ascii_char, sFONT* font, int colored);
  void DrawBigStringAt(int x, int y, const char* text, sFONT* font, int colored);
  void DrawFilledTriangle(int x0, int y0, int x1, int y1, int x2, int y2, int color);
 protected:
  void fillTriangle(int x0, int y0, int x1, int y1, int x2, int y2, int color);
 //-----NEW 180601----------------------------------------------
 public:
  void DrawEllipse(int x0, int y0, int x1, int y1, int color);
  void DrawFilledEllipse(int x0, int y0, int x1, int y1, int color);

 private:
  unsigned char* image;
  int width;
  int height;
  int rotate;
};

#endif

/* END OF FILE */

