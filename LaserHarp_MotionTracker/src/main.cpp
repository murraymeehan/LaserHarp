#include "testApp.h"
#include "ofMain.h"
#include "ofxArgs.h"
#include "ofAppGlutWindow.h"

int main(int argc, char *argv[]){
	
	ofAppGlutWindow window;
//	ofSetupOpenGL(&window, 640,480, OF_FULLSCREEN);	// <-------- setup the GL context
	ofSetupOpenGL(&window, 640,480, OF_WINDOW);		// <-------- setup the GL context
	
	ofxArgs* args = new ofxArgs(argc, argv);
	ofRunApp(new testApp(args));
	delete args;
	
}

