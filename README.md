# RCPled
Individual control of each button LED, the overall backlight and data/power indicators.

Uses millis() function rather than delay(), so shouldnt impact loop timing very much

rcpled.LED(led num (0-9), colour (0 black, 1 red, 2 green, 3 yellow), toggle colour, repitition (0 continuous), flash rate mS)
rcpled.BackLight(0-255 level)
rcpled.PwrLight(0,1)
rcpled.DataLight(0,1)
rcp.led.LEDupdate() writes out changes. Stick this in loop


Use the callback function as per the example to read which key has been pressed 

