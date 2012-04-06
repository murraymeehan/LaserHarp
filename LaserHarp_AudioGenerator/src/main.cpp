#include "testApp.h"
#include "ofMain.h"
#include "ofAppGlutWindow.h"

int main( ){

    ofAppGlutWindow window;
//	ofSetupOpenGL(&window, 640,480, OF_FULLSCREEN);			// <-------- setup the GL context
	ofSetupOpenGL(&window, 640,480, OF_WINDOW);			// <-------- setup the GL context
	ofRunApp( new testApp());

}
