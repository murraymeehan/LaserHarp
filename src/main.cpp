
#include "testApp.h"
#include "ofMain.h"
//#include "ofConstants.h"
//#include "ofFileUtils.h"
//#include "ofLog.h"
//#include "ofSystemUtils.h"
//#include "ofThread.h"
//#include "ofURLFileLoader.h"
//#include "ofUtils.h"
//#include "ofMath.h"
//#include "ofVectorMath.h"
//#include "ofEvents.h"
#include "ofAppGlutWindow.h"

int main( ){

    ofAppGlutWindow window;
	ofSetupOpenGL(&window, 640,480, OF_WINDOW);			// <-------- setup the GL context
	// 640x480 video feed with 20pixel band on bottom for text display

	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp( new testApp());

}
