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
	{	// Warning: if getCount() returns 2, this means arg(0) & arg(1) exist, not arg(2). 
		string ONIfilename = this->args->getString(1); 
		cout << "File-Reading Status: ONIfilename " << ONIfilename 
				<< " received from cmd-line arg!" << endl;
		detector.ONIfilename = ONIfilename;
		ofSetWindowTitle(ONIfilename);
	} else {
		cout << "File-Reading Status: incorrect number of command line arguements " 
			<< "were passed, ignoring them." << endl;
		detector.ONIfilename = "";
		ofSetWindowTitle("LaserHarp Motion Detector : LIVE");
	}
	
	detector.setup();
	
	sender.setup( HOST, PORT );
	
}

//--------------------------------------------------------------
void testApp::update(){
	detector.update();
	
	if (detector.detect()) {
		
		int stringNum_ = detector.playEvent.stringNum;
		float pluckpos_ = detector.playEvent.pluckpos;
		float velocity_ = detector.playEvent.velocity;
		float acceleration_ = detector.playEvent.acceleration;
		float time_= ofGetElapsedTimef();
		
		// send playEvent (was touchEvent) parameters to audio generator
		ofxOscMessage m;
		m.setAddress( "/laserharp/playEvent" );
		m.addIntArg( detector.playEvent.stringNum );
		m.addFloatArg( detector.playEvent.pluckpos );
		m.addFloatArg( detector.playEvent.velocity );
		m.addFloatArg( detector.playEvent.acceleration );
		m.addFloatArg( time_ );
		sender.sendMessage( m );
		
//		cout << "\n PLUCK DETECTED! "
//			<< "\n sending playEvent: " 
//			<< stringNum_ << " : " << pluckpos_ << " : " << velocity_ 
//			<< " : " << acceleration_ << " : " << time_ << endl;
	}
}

//--------------------------------------------------------------
void testApp::draw(){
	detector.draw();
}

void testApp::keyPressed  (int key){
	if ( key =='a' || key == 'A' ){
		int stringNum_ = detector.playEvent.stringNum;
		float pluckpos_ = detector.playEvent.pluckpos;
		float velocity_ = detector.playEvent.velocity;
		float acceleration_ = detector.playEvent.acceleration;
		float time_= ofGetElapsedTimef();
		
		// send OSC message
		ofxOscMessage m;
		m.setAddress( "/laserharp/playEvent" );
		m.addIntArg( detector.playEvent.stringNum );
		m.addFloatArg( detector.playEvent.pluckpos );
		m.addFloatArg( detector.playEvent.velocity ); // TODO: move scaling to conversion function
		m.addFloatArg( detector.playEvent.acceleration );
		m.addFloatArg( time_ );
		sender.sendMessage( m );
		cout<< "\n PLUCK FORCED! "
			<< "\n sending playEvent: " 
			<< stringNum_ << " : " << pluckpos_ << " : " << velocity_ 
			<< " : " << acceleration_ << " : " << time_ << endl;
		
		
	}

	detector.keyPressed(key);
}


