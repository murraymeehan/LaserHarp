#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup() {
	detector.setup();
	//TO.start();
}

//--------------------------------------------------------------
void testApp::update(){
	detector.update();
//	TO.update();
	
	if (detector.pluckDetection()) {
		//generator.play(detector.pluckParams);
	}
}

//--------------------------------------------------------------
void testApp::draw(){
	detector.draw();
	TO.draw();
}

void testApp::keyPressed  (int key){
    if (key == 'a'){
        TO.start();
    } else if (key == 's'){
        TO.stop();
    }
	//detector.keyPressed(key);
	//TO.play(detector.pluckParams);
}

