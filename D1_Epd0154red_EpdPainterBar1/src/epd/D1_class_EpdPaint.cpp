/**
 *  @filename   :   D1_class_EpdPaint.cpp (epdpaint.cpp)
 *  @brief      :   Paint tools
 *  @author     :   Yehui from Waveshare
 *  @updates : by Christian & Karl Hartinger
 *             2018-04-01 Add DrawLine, DrawTriangle, DrawBig...
 *             2018-06-01 Add DrawEllipse
 *  
 *  Copyright (C) Waveshare     September 9 2017
 *  
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

//#include "epdpaint.h"                     // NEW 180401
#include "D1_class_EpdPaint.h"              // NEW 180401

EpdPaint::EpdPaint(unsigned char* image, int width, int height) {
 this->rotate = ROTATE_0;
 this->image = image;
 /* 1 byte = 8 pixels, so the width should be the multiple of 8 */
 this->width = width % 8 ? width + 8 - (width % 8) : width;
 this->height = height;
}

EpdPaint::~EpdPaint() {
}

/**
 *  @brief: clear the image
 */
void EpdPaint::Clear(int colored) {
 for (int x = 0; x < this->width; x++) {
  for (int y = 0; y < this->height; y++) {
   DrawAbsolutePixel(x, y, colored);
  }
 }
}

/**
 *  @brief: this draws a pixel by absolute coordinates.
 *    this function won't be affected by the rotate parameter.
 */
void EpdPaint::DrawAbsolutePixel(int x, int y, int colored) {
 if (x < 0 || x >= this->width || y < 0 || y >= this->height) {
  return;
 }
 if (IF_INVERT_COLOR) {
  if (colored) {
   image[(x + y * this->width) / 8] |= 0x80 >> (x % 8);
  } else {
   image[(x + y * this->width) / 8] &= ~(0x80 >> (x % 8));
  }
 } else {
  if (colored) {
   image[(x + y * this->width) / 8] &= ~(0x80 >> (x % 8));
  } else {
   image[(x + y * this->width) / 8] |= 0x80 >> (x % 8);
  }
 }
}

/**
 *  @brief: Getters and Setters
 */
unsigned char* EpdPaint::GetImage(void) {
 return this->image;
}

int EpdPaint::GetWidth(void) {
 return this->width;
}

void EpdPaint::SetWidth(int width) {
 this->width = width % 8 ? width + 8 - (width % 8) : width;
}

int EpdPaint::GetHeight(void) {
 return this->height;
}

void EpdPaint::SetHeight(int height) {
 this->height = height;
}

int EpdPaint::GetRotate(void) {
 return this->rotate;
}

void EpdPaint::SetRotate(int rotate){
 this->rotate = rotate;
}

/**
 *  @brief: this draws a pixel by the coordinates
 */
void EpdPaint::DrawPixel(int x, int y, int colored) {
 int point_temp;
 if (this->rotate == ROTATE_0) {
  if(x < 0 || x >= this->width || y < 0 || y >= this->height) {
   return;
  }
  DrawAbsolutePixel(x, y, colored);
 } else if (this->rotate == ROTATE_90) {
  if(x < 0 || x >= this->height || y < 0 || y >= this->width) {
    return;
  }
  point_temp = x;
  x = this->width - y;
  y = point_temp;
  DrawAbsolutePixel(x, y, colored);
 } else if (this->rotate == ROTATE_180) {
  if(x < 0 || x >= this->width || y < 0 || y >= this->height) {
    return;
  }
  x = this->width - x;
  y = this->height - y;
  DrawAbsolutePixel(x, y, colored);
 } else if (this->rotate == ROTATE_270) {
  if(x < 0 || x >= this->height || y < 0 || y >= this->width) {
    return;
  }
  point_temp = x;
  x = y;
  y = this->height - point_temp;
  DrawAbsolutePixel(x, y, colored);
 }
}

/**
 *  @brief: this draws a charactor on the frame buffer but not refresh
 */
