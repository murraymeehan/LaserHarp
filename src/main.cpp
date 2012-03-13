
#include "testApp.h"
//========================================================================
#include "ofMain.h"
//========================================================================

////--------------------------
//// utils
//#include "ofConstants.h"
//#include "ofFileUtils.h"
//#include "ofLog.h"
//#include "ofSystemUtils.h"
//#include "ofThread.h"
//#include "ofURLFileLoader.h"
//#include "ofUtils.h"

////--------------------------
//// types
//#include "ofBaseTypes.h"
//#include "ofTypes.h"
//#include "ofColor.h"
//#include "ofPoint.h"
//#include "ofRectangle.h"

////--------------------------
//// math
//#include "ofMath.h"
//#include "ofVectorMath.h"

////--------------------------
//// communication
//#if !defined( TARGET_OF_IPHONE ) & !defined(TARGET_ANDROID)
//        #include "ofSerial.h"
//        #include "ofArduino.h"
//#endif

////--------------------------
//// gl
//#include "ofFbo.h"
//#include "ofGLRenderer.h"
//#include "ofGLUtils.h"
//#include "ofLight.h"
//#include "ofMaterial.h"
//#include "ofShader.h"
//#include "ofTexture.h"
//#include "ofVbo.h"
//#include "ofVboMesh.h"

////--------------------------
//// graphics
//#include "ofCairoRenderer.h"
//#include "ofGraphics.h"
//#include "ofImage.h"
//#include "ofPath.h"
//#include "ofPixels.h"
//#include "ofPolyline.h"
//#include "ofRendererCollection.h"
//#include "ofTessellator.h"
//#include "ofTrueTypeFont.h"

////--------------------------
//// app
//#include "ofBaseApp.h"
//#include "ofAppRunner.h"

////--------------------------
//// audio
//#include "ofSoundStream.h"
//#include "ofSoundPlayer.h"

////--------------------------
//// video
//#include "ofVideoGrabber.h"
//#include "ofVideoPlayer.h"

////--------------------------
//// events
//#include "ofEvents.h"

////--------------------------
//// 3d
//#include "of3dUtils.h"
//#include "ofCamera.h"
//#include "ofEasyCam.h"
//#include "ofMesh.h"
//#include "ofNode.h"

//========================================================================
#include "ofAppGlutWindow.h"

//========================================================================
int main( ){

    ofAppGlutWindow window;
	ofSetupOpenGL(&window, 640,480, OF_WINDOW);			// <-------- setup the GL context
	// 640x480 video feed with 20pixel band on bottom for text display

	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp( new testApp());

}
