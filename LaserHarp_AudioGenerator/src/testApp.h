#include "LaserHarp_AudioGenerator.h"
#include <iostream>

#include "ofxOsc.h"

// listen on port 12345
#define PORT 12345
#define NUM_MSG_STRINGS 20

class testApp : public ofBaseApp{

public:
	void setup();
	void update();
	void draw();
	void keyPressed  (int key);
	
	// audio synthesis
	LaserHarp_AudioGenerator generator;
	
	// OSC messaging
	void checkMessage();
	ofxOscReceiver	receiver;
	int				current_msg_string;
	string		msg_strings[NUM_MSG_STRINGS];
	float			timers[NUM_MSG_STRINGS];
	
	// plotting data
	void plotData();
	ofPolyline linePluckPos,lineStringNum, lineVelocity,lineLoss;
	int lineCounter;
	int* boxTints;
	
	bool isShowOnscreenInstructions;
	
	
};