void EpdPaint::DrawCharAt(int x, int y, char ascii_char, sFONT* font, int colored) {
 int i, j;
 unsigned int char_offset = (ascii_char - ' ') * font->Height * (font->Width / 8 + (font->Width % 8 ? 1 : 0));
 const unsigned char* ptr = &font->table[char_offset];

 for (j = 0; j < font->Height; j++) {
  for (i = 0; i < font->Width; i++) {
   if (*ptr & (0x80 >> (i % 8))) {
    DrawPixel(x + i, y + j, colored);
   }
   if (i % 8 == 7) {
    ptr++;
   }
  }
  if (font->Width % 8 != 0) {
   ptr++;
  }
 }
}

void EpdPaint::DrawBigCharAt(int x, int y, char ascii_char, sFONT* font, int colored) {
 int i, j;
 unsigned int char_offset = (ascii_char - ' ') * font->Height * (font->Width / 8 + (font->Width % 8 ? 1 : 0));
 const unsigned char* ptr = &font->table[char_offset];
 for (j = 0; j < font->Height; j++) {
  for (i = 0; i < font->Width; i++) {
   if (*ptr & (0x80 >> (i % 8))) 
   {
    DrawPixel(x + 2*i,     y + j,     colored);
    DrawPixel(x + 2*i + 1, y + j,     colored);
    DrawPixel(x + 2*i,     y + j + 1, colored);
    DrawPixel(x + 2*i + 1, y + j + 1, colored);
   }

   if (i % 8 == 7) { ptr++; }
  }
  if (font->Width % 8 != 0) { ptr++; }
  y++;
 }
}

/**
*  @brief: this displays a string on the frame buffer but not refresh
*/
void EpdPaint::DrawStringAt(int x, int y, const char* text, sFONT* font, int colored) {
 const char* p_text = text;
 unsigned int counter = 0;
 int refcolumn = x;
 
 /* Send the string character by character on EPD */
 while (*p_text != 0) {
  /* Display one character on EPD */
  DrawCharAt(refcolumn, y, *p_text, font, colored);
  /* Decrement the column position by 16 */
  refcolumn += font->Width;
  /* Point on the next character */
  p_text++;
  counter++;
 }
}

/**
*  @brief: NEW 
*/
void EpdPaint::DrawBigStringAt(int x, int y, const char* text, sFONT* font, int colored) {
 const char* p_text = text;
 unsigned int counter = 0;
 int refcolumn = x;
 
 /* Send the string character by character on EPD */
 while (*p_text != 0) {
  /* Display one character on EPD */
  DrawBigCharAt(refcolumn, y, *p_text, font, colored);
  /* Decrement the column position by 16 */
  refcolumn += 2*font->Width;
  /* Point on the next character */
  p_text++;
  counter++;
 }
}


/**
*  @brief: this draws a horizontal line on the frame buffer
*/
void EpdPaint::DrawHorizontalLine(int x, int y, int line_width, int colored) {
 int i;
 for (i = x; i < x + line_width; i++) {
  DrawPixel(i, y, colored);
 }
}

/**
*  @brief: this draws a vertical line on the frame buffer
*/
void EpdPaint::DrawVerticalLine(int x, int y, int line_height, int colored) {
 int i;
 for (i = y; i < y + line_height; i++) {
  DrawPixel(x, i, colored);
 }
}

/**
*  @brief: this draws a rectangle
*/
void EpdPaint::DrawRectangle(int x0, int y0, int x1, int y1, int colored) {
 int min_x, min_y, max_x, max_y;
 min_x = x1 > x0 ? x0 : x1;
 max_x = x1 > x0 ? x1 : x0;
 min_y = y1 > y0 ? y0 : y1;
 max_y = y1 > y0 ? y1 : y0;
 
 DrawHorizontalLine(min_x, min_y, max_x - min_x + 1, colored);
 DrawHorizontalLine(min_x, max_y, max_x - min_x + 1, colored);
 DrawVerticalLine(min_x, min_y, max_y - min_y + 1, colored);
 DrawVerticalLine(max_x, min_y, max_y - min_y + 1, colored);
}

