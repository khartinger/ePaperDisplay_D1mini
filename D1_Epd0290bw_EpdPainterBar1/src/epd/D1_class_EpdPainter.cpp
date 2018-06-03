//_____D1_class_EpdPainter.cpp________________180401-180601_____
// D1 mini class for painting text and shapes on a waveshare
// e-paper display.
// 180515: utf8ToEpd() added
// 180601: drawEllipse(), getFontWidth(), ... added
//
#include "D1_class_EpdPainter.h"
#include <stdlib.h>

//**************************************************************
//    constructor & co
//**************************************************************

//_____constructor______________________________________________
EpdPainter::EpdPainter(Epd_ &epd){
 this->epd=epd;
 //-----init frame(s) (background)------------------------------
 this->frame1=new unsigned char[epd.height * epd.width / 8];
 if (epd.colors > 2)
  this->frame2=new unsigned char[epd.height * epd.width / 8];
 else
  this->frame2=NULL;
 //-----init canvas---------------------------------------------
 paintBlack=new EpdPaint(this->frame1, epd.width, epd.height);
 if (epd.colors > 2)
  paintColor=new EpdPaint(this->frame2, epd.width, epd.height);
 else
  paintColor=NULL;
 //-----init font-----------------------------------------------
 this->font=NULL;
}

EpdPainter::~EpdPainter() {
    delete[] this->frame1;
    delete[] this->frame2;
 }
//**************************************************************
//    drawing methods
//**************************************************************

void EpdPainter::clear(int color) {
paintBlack->Clear((color==BLACK) ? 1 : 0);
 if (paintColor!=NULL) paintColor->Clear((color==RED) ? 1 : 0);
}

void EpdPainter::setRotation(int rotation)
{
    paintBlack->SetRotate(rotation);
    if (paintColor!=NULL) paintColor->SetRotate(rotation);
}

void EpdPainter::setFont(sFONT* font) {
    this->font = font;
}

void EpdPainter::drawBackgroundFromProgmem(const unsigned char* imageBlack) {
    drawBackgroundFromProgmem(imageBlack, NULL);
}

void EpdPainter::drawBackgroundFromProgmem(const unsigned char* imageBlack, const unsigned char* imageColor)
{
 if(imageBlack!=NULL) {
  int size=epd.width * epd.height / 8;
  for (int i=0; i < size; i++) {
   frame1[i]=pgm_read_byte(&imageBlack[i]);
  }
 }
 if(imageColor!=NULL && frame2!=NULL) {
  int size=epd.width * epd.height / 8;
  for (int i=0; i < size; i++) {
   frame2[i]=pgm_read_byte(&imageColor[i]);
  }
 }
}

void EpdPainter::drawPixel(int x0, int y0, int color)
{
 paintBlack->DrawPixel(x0,y0, (color==BLACK) ? 1 : 0);
 if (paintColor!=NULL) paintColor->DrawPixel(x0,y0, (color==RED) ? 1 : 0);
}

void EpdPainter::drawLine(int x0, int y0, int x1, int y1, int color)
{
 paintBlack->DrawLine(x0,y0,x1,y1, (color==BLACK) ? 1 : 0);
 if (paintColor!=NULL) paintColor->DrawLine(x0,y0,x1,y1, (color==RED) ? 1 : 0);
}

void EpdPainter::drawTriangle(int x0, int y0, int x1, int y1, int x2, int y2, int color)
{
 drawLine(x0, y0, x1, y1, color);
 drawLine(x1, y1, x2, y2, color);
 drawLine(x2, y2, x0, y0, color);
}

void EpdPainter::drawFilledTriangle(int x0, int y0, int x1, int y1, int x2, int y2, int color)
{
 paintBlack->DrawFilledTriangle(x0,y0,x1,y1,x2,y2, (color==BLACK) ? 1 : 0);
 if (paintColor!=NULL) paintColor->DrawFilledTriangle(x0,y0,x1,y1,x2,y2, (color==RED) ? 1 : 0);
}

void EpdPainter::drawRectangle(int x0, int y0, int x1, int y1, int color)
{
 paintBlack->DrawRectangle(x0,y0,x1,y1, (color==BLACK) ? 1 : 0);
 if (paintColor!=NULL) paintColor->DrawRectangle(x0,y0,x1,y1, (color==RED) ? 1 : 0);
}

void EpdPainter::drawFilledRectangle(int x0, int y0, int x1, int y1, int color)
{
 paintBlack->DrawFilledRectangle(x0,y0,x1,y1, (color==BLACK) ? 1 : 0);
 if (paintColor!=NULL) paintColor->DrawFilledRectangle(x0,y0,x1,y1, (color==RED) ? 1 : 0);
}

void EpdPainter::drawCircle(int x0, int y0, int r, int color)
{
 paintBlack->DrawCircle(x0,y0,r, (color==BLACK) ? 1 : 0);
 if (paintColor!=NULL) paintColor->DrawCircle(x0,y0,r, (color==RED) ? 1 : 0);
}

void EpdPainter::drawFilledCircle(int x0, int y0, int r, int color)
{
 paintBlack->DrawFilledCircle(x0,y0,r, (color==BLACK) ? 1 : 0);
 if (paintColor!=NULL) paintColor->DrawFilledCircle(x0,y0,r, (color==RED) ? 1 : 0);
}

void EpdPainter::drawCharAt(int x, int y, char ascii_char, int color) {
 paintBlack->DrawCharAt(x, y, ascii_char, this->font, (color==BLACK) ? 1 : 0);
 if (paintColor!=NULL) paintColor->DrawCharAt(x, y, ascii_char, this->font, (color==RED) ? 1 : 0);
}

