#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup() {
	detector.setup();
	generator.setup();
}

//--------------------------------------------------------------
void testApp::update(){
	detector.update();
	generator.update();
	
	if (detector.pluckDetection()) {
		generator.play(detector.pluckParams);
	}
}

//--------------------------------------------------------------
void testApp::draw(){
	detector.draw();
	generator.draw();
}

void testApp::keyPressed  (int key){
	detector.keyPressed(key);
	generator.play(detector.pluckParams);
}

