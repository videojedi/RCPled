//Example LED 
//rcpled.LED(led num (0-9), colour (0 black, 1 red, 2 green, 3 yellow), toggle colour, repitition (0 continuous), flash rate mS)



#include <RCPled.h>

RCPled rcpled;

void setup() {


  
rcpled.LED(0,2,0,0,500);
rcpled.LED(1,1,3,0,100);
rcpled.LED(2,3,0,0,500);
rcpled.LED(3,2,2,0,0);
rcpled.LED(4,2,3,0,50);
rcpled.LED(5,1,0,0,2000);
rcpled.LED(6,3,1,0,1000);
rcpled.LED(7,1,2,0,750);
rcpled.LED(8,2,0,0,500);
rcpled.LED(9,3,1,0,1500);

rcpled.BackLight(255);
rcpled.PwrLight(1);
rcpled.DataLight(1);

}

void loop() {

  rcpled.LEDupdate();
}
