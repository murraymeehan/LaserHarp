#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup() {
	detector.setup();
	//generator.setup();
	TO.start();
}

//--------------------------------------------------------------
void testApp::update(){
	detector.update();
	//generator.update();
	
	if (detector.pluckDetection()) {
		//generator.play(detector.pluckParams);
	}
}

//--------------------------------------------------------------
void testApp::draw(){
	detector.draw();
	//generator.draw();
	TO.draw();
}

void testApp::keyPressed  (int key){
    if (key == 'a'){
        TO.start();
    } else if (key == 's'){
        TO.stop();
    }
	detector.keyPressed(key);
	//generator.play(detector.pluckParams);
}

