#ifndef _TEST_APP
#define _TEST_APP

#include "motionDetector.h"
#include "ofxArgs.h"

#define HOST "localhost"
#define PORT 12358

class testApp : public ofBaseApp{
	private:
		ofxArgs* args;
		string option1, option2;
		bool flag1;
	public:
		testApp(ofxArgs* args);
		void setup();
		void update();
		void draw();
		void keyPressed  (int key);

		void playEvent();
		
		motionDetector detector;
		
		ofxOscSender sender;	
		
};

#endif