/**
*  @brief: this draws a filled rectangle
*/
void EpdPaint::DrawFilledRectangle(int x0, int y0, int x1, int y1, int colored) {
 int min_x, min_y, max_x, max_y;
 int i;
 min_x = x1 > x0 ? x0 : x1;
 max_x = x1 > x0 ? x1 : x0;
 min_y = y1 > y0 ? y0 : y1;
 max_y = y1 > y0 ? y1 : y0;
 
 for (i = min_x; i <= max_x; i++) {
   DrawVerticalLine(i, min_y, max_y - min_y + 1, colored);
 }
}

/**
*  @brief: this draws a circle
*/
void EpdPaint::DrawCircle(int x, int y, int radius, int colored) {
 /* Bresenham algorithm */
 int x_pos = -radius;
 int y_pos = 0;
 int err = 2 - 2 * radius;
 int e2;

 do {
  DrawPixel(x - x_pos, y + y_pos, colored);
  DrawPixel(x + x_pos, y + y_pos, colored);
  DrawPixel(x + x_pos, y - y_pos, colored);
  DrawPixel(x - x_pos, y - y_pos, colored);
  e2 = err;
  if (e2 <= y_pos) {
   err += ++y_pos * 2 + 1;
   if(-x_pos == y_pos && e2 <= x_pos) {
     e2 = 0;
   }
  }
  if (e2 > x_pos) {
   err += ++x_pos * 2 + 1;
  }
 } while (x_pos <= 0);
}

/**
*  @brief: this draws a filled circle
*/
void EpdPaint::DrawFilledCircle(int x, int y, int radius, int colored) {
 /* Bresenham algorithm */
 int x_pos = -radius;
 int y_pos = 0;
 int err = 2 - 2 * radius;
 int e2;

 do {
  DrawPixel(x - x_pos, y + y_pos, colored);
  DrawPixel(x + x_pos, y + y_pos, colored);
  DrawPixel(x + x_pos, y - y_pos, colored);
  DrawPixel(x - x_pos, y - y_pos, colored);
  DrawHorizontalLine(x + x_pos, y + y_pos, 2 * (-x_pos) + 1, colored);
  DrawHorizontalLine(x + x_pos, y - y_pos, 2 * (-x_pos) + 1, colored);
  e2 = err;
  if (e2 <= y_pos) {
   err += ++y_pos * 2 + 1;
   if(-x_pos == y_pos && e2 <= x_pos) {
    e2 = 0;
   }
  }
  if(e2 > x_pos) {
   err += ++x_pos * 2 + 1;
  }
 } while(x_pos <= 0);
}

//**************************************************************
// UPDATE April 2018
//**************************************************************

/**
*  @brief: draw a line on the frame buffer
*    New implementation by Christian Hartinger
*/
void EpdPaint::DrawLine(int x0, int y0, int x1, int y1, int colored) {
  int dx;
  int dy;
  dx = x1-x0;
  if (dx < 0) dx = -dx;

  dy = y1-y0;
  if (dy < 0) dy = -dy;

  int sx = x0<x1 ? 1 : -1;
  int sy = y0<y1 ? 1 : -1; 
  int err = (dx>dy ? dx : -dy)/2, e2;
 
  for(;;){
   DrawPixel(x0, y0, colored);
   if (x0==x1 && y0==y1) break;
   e2 = err;
   if (e2 >-dx) { err -= dy; x0 += sx; }
   if (e2 < dy) { err += dx; y0 += sy; }
  }
}

/**
*  @brief: Draw a tripple width, tripple height pixel
*          x0/y0 is center
*    Added by Karl Hartinger
*/
void EpdPaint::DrawBigPixel(int x0, int y0, int colored) {
   DrawPixel(x0-1, y0-1, colored);
   DrawPixel(x0-1, y0,   colored);
   DrawPixel(x0-1, y0+1, colored);
   DrawPixel(x0,   y0-1, colored);
   DrawPixel(x0,   y0,   colored);
   DrawPixel(x0,   y0+1, colored);
   DrawPixel(x0+1, y0-1, colored);
   DrawPixel(x0+1, y0,   colored);
   DrawPixel(x0+1, y0+1, colored);
}

