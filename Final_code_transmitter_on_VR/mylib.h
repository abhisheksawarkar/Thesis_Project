#ifndef mylib_h
#define mylib_h

#include "Arduino.h"
#include "Wire.h"

class mylib
{
  public:
	int digitalSmooth(int,int *);
	int calib_magneto();
	int calib_acc();
	void green_led_blink();
	void violet_special();
	void normal_working();
	void transmitting_led();
};

#endif
