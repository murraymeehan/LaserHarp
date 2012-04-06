#include "testApp.h"

testApp::testApp(ofxArgs* args){
	this->args = args;
}

//--------------------------------------------------------------
void testApp::setup() {
	// for debugging, you can print the arguments and options...
	this->args->printArgs();
	this->args->printOpts();
	
	if (this->args->getCount() > 1)
	{
		// Warning: if getCount() returns 2, this means arg(0) & arg(1) exist, not arg(2). 
		string filename = this->args->getString(1); 
		cout << "File-Reading Status: fileName " << filename 
				<< " received from cmd-line arg!" << endl;
		detector.filename = filename;
	} else {
		cout << "File-Reading Status: incorrect number of command line arguements " 
			<< "were passed, ignoring them." << endl;
		detector.filename = "";
	}
	
	detector.setup();
	
	
}

//--------------------------------------------------------------
void testApp::update(){
	detector.update();
	
//	myfile.open ("handPosition.txt");
	
	if (detector.detect()) {
		cout << "\n PLUCK DETECTED! Position logged to file. ";
	}
		
}

//--------------------------------------------------------------
void testApp::draw(){
	detector.draw();
}

void testApp::keyPressed  (int key){
	detector.keyPressed(key);
}

void testApp::checkForArgs() {
		
}


