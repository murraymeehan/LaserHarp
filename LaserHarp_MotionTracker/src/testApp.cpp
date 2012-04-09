#include "testApp.h"

testApp::testApp(ofxArgs* args){
	this->args = args;
}

//--------------------------------------------------------------
void testApp::setup() {

	this->args->printArgs();
	this->args->printOpts();
	
	if (this->args->getCount() > 1)
	{	
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
void testApp::draw(){
	detector.draw();
}

//--------------------------------------------------------------
void testApp::update(){
	detector.update();
	
	if (detector.detect()) 
		playEvent();
}

//--------------------------------------------------------------
void testApp::keyPressed  (int key){
	if ( key =='a' || key == 'A' )
		playEvent();

	detector.keyPressed(key);
}

//--------------------------------------------------------------
void testApp::playEvent(){
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
	
	cout << "\n PLUCK DETECTED! "
		<< "\n sending playEvent: " 
		<< stringNum_ << " : " << pluckpos_ << " : " << velocity_ 
		<< " : " << acceleration_ << " : " << time_ << endl;
}