/**
*  @brief: Draw a bigger line (3 pixel)
*    Added by Karl Hartinger
*/
void EpdPaint::DrawBigLine(int x0, int y0, int x1, int y1, int colored) {
  int dx;
  int dy;
  dx = x1-x0;
  if (dx < 0) dx = -dx;

  dy = y1-y0;
  if (dy < 0) dy = -dy;

  int sx = x0<x1 ? 1 : -1;
  int sy = y0<y1 ? 1 : -1; 
  int err = (dx>dy ? dx : -dy)/2, e2;
 
  for(;;){
   DrawBigPixel(x0, y0, colored);
   if (x0==x1 && y0==y1) break;
   e2 = err;
   if (e2 >-dx) { err -= dy; x0 += sx; }
   if (e2 < dy) { err += dx; y0 += sy; }
  }
}

/**
*  @brief: Draw a filled Triangle
*    Added by Karl Hartinger
*/
void EpdPaint::DrawFilledTriangle(int x0, int y0, int x1, int y1, int x2, int y2, int color)
{
 int x3=-1, y3=-1;
 int tmp;
 //-----sort the three vertices by y-coordinate-----------------
 if(y1<y0) { tmp=x0; x0=x1; x1=tmp; tmp=y0; y0=y1; y1=tmp; }
 if(y2<y0) { tmp=x0; x0=x2; x2=tmp; tmp=y0; y0=y2; y2=tmp; }
 if(y2<y1) { tmp=x1; x1=x2; x2=tmp; tmp=y1; y1=y2; y2=tmp; }
 //-----check for trivial cases of triangle---------------------
 if(y1==y2) {
  if(x2<x1) { tmp=x1; x1=x2; x2=tmp; tmp=y1; y1=y2; y2=tmp; }
  fillTriangle(x0, y0, x1, y1, x2, y2, color);
  return;
 }
 if(y0==y1)
 {
  if(x1<x0) { tmp=x0; x0=x1; x1=tmp; tmp=y0; y0=y1; y1=tmp; }
  fillTriangle(x2, y2, x0, y0, x1, y1, color);
  return;
 }
 //-----calculate split point-----------------------------------
 float f=(float)(y1-y0)*(float)(x2-x0)/(float)(y2-y0);
 if(f>0) f=f+0.5; else f=f-0.5;
 x3=x0+(int)f;
 y3=y1;
 if(x3<x1) { tmp=x1; x1=x3; x3=tmp; tmp=y1; y1=y3; y3=tmp; }
 //-----fill triangles------------------------------------------
 fillTriangle(x0, y0, x1, y1, x3, y3, color);
 fillTriangle(x2, y2, x1, y1, x3, y3, color);
}

