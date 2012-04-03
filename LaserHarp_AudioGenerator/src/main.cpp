#include "testApp.h"
#include "ofMain.h"
#include "ofAppGlutWindow.h"

int main( ){

    ofAppGlutWindow window;
    
    // this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	//	ofSetupOpenGL(&window, 640,480, OF_WINDOW);			// <-------- setup the GL context
	ofSetupOpenGL(&window, 640,480, OF_WINDOW);			// <-------- setup the GL context

	ofRunApp( new testApp());

}
