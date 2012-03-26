
#include "arrayV.h"
#include "ofMain.h"	
#include "ofAppGlutWindow.h"

int main( ){

    ofAppGlutWindow window;
	
	ofSetupOpenGL(&window, 640,480, OF_WINDOW);			// <-------- setup the GL context
//	ofSetupOpenGL(&window, 1024,768, OF_WINDOW);		// <-------- setup the GL context

	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	
	for (int i=0; i<3;i++) {
		ofRunApp( new arrayV());
	}

}
