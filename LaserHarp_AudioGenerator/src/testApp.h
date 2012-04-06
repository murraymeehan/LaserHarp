#include "audioGenerator.h"
#include "ofxOsc.h"
#include <iostream>

// listen for sound control messages on port 12358
#define PORT 12358
#define NUM_MSG_STRINGS 20

class testApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();
		void keyPressed(int key);
	
		// audio synthesis
		audioGenerator generator;
	
		// OSC messaging
		ofxOscReceiver	receiver;
		void 			checkMessage();
		int				current_msg_string;
		string			msg_strings[NUM_MSG_STRINGS];
		float			timers[NUM_MSG_STRINGS];
	
		// On-screen display
		bool displayInstructions;
		ofPolyline linePluckPos,lineStringNum, lineVelocity,lineLoss;
		int lineCounter;
		int* boxTints;
	
};
