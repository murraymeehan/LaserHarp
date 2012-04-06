#include "testApp.h"
#include "ofMain.h"
#include "ofxArgs.h"
#include "ofAppGlutWindow.h"

int main(int argc, char *argv[]){
	ofxArgs* args = new ofxArgs(argc, argv);

	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofAppGlutWindow window;
	ofSetupOpenGL(&window, 640,500, OF_WINDOW);	// <-------- setup the GL context
	
	// this kicks off the running of my app
	ofRunApp(new testApp(args));
	delete args;
}

