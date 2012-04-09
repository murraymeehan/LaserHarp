#pragma once

#include "ofMain.h"
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
		
 		int	byte1;
		int	byte2;
		int	byte3;
	
		void 	setupPort(string device, int commrate);
		void 	setupPort(string device);
		void 	setupPort();
		void 	reconfigureScale(int scale);
		
		ofSerial serial;
		bool 	bSendSerialMessage;
		
		// OSC
		void 	checkMessage();
		ofxOscReceiver	receiver;

		int	current_msg_string;
		string	msg_strings[NUM_MSG_STRINGS];
		float	timers[NUM_MSG_STRINGS];
		
};

