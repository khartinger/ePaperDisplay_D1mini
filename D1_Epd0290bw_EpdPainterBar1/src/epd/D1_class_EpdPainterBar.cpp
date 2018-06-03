//_____D1_class_EpdPainterBar.cpp_____________180601-180601_____
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
 scaleformat=BAR_SCALE_FORMAT;
 colorFrame=BAR_COLOR_FRAME; 
 colorBar=BAR_COLOR_BAR;
 setup();
}
	
//_____constructor 2____________________________________________
EpdPainterBar::EpdPainterBar( EpdPainter *epdPainter1,
                int x0, int y0, int x1, int y1, 
                int xscaleline, int xgapbar,
                double vmin, double vmax, double vstep,
                String scaleformat,
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
 this->scaleformat=scaleformat;
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
 //-----set default font----------------------------------------
 if(!this->epdPainter_->isFont()) setFont(&Font12);
}

//**************************************************************
//    setter and getter methods
//**************************************************************
bool EpdPainterBar::setParams(int x0, int y0, int x1, int y1, 
                 int xscaleline, int xgapbar,
                 double vmin, double vmax, double vstep,
                 String scaleformat)
{
 this->x0=x0; this->y0=y0;
 this->x1=x1; this->y1=y1;
 this->xscaleline=xscaleline; 
 this->xgapbar=xgapbar;
 this->vmin=vmin; this->vmax=vmax; 
 this->vstep=vstep;
 this->scaleformat=scaleformat;
 setup();
}

//_____set font just for scaling________________________________
void EpdPainterBar::setFont(sFONT* font)
{
 this->epdPainter_->setFont(font);
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

//_____draw scale with default value SCALE_BOTH_________________
void EpdPainterBar::drawScale() { drawScale(BAR_SCALE_BOTH); }

//_____draw scale with given style______________________________
void EpdPainterBar::drawScale(int style) {
 char cValue[BAR_SCALE_FORMAT_MAX+7];
 int fontheight_=epdPainter_->getFontHeight(); 
 int fontwidth_=epdPainter_->getFontWidth(); 
 int imax;
 String sFormat="%"+scaleformat+"f\0";
 //-----check input values--------------------------------------
 if(style<1) return;
 if(fontheight_<1) return;
 if(vmax==vmin) return;
 //-----calculate text x-position(s)----------------------------
 int textlen=0;
 int pospoint=scaleformat.indexOf(".");
 if(pospoint==0) return;
 if(pospoint<0)  textlen=scaleformat.toInt();
 else textlen=scaleformat.substring(0,pospoint).toInt();
 if((textlen<1)||(textlen>BAR_SCALE_FORMAT_MAX)) return;
 int xleft=x0-xgaptext-textlen*fontwidth_;
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
 //-----draw limit values---------------------------------------
 sprintf(cValue,sFormat.c_str(),vmin);
 if((style & BAR_SCALE_LEFT)>0)
  epdPainter_->drawStringAt(xleft,ymintext,String(cValue));
 if((style & BAR_SCALE_RIGHT)>0)
  epdPainter_->drawStringAt(xright,ymintext,String(cValue));
 sprintf(cValue,sFormat.c_str(),vmax);
 if((style & BAR_SCALE_LEFT)>0)
  epdPainter_->drawStringAt(xleft,ymaxtext,String(cValue));
 if((style & BAR_SCALE_RIGHT)>0)
  epdPainter_->drawStringAt(xright,ymaxtext,String(cValue));
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
   sprintf(cValue,sFormat.c_str(),v);
   String sV="";
   int j=0; while(cValue[j]!=0) sV=sV+String(cValue[j++]);
   if((style & BAR_SCALE_LEFT)>0)
    epdPainter_->drawStringAt(xleft,y,sV);
   if((style & BAR_SCALE_RIGHT)>0)
    epdPainter_->drawStringAt(xright,y,sV);
  }
 }
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