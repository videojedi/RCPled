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
	this->pressCallback = NULL;
	this->nextTime =0;
    
		this->setLayout(4, 3, (char **)kp2DefaultLayout, (uint8_t *)kp2DefaultStateHolder, \
				(int *)kp2RowPins, (int *)kp2ColPins, 1);

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

void RCPled::setLayout(int rowc, int colc, char **l, uint8_t *keyState, \
		int *rowp, int *colp, uint8_t useInternalPullup) {
	this->rowc = rowc;
	this->colc = colc;
	this->layout = (char *)l;
	this->keyState = keyState;
	this->rowp = rowp;
	this->colp = colp;

	// set row pins as INPUT
	for(int i=0; i<this->rowc; i++) {
		pinMode(this->rowp[i], INPUT);
		if(useInternalPullup)
			digitalWrite(this->rowp[i], HIGH); // set pullups
	}

	// set column pins as OUTPUT
	for(int i=0; i<this->colc; i++) {
		pinMode(this->colp[i], OUTPUT);
		digitalWrite(this->colp[i], HIGH); // set high on column pin
	}
	this->nextTime = millis()+KP2_SCAN_INTERVAL;
}


void RCPled::LED(int num, int col, int Tcol, int rep, int rate) {
  LEDfunc [0] [num] = col;
  LEDfunc [1] [num] = Tcol;
  LEDfunc [2] [num] = rep;
  LEDfunc [3] [num] = rate;

}




void RCPled::LEDupdate() {
	char result;
  unsigned long currentMillis = millis();

	if(millis() < this->nextTime)
		return;

	uint8_t state;

	for(int c=0, stateIdx=0; c<this->colc; c++) {
		// send LOW on the column, and check for LOW on every row
		digitalWrite(this->colp[c], LOW);

		for(int r=0; r<this->rowc; r++, stateIdx+=2) {
			// get state for key[r,c]
			state = this->getKeyState(stateIdx);
			// key[r,c] is not pressed
			if(digitalRead(this->rowp[r])) {
				if(state == KP2_KEYDOWN) 
					this->callPressCallback(r, c, KP2_KEYUP);
				if(state != KP2_KEYUP)
					this->setKeyState(stateIdx, KP2_KEYUP);
			}
			else { // pressed
				if(state == KP2_KEYUP) 
					this->setKeyState(stateIdx, KP2_PENDING);
				else if(state == KP2_PENDING) {
					this->setKeyState(stateIdx, KP2_KEYDOWN);
					this->callPressCallback(r, c, KP2_KEYDOWN);
				}
			}
		}

		// set column to HIGH again
		digitalWrite(this->colp[c], HIGH);
	}

	this->nextTime = millis()+KP2_SCAN_INTERVAL;

  
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


void RCPled::setPressCallback(void (*callback)(char keyChar, uint8_t updown)) {
	this->pressCallback = callback;
}


void RCPled::callPressCallback(int r, int c, uint8_t updown) {
	// call press callback if exists
	if(this->pressCallback != NULL) {
		char keyChar = this->layout[r*this->colc+c];
		this->pressCallback(keyChar, updown);
	}
}

uint8_t RCPled::getKeyState(int stateIdx) {
	uint8_t sb = this->keyState[stateIdx/8];
	int pos = stateIdx%8;
	return (sb>>pos)&0x03;
}

void RCPled::setKeyState(int stateIdx, uint8_t state) {
	uint8_t *sb = this->keyState+stateIdx/8;
	int pos = stateIdx%8;
	bitWrite(*sb, pos, bitRead(state, 0));
	bitWrite(*sb, pos+1, bitRead(state, 1));
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