//-----help methode for DrawFilledTriangle----------------------
void EpdPaint::fillTriangle(int x0, int y0, int x1, int y1, int x2, int y2, int color)
{
 int dxa;
 int dya;
 int dxb;
 int dyb;
 int tmp;
 //-----special case--------------------------------------------
 if(y0==y1 && y0==y2)
 {
  if(x1<x0) {tmp=x1; x1=x0; x0=tmp;}
  if(x1>x2) {tmp=x1; x1=x2; x2=tmp;}
  for(int xi=x0; xi<=x2; xi++) DrawPixel(xi, y0, color);
  return;
 }
 //-----prepare 1. line a---------------------------------------
 dxa = x1-x0;
 if (dxa < 0) dxa = -dxa;
 dya = y1-y0;
 if (dya < 0) dya = -dya;

 int sxa = x0<x1 ? 1 : -1;
 int sya = y0<y1 ? 1 : -1;
 int e2a;
 int erra = (dxa>dya ? dxa : -dya)/2;
 int x0a=x0, y0a=y0;
 //-----prepare 2. line b---------------------------------------
 dxb = x2-x0;
 if (dxb < 0) dxb = -dxb;
 dyb = y2-y0;
 if (dyb < 0) dyb = -dyb;

 int sxb = x0<x2 ? 1 : -1;
 int syb = y0<y2 ? 1 : -1;
 int e2b;
 int errb = (dxb>dyb ? dxb : -dyb)/2;
 int x0b=x0, y0b=y0;
 //-----draw lines, fill vertical lines-------------------------
 int xi=0;
 for(;;){
  for(xi=x0a; xi<=x0b; xi++) DrawPixel(xi, y0a, color);
  int y0aOld=y0a;
  while(y0a==y0aOld)
  {
   if (x0a==x1 && y0a==y1) return; //break
   e2a = erra;
   if (e2a >-dxa) { erra -= dya; x0a += sxa; }
   if (e2a < dya) { erra += dxa; y0a += sya; }
  }
  int y0bOld=y0b;
  while(y0b==y0bOld)
  {
   if (x0b==x1 && y0b==y1) return; //break;
   e2b = errb;
   if (e2b >-dxb) { errb -= dyb; x0b += sxb; }
   if (e2b < dyb) { errb += dxb; y0b += syb; }
  }
 }
}

//**************************************************************
// UPDATE June 2018
//**************************************************************

/**
*  @brief: Draw a ellise (points: upper left, lower right)
*    Added by Karl Hartinger
*/
void EpdPaint::DrawEllipse(int x0, int y0, int x1, int y1, int color)
{
 int xm=(x0+x1)/2, ym=(y0+y1)/2;  // center of the ellipse
 int a=(x1-x0)/2, b=(y1-y0)/2;    // axis of the ellipse
 if(a<0) a=-a;                    // make value positive
 if(b<0) b=-b;                    // make value positive
 int dx = 0, dy = b;              // I. Quadrant from upper left to lower right
 long a2 = a*a, b2 = b*b;
 long err = b2-(2*b-1)*a2, e2;    // error in 1st step
 do {
  DrawPixel(xm+dx, ym+dy, color); // I. Quadrant
  DrawPixel(xm-dx, ym+dy, color); // II. Quadrant
  DrawPixel(xm-dx, ym-dy, color); // III. Quadrant
  DrawPixel(xm+dx, ym-dy, color); // IV. Quadrant
  e2 = 2*err;
  if (e2 <  (2*dx+1)*b2) { dx++; err += (2*dx+1)*b2; }
  if (e2 > -(2*dy-1)*a2) { dy--; err -= (2*dy-1)*a2; }
 } while (dy >= 0);
 while (dx++ < a) {               // break, if flat ellipse (b=1)
  DrawPixel(xm+dx, ym, color);    // -> draw top of ellipse
  DrawPixel(xm-dx, ym, color);
 }
}

void EpdPaint::DrawFilledEllipse(int x0, int y0, int x1, int y1, int color)
{
 int xm=(x0+x1)/2, ym=(y0+y1)/2;  // center of the ellipse
 int a=(x1-x0)/2, b=(y1-y0)/2;    // axis of the ellipse
 if(a<0) a=-a;                    // make value positive
 if(b<0) b=-b;                    // make value positive
 int dx = 0, dy = b;              // I. Quadrant from upper left to lower right
 long a2 = a*a, b2 = b*b;
 long err = b2-(2*b-1)*a2, e2;    // error in 1st step
 do {
  DrawLine(xm-dx, ym+dy, xm+dx, ym+dy, color);
  DrawLine(xm-dx, ym-dy, xm+dx, ym-dy, color);
  e2 = 2*err;
  if (e2 <  (2*dx+1)*b2) { dx++; err += (2*dx+1)*b2; }
  if (e2 > -(2*dy-1)*a2) { dy--; err -= (2*dy-1)*a2; }
 } while (dy >= 0);
 while (dx++ < a) {               // break, if flat ellipse (b=1)
  DrawLine(xm-dx, ym, xm+dx, ym, color);
 }
} 

/* END OF FILE */
