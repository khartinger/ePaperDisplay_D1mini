//_____D1_class_Ain.cpp_______________________171129-171129_____
// The class Ain reads an analog value and converts it to
// a voltage using a straight line.
// Default points for the stright line are 0/0.00 and 1023/3.20
// and can be changed according to your requirements. 
// Created by Karl Hartinger, November 29, 2017.
// Released into the public domain.
#include "D1_class_Ain.h"

//**************************************************************
//     constructor & co
//**************************************************************

//_____constructor______________________________________________
Ain::Ain() { setup(); }

//_____setup properties_________________________________________
void Ain::setup() {
 pinAin=A0;
 ain_1023=0;
 ain_x1=0;
 ain_y1=0.0;
 ain_x2=1023; 
 ain_y2=3.2;
 decimals=2;
 status=calcKD(ain_x1, ain_y1, ain_x2, ain_y2);
}

//_____calculate straightline equation parameters_______________
int Ain::calcKD(int x1, float y1, int x2, float y2)
{
 if((x1<0) || (x1>1023)) return(status=AIN_ERR_X_RANGE);
 if((x2<0) || (x2>1023)) return(status=AIN_ERR_X_RANGE);
 if((y1<0) || (y2<0))    return(status=AIN_ERR_Y_RANGE);
 if(x2==x1) return(status=AIN_ERR_X_EQUAL);
 if(y2==y1) return(status=AIN_ERR_Y_EQUAL);
 if(((y2-y1)/(x2-x1))<0) return(status=AIN_ERR_K_NEG);
 if(x1<x2) { ain_x1=x1; ain_y1=y1; ain_x2=x2; ain_y2=y2; }
      else { ain_x1=x2; ain_y1=y2; ain_x2=x1; ain_y2=y1; }
 ain_k=(y2-y1)/(x2-x1);
 ain_d=(x2*y1-x1*y2)/(x2-x1);
 return (status=AIN_OK);  
}

//**************************************************************
//     get (new) measurement values
//**************************************************************

//_____return status____________________________________________
int Ain::getStatus() { return status; }

//_____read analog in and return decimal value__________________
int Ain::getAin1023() { return (ain_1023=analogRead(pinAin)); }

//_____read analog in, calculate voltage and return it as float_
float Ain::getVoltage() {
 ain_1023=analogRead(pinAin);
 return(ain_k*ain_1023 + ain_d);
}

//_____read analog in, calculate voltage, return String_________
String Ain::getsVoltage() {
 return(String(getVoltage(), decimals));
}

//_____read analog in, calculate voltage, return String_________
String Ain::getsVoltage(int decimal_places) {
 return(String(getVoltage(), decimal_places));
}

//**************************************************************
//     get last measurement values
//**************************************************************

//_____return last analog in value______________________________
int Ain::getLastAin1023() { return ain_1023; }

//_____calculate last voltage and return it as float____________
float Ain::getLastVoltage() { return(ain_k*ain_1023 + ain_d); }

//_____calculate (old) voltage again, return it as String_______
String Ain::getLastsVoltage() {
 return(String(getLastVoltage(), decimals));
}

//______calculate last voltage, return String (given decimals)__
String Ain::getLastsVoltage(int decimal_places) {
 return(String(getLastVoltage(), decimal_places));
}

//**************************************************************
//     reference points for straight line
//**************************************************************

//_____set both reference points P1 and P2______________________
int Ain::setRefPoints(int x1, float y1, int x2, float y2) {
 return(calcKD(x1, y1, x2, y2));
}

//_____set only reference point P1______________________________
int Ain::setRefPoint1(int x1, float y1) {
 return(calcKD(x1, y1, ain_x2, ain_y2));
}

//_____set only reference point P2______________________________
int Ain::setRefPoint2(int x2, float y2) {
 return(calcKD(ain_x1, ain_y1, x2, y2));
}

//_____return reference points as String________________________
String Ain::getRefPoints()
{
 char s0[15];
 sprintf(s0,"%04d",ain_x1); String sx1=String(s0);
 sprintf(s0,"%04d",ain_x2); String sx2=String(s0);
 String sy1=String(dtostrf(ain_y1,decimals+2,decimals,s0));
 String sy2=String(dtostrf(ain_y2,decimals+2,decimals,s0));
 return (sx1+"/"+sy1+" "+sx2+"/"+sy2);
}

//**************************************************************
//     some more methods
//**************************************************************

//_____set number of decimal places for String return___________
void Ain::setDecimals(int decimal_places) {
 if((decimal_places<0)||(decimal_places>10))
 { status=AIN_ERR_DECIMALS; return; }
 decimals=decimal_places;
}

//_____get number of decimal places_____________________________
int Ain::getDecimals() { return decimals; }