void EpdPainter::drawStringAt(int x, int y, String text, int color) {
 char charBuf[text.length()+1];
 text.toCharArray(charBuf, text.length()+1);
 paintBlack->DrawStringAt(x, y, charBuf, this->font, (color==BLACK) ? 1 : 0);
 if (paintColor!=NULL) paintColor->DrawStringAt(x, y, charBuf, this->font, (color==RED) ? 1 : 0);
}

void EpdPainter::drawBigPixel(int x, int y, int color)
{
 paintBlack->DrawBigPixel(x, y, (color==BLACK) ? 1 : 0);
 if (paintColor!=NULL) paintColor->DrawBigPixel(x, y, (color==RED) ? 1 : 0);
}

void EpdPainter::drawBigLine(int x0, int y0, int x1, int y1, int color)
{
 paintBlack->DrawBigLine(x0,y0, x1,y1, (color==BLACK) ? 1 : 0);
 if (paintColor!=NULL) paintColor->DrawBigLine(x0,y0, x1,y1, (color==RED) ? 1 : 0);
}

void EpdPainter::drawBigCharAt(int x, int y, char ascii_char, int color) {
 paintBlack->DrawBigCharAt(x, y, ascii_char, this->font, (color==BLACK) ? 1 : 0);
 if (paintColor!=NULL) paintColor->DrawBigCharAt(x, y, ascii_char, this->font, (color==RED) ? 1 : 0);
}

void EpdPainter::drawBigStringAt(int x, int y, String text, int color) {
 char charBuf[text.length()+1];
 text.toCharArray(charBuf, text.length()+1);
 paintBlack->DrawBigStringAt(x, y, charBuf, this->font, (color==BLACK) ? 1 : 0);
 if (paintColor!=NULL) paintColor->DrawBigStringAt(x, y, charBuf, this->font, (color==RED) ? 1 : 0);
}

//-----New 2018-06-01-------------------------------------------
void EpdPainter::drawEllipse(int x0, int y0, int x1, int y1, int color)
{
 paintBlack->DrawEllipse(x0,y0,x1,y1, (color==BLACK) ? 1 : 0);
 if (paintColor!=NULL) paintColor->DrawEllipse(x0,y0,x1,y1, (color==RED) ? 1 : 0);
}

void EpdPainter::drawFilledEllipse(int x0, int y0, int x1, int y1, int color)
{
 paintBlack->DrawFilledEllipse(x0,y0,x1,y1, (color==BLACK) ? 1 : 0);
 if (paintColor!=NULL) paintColor->DrawFilledEllipse(x0,y0,x1,y1, (color==RED) ? 1 : 0);
}

int EpdPainter::getFontHeight()
{
 Serial.print("EpdPainter::getFontHeight()=");
 if(this->font==NULL) { Serial.println("NULL"); return (0); }
 Serial.println(font->Height);  
 return font->Height;
}

int EpdPainter::getFontWidth() 
{
 if(this->font==NULL) return (0);
 return font->Width;
}

//_____Is a font defined?_______________________________________
bool EpdPainter::isFont() {
 if(this->font==NULL) return false;
 return true;
}

//**************************************************************
//    display methods
//**************************************************************

void EpdPainter::clearDisplay() {
 int size_=epd.height*epd.width/8;
 for(int i=0; i<size_; i++) frame1[i]=0xFF;
 if(epd.colors>2) for(int i=0; i<size_; i++) frame2[i]=0xFF;
}

void EpdPainter::display() {
 epd.display(frame1, frame2);
}

void EpdPainter::displayNoWait() {
 epd.displayNoWait(frame1, frame2);
}

//**************************************************************
// helper functions
//**************************************************************

//_____replace special chars by hex code________________________
String EpdPainter::utf8ToEpd(String s)
{
 String sUnknown=String((char)127);
 String s1="";
 char c1,c2,c3;
 int i, imax=s.length();
 //Serial.print("imax=");Serial.println(imax);
 for(i=0; i<imax; i++)
 {
  c1=s.charAt(i);
  if(c1<128) 
   s1=s1+String(c1);
  else
  {
   c2=s.charAt(++i);
   switch(c1)
   {
    case 0xc2: //-----first byte is C2--------------------------
     switch(c2) {
      case 0xb0: s1=s1+"\xF8"; break;  //degree sign
      case 0xb5: s1=s1+"\xE6"; break;  //micro sign
      case 0xaa: s1=s1+"\xA6"; break;  //feminine ordinal indicator
      case 0xb2: s1=s1+"\xFD"; break;  //superscript 2
      default: s1=s1+sUnknown;
     }
     break;
    case 0xc3: //-----first byte is C3--------------------------
     switch(c2){
      case 0x84: s1=s1+"\x8E"; break;  //A with diaeresis
      case 0x96: s1=s1+"\x99"; break;  //O with diaeresis
      case 0x9c: s1=s1+"\x9A"; break;  //U with diaeresis
      case 0xa4: s1=s1+"\x84"; break;  //a with diaeresis
      case 0xb6: s1=s1+"\x94"; break;  //o with diaeresis
      case 0xbc: s1=s1+"\x81"; break;  //u with diaeresis
      case 0x9f: s1=s1+"\xE1"; break;  //sharp s, beta
      default: s1=s1+sUnknown;
     }
     break;
    case 0xe2: //-----first byte is E2--------------------------
     switch(c2) {
      case 0x82: 
       c3=s.charAt(++i);
       if(c3==0xAC) { s1=s1+"\x9E"; }  // Euro
               else { s1=s1+sUnknown; }
       break;
      default: s1=s1+sUnknown;
     }
     break;
    default: //-----first byte unknown--------------------------
     //s1=s1+sUnknown; break;
     s1=s1+String(c1); i--; break;
   }
  }
 }
 return s1;
}
