/*
  RCPled.h - Library for driving LEDs on RCP.
  Created by Richard Burford May 2016.
*/
#ifndef RCPled_h
#define RCPled_h

#include "Arduino.h"

class RCPled
{
  public:
	 RCPled();
	
    void LEDupdate();
    void LED (int num, int col, int Tcol, int rep, int rate);
    void BackLight (int level);
    void PwrLight (bool mode);
    void DataLight (bool mode);

   private:
	

    void SetColour (int i, int col);
    
};

#endif