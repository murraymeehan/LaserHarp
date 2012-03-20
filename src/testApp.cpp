#include "testApp.h"

void testApp::setup(){
		detector.setup();
	}
	
void testApp::update(){
	detector.update();
	if (detector.pluckDetection()) {
		generator.start(); //detector.pluckParams
	}
}

void testApp::draw(){
	detector.draw();
}

void testApp::keyPressed  (int key){

	switch (key) {
		case 'A':
		case 'a':
			generator.start();
			break;
		case 'S':
		case 's':
			generator.stop();
			break;
		case 'Q':
		case 'q':
			OF_EXIT_APP(0);
			break;
		default:
			break;
	}
}
