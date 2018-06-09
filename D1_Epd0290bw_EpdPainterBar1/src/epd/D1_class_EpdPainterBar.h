//_____D1_class_EpdPainterBar.h_______________180601-180608_____
// D1 mini class that uses EpdPainter-class for painting 
// a bargraph:
// location: (x0,y0) lower left (x1,y1) upper right
// values  : vmin, vmax, subdivisions
#ifndef D1_CLASS_EPDPAINTERBAR_H
#define D1_CLASS_EPDPAINTERBAR_H
#include "Arduino.h"                   // 
#include <stdio.h>
#include "D1_class_EpdPainter.h"       // class EdpPaint

//-----Default values: draw frame with scale lines--------------
#define  BAR_X0_PIXEL           54     // upper left point of
#define  BAR_Y0_PIXEL           25     // frame is (x0/y0) 
#define  BAR_X1_PIXEL           74     // lower rigth point of
#define  BAR_Y1_PIXEL          125     // frame is (x1,y1)
#define  BAR_XSCALELINE          4     // length of scale lines
#define  BAR_COLOR_FRAME     BLACK
#define  BAR_COLOR_BAR       BLACK
#define  BAR_SUBDIV_MAX         20     // max. num subdivisions
//-----distance between frame and bar rectangle in x direction--
#define  BAR_BAR_XGAP_PIXEL      1     // 1 px distance (white)
//-----distance between text and scale line---------------------
#define  BAR_TEXT_XGAP_PIXEL     2
//-----values in "real" world-----------------------------------
#define  BAR_VMIN                0     // minimum value
#define  BAR_VMAX              100     // maximum value
#define  BAR_VSTEP              20     // step size for scaling
#define  BAR_DECIMALS            0
#define  BAR_SCALE_FORMAT    "3.0"     // length.decimals
#define  BAR_SCALE_FORMAT_MAX   10     // max length value
//-----draw scale values (out of the frame!)--------------------
#define  BAR_SCALE_LEFT          1     // values left of frame
#define  BAR_SCALE_RIGHT         2     // values right of frame
#define  BAR_SCALE_BOTH          3     // values both sides
#define  BAR_SCALE_ONLY_LIMITS   4     // draw just limit values
#define  BAR_SCALE_Y_FRAMELIMIT  8     // limit text in yframesize

class EpdPainterBar {
 //-----properties----------------------------------------------
 protected:
  EpdPainter* epdPainter_;             // painter class
  int    x0, y0, x1, y1;               // Edges of bar 
  int    xscaleline;                   // x scale length
  int    xgapbar;                      // x-gap frame - bar
  int    xgaptext;                     // x-gap text - scaleline
  double vmin, vmax, vstep;            // "real" values
  int    colorFrame, colorBar;         // color frame, bar
 //-----constructor & co----------------------------------------
 public:
  EpdPainterBar(EpdPainter* epdPainter1);
  EpdPainterBar(EpdPainter* epdPainter1,
                int x0, int y0, int x1, int y1, 
                int xscaleline, int xgapbar,
                double vmin, double vmax, double vstep,
                int colorFrame=BLACK, int colorBar=BLACK);
  ~EpdPainterBar();
  void setup();
 //-----setter and getter methods-------------------------------
  bool setParams(int x0, int y0, int x1, int y1, 
                 int xscaleline, int xgapbar,
                 double vmin, double vmax, double vstep);
 //-----draw methods--------------------------------------------
  void drawFrame();
  void drawFrame(int colorF);
  void drawBar(double value);
  void drawBar(double value, int colorB);
  void drawScale();
  void drawScale(sFONT* font, String scaleformat);
  void drawScale(sFONT* font, String scaleformat, int style);
 //-----working methods-----------------------------------------
  int    getY(double value);
  String formatV(double value, String scaleformat);
};
#endif
