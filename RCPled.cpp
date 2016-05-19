/*
  RCPled.cpp - Library for driving LEDs on RCP.
  Created by Richard Burford May 2016.
*/

#include <Arduino.h>
#include <RCPled.h>

  int pinArrayG[] = {
  39, 41, 43, 32, 34, 23, 25, 27, 24, 26
};  //Green LED pins
int pinArrayR[] = {
  45, 47, 49, 36, 38, 33, 35, 37, 28, 30
};	//Red LED pins

unsigned long LEDfunc [5][10] = {
{0,0,0,0,0,0,0,0,0,0},  //Colour 0
{0,0,0,0,0,0,0,0,0,0},  //Toggle Colour 1
{0,0,0,0,0,0,0,0,0,0},  //Rep 2
{0,0,0,0,0,0,0,0,0,0},  //Rate 3
{0,0,0,0,0,0,0,0,0,0}   //Previous Rate 4
};

int BL = 44;
int PL = 13;
int DL = 12;

RCPled::RCPled()
{
for (int count=0;count<10;count++) {
    pinMode(pinArrayG[count], OUTPUT);
    digitalWrite(pinArrayG[count],HIGH);
    pinMode(pinArrayR[count], OUTPUT);
    digitalWrite(pinArrayR[count],HIGH);
  }
pinMode(BL, OUTPUT);
pinMode(PL, OUTPUT);
pinMode(DL, OUTPUT);
}

void RCPled::LED(int num, int col, int Tcol, int rep, int rate) {
  LEDfunc [0] [num] = col;
  LEDfunc [1] [num] = Tcol;
  LEDfunc [2] [num] = rep;
  LEDfunc [3] [num] = rate;

}

void RCPled::LEDupdate() {
  unsigned long currentMillis = millis();
  
  for (int i = 0; i < 10; i++) {

    if ((LEDfunc [2][i]) >0 ) {        //if rep is not 0
     
    if (currentMillis - (LEDfunc [4][i]) >= (LEDfunc [3][i])) {
      LEDfunc [4][i] = currentMillis;
    
     SetColour(i,LEDfunc [0][i]);
     int tcol = LEDfunc [0][i];           //temp store colour
     LEDfunc [0][i] = LEDfunc [1][i];     //toggle colour 1
     LEDfunc [1][i] = tcol;               //toggle colour 2
   

      LEDfunc [2][i] = (LEDfunc [2][i]) - 1;        //decrease repetitions
      if ((LEDfunc [2][i]) == 0 ) {
        LEDfunc [0][i] = 0;                         //set to black at end of rep
        LEDfunc [1][i] = 0;
      }
    }
    }
    else  {        //if rep = 0
      
    if (currentMillis - (LEDfunc [4][i]) >= (LEDfunc [3][i])) {
      LEDfunc [4][i] = currentMillis;
    
     SetColour(i,LEDfunc [0][i]);
     int tcol = LEDfunc [0][i];           //temp store colour
     LEDfunc [0][i] = LEDfunc [1][i];     //toggle colour 1
     LEDfunc [1][i] = tcol;               //toggle colour 2
    }
    }    
  }
}

void RCPled::SetColour(int i, int col) {
  switch (col)            //switch colour
     {
      case 1 :    //RED
      //Serial.print("RED");
        digitalWrite(pinArrayR[i],LOW);
        digitalWrite(pinArrayG[i],HIGH);
       break;
      case 2 :    //GREEN
      //Serial.print("GREEN");
        digitalWrite(pinArrayR[i],HIGH);
        digitalWrite(pinArrayG[i],LOW);
        break;
      case 3 :
      //Serial.print("YELLOW");
        digitalWrite(pinArrayR[i],LOW);
        digitalWrite(pinArrayG[i],LOW);
        break;
      default:
      //Serial.print("BLACK");
        digitalWrite(pinArrayR[i],HIGH);
        digitalWrite(pinArrayG[i],HIGH);
        break;
     }
}

void RCPled::BackLight(int level){
	
	analogWrite(BL, level);
}

void RCPled::PwrLight(bool mode)
{
	digitalWrite(PL,mode);
}

void RCPled::DataLight(bool mode)
{
	digitalWrite(DL, mode);
}
