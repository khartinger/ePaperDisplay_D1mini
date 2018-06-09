//_____D1_class_EpdPainterBar.cpp_____________180601-180608_____
// D1 mini class that extends EpdPainter-class for painting 
// text and shapes on a waveshare e-paper display:
// * bargraph
//
// Class NEEDs a epdPainter OBJECT, to be defined before!
#include "D1_class_EpdPainterBar.h"
#include <string.h>

//**************************************************************
//    constructor & co
//**************************************************************

//_____constructor 1____________________________________________
EpdPainterBar::EpdPainterBar(EpdPainter *epdPainter1) {
 this->epdPainter_=epdPainter1;
 //-----set default values--------------------------------------
 x0=BAR_X0_PIXEL; y0=BAR_Y0_PIXEL;
 x1=BAR_X1_PIXEL; y1=BAR_Y1_PIXEL;
 xscaleline=BAR_XSCALELINE;
 xgapbar=BAR_BAR_XGAP_PIXEL;
 vmin=BAR_VMIN;
 vmax=BAR_VMAX;
 vstep=BAR_VSTEP;
 colorFrame=BAR_COLOR_FRAME; 
 colorBar=BAR_COLOR_BAR;
 setup();
}
	
//_____constructor 2____________________________________________
EpdPainterBar::EpdPainterBar( EpdPainter *epdPainter1,
                int x0, int y0, int x1, int y1, 
                int xscaleline, int xgapbar,
                double vmin, double vmax, double vstep,
                int colorFrame, int colorBar)
{
 this->epdPainter_=epdPainter1;
 Serial.print("this->epdPainter_=");
 
 if((this->epdPainter_)==NULL) Serial.println("NULL");
 else
 {
  Serial.println("NOT NULL");
 }
 this->x0=x0; this->y0=y0;
 this->x1=x1; this->y1=y1;
 this->xscaleline=xscaleline;
 this->xgapbar=xgapbar;
 this->vmin=vmin; this->vmax=vmax;
 this->vstep=vstep;
 this->colorFrame=colorFrame; this->colorBar=colorBar;
 setup();
}

//_____Destructor_______________________________________________
EpdPainterBar::~EpdPainterBar() {};

//______________________________________________________________
void EpdPainterBar::setup() {
 int i;
 double v;
 //-----check draw values---------------------------------------
 if(y1<y0)        { i=y0; y0=y1; y1=i; }
 if(x1<x0)        { i=x0; x0=x1; x1=i; }
 if(xscaleline<0) xscaleline=0;
 if(xgapbar<0)    xgapbar=0;
 //-----check "real" values-------------------------------------
 if(vmin>vmax) { v=vmin; vmin=vmax; vmax=v; }
 if(vstep<=0) vstep=vmax-vmin;
 if(((vmax-vmin)/vstep) > BAR_SUBDIV_MAX)
 {
  vstep=(vmax-vmin)/BAR_SUBDIV_MAX;
 }
 xgaptext=BAR_TEXT_XGAP_PIXEL;
}

//**************************************************************
//    setter and getter methods
//**************************************************************
bool EpdPainterBar::setParams(int x0, int y0, int x1, int y1, 
                 int xscaleline, int xgapbar,
                 double vmin, double vmax, double vstep)
{
 this->x0=x0; this->y0=y0;
 this->x1=x1; this->y1=y1;
 this->xscaleline=xscaleline; 
 this->xgapbar=xgapbar;
 this->vmin=vmin; this->vmax=vmax; 
 this->vstep=vstep;
 setup();
}

//**************************************************************
//    draw methods
//**************************************************************

//______________________________________________________________
void EpdPainterBar::drawFrame()
{
 int x2=x0+xscaleline;
 int x3=x1-xscaleline;
 int x,y; 
 double wert;
 //-----check input values--------------------------------------
 if(vmax==vmin) return;
 if(vstep<=0) return;
 if(y1==y0) return;
 if(y1<y0) { y=y0; y0=y1; y1=y; }
 if(x1<x0) { x=x0; x0=x1; x1=x; }
 //-----frame---------------------------------------------------
 epdPainter_->drawLine(x0,y0,x1,y0,colorFrame);
 epdPainter_->drawLine(x0,y1,x1,y1,colorFrame);
 epdPainter_->drawRectangle(x2,y0,x3,y1,colorFrame);
 //-----scale lines---------------------------------------------
 int imax=(vmax-vmin)/vstep;
 if(imax<1) imax=1;
 if(imax>BAR_SUBDIV_MAX) imax=BAR_SUBDIV_MAX;
 for(int i=1; i<imax; i++)
 {
  y=getY(vmin+i*(vmax-vmin)/imax);
  epdPainter_->drawLine(x0,y,x2,y,colorFrame);
  epdPainter_->drawLine(x3,y,x1,y,colorFrame);
 }
}

//______________________________________________________________
void EpdPainterBar::drawFrame(int colorF)
{
 if(vmax==vmin) return;
 colorFrame=colorF;
 drawFrame();
}

