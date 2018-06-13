//_____D1_class_Ain.h_________________________171129-171129_____
// The class Ain reads an analog value and converts it to
// a voltage using a straight line.
// Default points for the stright line are 0/0.00 and 1023/3.20
// and can be changed according to your requirements. 
// Created by Karl Hartinger, November 29, 2017.
// Released into the public domain.
#ifndef D1_CLASS_AIN_H
#define D1_CLASS_AIN_H
#include "Arduino.h"    

//-------status-------------------------------------------------
#define  AIN_OK              0    // success
#define  AIN_ERR_X_RANGE     1    // x<0 or x>1023
#define  AIN_ERR_X_EQUAL     2    // x1=x2
#define  AIN_ERR_Y_RANGE     3    // y<0
#define  AIN_ERR_Y_EQUAL     4    // y1=y2
#define  AIN_ERR_K_NEG       5    // gradient k must be >0
#define  AIN_ERR_DECIMALS    6    // decimals <0, >20

class Ain {
 protected:
  //-----properties---------------------------------------------
  int    status;
  int    pinAin;
  int    ain_1023;
  int    ain_x1, ain_x2;
  float  ain_y1, ain_y2;
  float  ain_k, ain_d;
  int    decimals;
 public:
  //-----constructor & co---------------------------------------
         Ain();
 protected:
  void   setup();                 // setup properties
  int    calcKD(int x1, float y1, int x2, float y2);
 public:
  //-----get (new) measurement values---------------------------
  int    getStatus();
  int    getAin1023();  
  float  getVoltage();
  String getsVoltage();
  String getsVoltage(int decimal_places);
  //-----get last measurement values (without new measurement)--
  int    getLastAin1023();  
  float  getLastVoltage();
  String getLastsVoltage();
  String getLastsVoltage(int decimal_places);
  //-----reference points for straight line---------------------
  int    setRefPoints(int x1, float y1, int x2, float y2);
  int    setRefPoint1(int x1, float y1);
  int    setRefPoint2(int x2, float y2);
  String getRefPoints();
  //-----some more methods--------------------------------------
  void   setDecimals(int decimal_places);
  int    getDecimals();
};
#endif
