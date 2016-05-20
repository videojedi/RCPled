/*
  RCPled.h - Library for driving LEDs on RCP.
  Created by Richard Burford May 2016.

Added a lot of stuff from 
* by Ozan Oner
* a library for keypad.
* http://www.cooking-hacks.com/keypad-12-button


*/
#ifndef RCPled_h
#define RCPled_h

#include "Arduino.h"

#define KP2_SCAN_INTERVAL 50


#define KP2_KEYUP 0
#define KP2_PENDING 1
#define KP2_KEYDOWN 2


const char kp2DefaultLayout[4][3] = {
  {'5','9','3'},
  {'6','0','4'},
  {'7','1','#'},
  {'8','2','*'}
};

const uint8_t kp2DefaultStateHolder[3] = {0, 0, 0};

const int kp2RowPins[4] = {31,40,42,48}; 
const int kp2ColPins[3] = {29,46,22};   



class RCPled
{
  public:
	 RCPled();
	 
	void setLayout(int rowc, int colc, char **layout, uint8_t *keyState, \
			int *rowp, int *colp, uint8_t useInternalPullup);
	
    void LEDupdate();
    void LED (int num, int col, int Tcol, int rep, int rate);
    void BackLight (int level);
    void PwrLight (bool mode);
    void DataLight (bool mode);
    

    void setPressCallback(void (*callback)(char keyChar, uint8_t updown));

   private:
	

    void SetColour (int i, int col);

	int rowc; // row count
	int colc; // column count
	char *layout; // keypad characters
	int *rowp; // row pins
	int *colp; // column pins
	uint8_t *keyState; // state pointer

	unsigned long nextTime; // last reading time

	// user provided press handler
	void (*pressCallback)(char keyChar, uint8_t state);

	// internal wrapper to call pressCallback
	void callPressCallback(int r, int c, uint8_t state);

	// filters state from keyState accd. to stateIdx
	uint8_t getKeyState(int stateIdx);
	// sets state on keyState
	void setKeyState(int stateIdx, uint8_t state);
    
};

#endif