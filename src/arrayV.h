#ifndef _ARRAYV
#define _ARRAYV

#ifndef _PLUCK_PARAMS
	#define _PLUCK_PARAMS
typedef struct {
	float r;
	float z;
	float stringNum; 
}   pluckParameters;
#endif

#include "motionDetector.h"
#include "soundGenerator.h"
#include "arduinoSerial.h"

class arrayV : public ofBaseApp{

public:
	void setup();
	void update();
	void draw();
	void keyPressed  (int key);
	
	motionDetector detector;
	soundGenerator generator;
	arduinoSerial controller;
};

#endif