//______________________________________________________________
void EpdPainterBar::drawBar(double value) {
 if(vmax==vmin) return;
 int y=getY(value);
 int x2=x0+xscaleline+1+xgapbar;
 int x3=x1-xscaleline-1-xgapbar;
 epdPainter_->drawFilledRectangle(x2,y1,x3,y,colorBar);
}

//______________________________________________________________
void EpdPainterBar::drawBar(double value, int colorB) {
 colorBar=colorB;
 drawBar(value);
}

//_____draw scale with default value decimalssSCALE_BOTH________
void EpdPainterBar::drawScale() {
 drawScale(epdPainter_->getFont(), BAR_SCALE_FORMAT, BAR_SCALE_BOTH); 
}

//_____draw scale with default value SCALE_BOTH_________________
 void EpdPainterBar::drawScale(sFONT* font, String scaleformat){
 drawScale(font, scaleformat, BAR_SCALE_BOTH); 
}

//_____draw scale with given style______________________________
void EpdPainterBar::drawScale(sFONT* font, String scaleformat, int style)
{
 char cValue[BAR_SCALE_FORMAT_MAX+7];
 int fontheight_;
 int fontwidth_;
 int imax;
 String sV;
 sFONT* pFontold;
 //-----check input values--------------------------------------
 if(font==NULL) return;
 fontheight_=font->Height; 
 fontwidth_=font->Width; 
 if(style<1) return;
 if(fontheight_<1) return;
 if(vmax==vmin) return;
 //-----calculate text x-position(s)----------------------------
 sV=formatV(vmin, scaleformat);
 int xleft=x0-xgaptext-sV.length()*fontwidth_;
 if(xleft<0) xleft=0;
 int xright=x1+xgaptext;
 //-----calculate min/max text y-position(s) (upper left)-------
 int yascenders=fontheight_/12;
 int ydescenders=(7*fontheight_-12)/20;
 int ymiddle=(7*fontheight_+18)/20;
 //-----limit texts inside y frame values-----------------------
 int ymintext=getY(vmin) - fontheight_ + ydescenders;
 int ymaxtext=getY(vmax) - yascenders;
 if((style & BAR_SCALE_Y_FRAMELIMIT)==0) // limits outside frame
 {
  ymintext=getY(vmin)-ymiddle;
  ymaxtext=getY(vmax)-ymiddle;
 }
 //-----save actual font----------------------------------------
 pFontold=epdPainter_->getFont();
 epdPainter_->setFont(font);
 //-----draw limit values---------------------------------------
 sV=formatV(vmin, scaleformat);
 if((style & BAR_SCALE_LEFT)>0)
  epdPainter_->drawStringAt(xleft,ymintext,sV);
 if((style & BAR_SCALE_RIGHT)>0)
  epdPainter_->drawStringAt(xright,ymintext,sV);
 sV=formatV(vmax, scaleformat);
 if((style & BAR_SCALE_LEFT)>0)
  epdPainter_->drawStringAt(xleft,ymaxtext,sV);
 if((style & BAR_SCALE_RIGHT)>0)
  epdPainter_->drawStringAt(xright,ymaxtext,sV);
 //-----draw subscale values------------------------------------
 if((style & BAR_SCALE_ONLY_LIMITS)==0)
 {
  imax=(vmax-vmin)/vstep;
  if(imax>BAR_SUBDIV_MAX)
  {
   imax=BAR_SUBDIV_MAX;
   vstep=(vmax-vmin)/imax;
  }
  for(int i=1; i<imax; i++)
  {
   double v=0.0+vmin+i*vstep;
   int y=getY(v)-ymiddle;
   sV=formatV(v, scaleformat);
   if((style & BAR_SCALE_LEFT)>0)
    epdPainter_->drawStringAt(xleft,y,sV);
   if((style & BAR_SCALE_RIGHT)>0)
    epdPainter_->drawStringAt(xright,y,sV);
  }
 }
 //-----restore old font----------------------------------------
 epdPainter_->setFont(pFontold);
}

//**************************************************************
//    working methods
//**************************************************************
//______________________________________________________________
int EpdPainterBar::getY(double value)
{
 if(vmax-vmin==0) return y0;
 if(value<vmin) value=vmin;
 if(value>vmax) value=vmax;
 int y=0.0+y1+(y0-y1)*(value-vmin)/(vmax-vmin);
 return y;
}

//_____format String as len.decimals____________________________
String EpdPainterBar::formatV(double value, String scaleformat)
{
 int textlen=0, decimals=0;
 int pospoint=scaleformat.indexOf(".");
 if(pospoint==0) return "";            // no length value
 if(pospoint<0) 
  textlen=scaleformat.toInt();         // only length
 else
 {
  textlen=scaleformat.substring(0,pospoint).toInt();
  decimals=scaleformat.substring(pospoint+1).toInt();
 }
 if((textlen<1)||(textlen>BAR_SCALE_FORMAT_MAX)) return "";
 if(pospoint>0)
 {
  if(textlen<=(decimals+1)) return "";
 }
 String sV=String(value, decimals);
 //-----number right-justified---------------------------------- 
 for(int i=0; i<textlen-sV.length(); i++) sV=" "+sV;
 return sV;
}