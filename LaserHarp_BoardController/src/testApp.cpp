#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){	 


	bSendSerialMessage = false;
	byte1 = 0x00;
	byte2 = 0x00;
	byte3 = 0x00;

	//----------------------------------- 
	serial.listDevices();
	vector <ofSerialDeviceInfo> deviceList = serial.getDeviceList();
	
	//setupPort();
		// note:
	// this should be set to whatever com port
	// your serial device is connected to.
	// (ie, COM4 on a pc, /dev/tty.... on linux, /dev/tty... on a mac)
	// arduino users check in arduino app....
	
	//serial.setup("COM4");  						  // windows example
	//serial.setup("/dev/tty.usbserial-A8004JtY",9600); // mac osx example
	serial.setup("/dev/ttyUSB0", 9600);			  //linux example
	
	
	// listen on the given port
	cout << "listening for osc messages on port " << PORT << "\n";
	
	receiver.setup( PORT );
	
	current_msg_string = 0;


}

//--------------------------------------------------------------
void testApp::setupPort(){
	string device = "/dev/ttyUSB0";
	int commrate = 9600;
	setupPort(device, commrate);
}
void testApp::setupPort(string device){
	setupPort(device,9600);
}
void testApp::setupPort(string device, int commrate){
	//set the comm rate before setting port
	serial.setup(0, 9600); 
	

	serial.setup(device, commrate);			  //linux example
}

//--------------------------------------------------------------
void testApp::update(){
	
	if (bSendSerialMessage){
		
		serial.writeByte(byte1);
		serial.writeByte(byte2);
		serial.writeByte(byte3);
		serial.writeByte(0x00);
		
		bSendSerialMessage = false;
		
	}
	
	checkMessage();
}

//--------------------------------------------------------------
void testApp::draw() {
	string buf;
	buf = "listening for osc messages on port" + ofToString( PORT );
	ofDrawBitmapString( buf, 10, 20 );

	for ( int i=0; i<NUM_MSG_STRINGS; i++ )
	{
		ofDrawBitmapString( msg_strings[i], 10, 40+15*i );
	}

}

//--------------------------------------------------------------
void testApp::keyPressed  (int key){ 
	//generate a chromatic scale
	if (key == 'c'){
		reconfigureScale('c');
	}
	//generate a  major pentatonic scale
	if (key == 'p'){
		reconfigureScale('p');
	}

}

void testApp::reconfigureScale(int scale) {
	switch (scale) {
	case 'c': // chromatic scale
		byte1 = 0xFF;
		byte2 = 0xFF;
		byte3 = 0xFF;
		bSendSerialMessage = true;
		break;
	default:
	case 'p' // major pentatonic scale
		byte1 = 0x29;
		byte2 = 0x52;
		byte3 = 0x95;
		bSendSerialMessage = true;
		break;
	}
}

void testApp::checkMessage() {
	// hide old messages
	for ( int i=0; i<NUM_MSG_STRINGS; i++ )
	{
		if ( timers[i] < ofGetElapsedTimef() )
			msg_strings[i] = "";
	}

	while( receiver.hasWaitingMessages() )
	{
		ofxOscMessage m;
		receiver.getNextMessage( &m );

		// check for reconfigure event message
		if ( m.getAddress() == "/laserboard/reactivate" )
		{
			reconfigureScale();
		}
		// check for reconfigure event message
		if ( m.getAddress() == "/laserboard/port" )
		{
			// reconfigure laser board
			byte1 = m.getArgAsInt32( 0 );
			byte2 = m.getArgAsInt32( 1 );
			byte3 = m.getArgAsInt32( 2 );
			bSendSerialMessage = true;
		}
		// unrecognized message: display on the bottom of the screen
		string msg_string;
		msg_string = m.getAddress();
		msg_string += ": ";
		for ( int i=0; i<m.getNumArgs(); i++ )
		{
			// get the argument type
			msg_string += m.getArgTypeName( i );
			msg_string += ":";
			// display the argument - make sure we get the right type
			if( m.getArgType( i ) == OFXOSC_TYPE_INT32 )
				msg_string += ofToString( m.getArgAsInt32( i ) );
			else if( m.getArgType( i ) == OFXOSC_TYPE_FLOAT )
				msg_string += ofToString( m.getArgAsFloat( i ) );
			else if( m.getArgType( i ) == OFXOSC_TYPE_STRING )
				msg_string += m.getArgAsString( i );
			else
				msg_string += "unknown";
		}
		
		// add to the list of strings to display
		msg_strings[current_msg_string] = msg_string;
		timers[current_msg_string] = ofGetElapsedTimef() + 5.0f;
		current_msg_string = ( current_msg_string + 1 ) % NUM_MSG_STRINGS;
		// clear the next line
		msg_strings[current_msg_string] = "";

	}

}

