#include "arrayV.h"
#include "ofMain.h"	
#include "ofAppGlutWindow.h"

int main( ){
	// is a window optional? It must be. 
    ofAppGlutWindow window;
	ofSetupOpenGL(&window, 640,480, OF_WINDOW);			// <-------- setup the GL context
//	ofSetupOpenGL(&window, 1024,768, OF_FULLSCREEN);		// <-------- setup the GL context

	ofRunApp( new arrayV() );

}
