#include "testApp.h"

void testApp::setup(){
	ofSetWindowTitle("Audio Generator (LaserHarp)");
	
	ofBackground(0, 0, 0); 
	
	std::cout << "Press 'a','s','d','f','z','x','c','v' to play sounds. \n The top and bottom rows are distinct strings, played at slightly different frequencies. \n Mix them together and see how they interact.";
	
	generator.setup();
	
	cout << "listening for osc messages on port " << PORT << "\n";
	receiver.setup( PORT );

	current_msg_string = 0;
	isShowOnscreenInstructions = false;
	
	
	// visualizations
		// setup()
	boxTints = new int[12];

	for (int i=0;i<12;++i) {
		boxTints[i] = 0;
	}
	
}

void testApp::checkMessage() {
	// hide old messages
	for ( int i=0; i<NUM_MSG_STRINGS; i++ )
	{
		if ( timers[i] < ofGetElapsedTimef() )
			msg_strings[i] = "";
	}

	// check for waiting messages
	while( receiver.hasWaitingMessages() )
	{
		// get the next message
		ofxOscMessage m;
		receiver.getNextMessage( &m );

		// message: display on the bottom of the screen
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
		
		///////////
		// start a PlayEvent for the received touchEventParameters
		int index_ = m.getArgAsInt32( 0 );
		float pos_ = m.getArgAsFloat( 1 );
		float vel_ = m.getArgAsFloat( 2 );
		float acc_ = m.getArgAsFloat( 3 );
		float time_= m.getArgAsFloat( 4 );

		cout << "ready to play playEvent for touchEvent: " 
			<< index_ << " : " << pos_ << " : " << vel_ 
			<< " : " << acc_ << " : " << time_ << endl;
			
		//////////////////////////////
		// Convert from touchEventParameters to playEventParameters
		//////////////////////////////
		double r_ = pos_;
		double z_ = pos_;
		int stringnum_ = index_;
		double loss_;
		double aT = 100;
		if(acc_<aT)
		{	loss_ = 0.999 - (aT-acc_)/(5*3000);	}
		else
		{	loss_ = 0.995;			}
//		double vT = 0.0087499;
		double vT = 300;
		double velocity_;
		if (vel_<vT)
		{	velocity_ = (vel_/vT)*255.0;		}
		else
		{	velocity_ = 255.0;			}
		//////////////////////////////
		
		generator.play(r_, z_, stringnum_, velocity_, loss_);
		
		///////////
		// add to the list of strings to display
		msg_strings[current_msg_string] = msg_string;
		timers[current_msg_string] = ofGetElapsedTimef() + 5.0f;
		current_msg_string = ( current_msg_string + 1 ) % NUM_MSG_STRINGS;
		// clear the next line
		msg_strings[current_msg_string] = "";

	}

}
void testApp::update(){

	checkMessage();
	generator.update();
}

void testApp::draw(){

	plotData();
	
	ofSetColor(255, 255, 255);
	string instructions = "Press 'a','s','d','f','z','x','c','v' to play sounds. \n The top and bottom rows are distinct strings, played at slightly different frequencies. \n Mix them together and see how they interact.";
	if (isShowOnscreenInstructions) {
		ofDrawBitmapString(instructions, 100, 300);
	}

	string buf;
	buf = "listening for osc messages on port" + ofToString( PORT );
	ofDrawBitmapString( buf, 10, 20 );
	
	
//	ofDrawBitmapString( buf, 430, 20 );

//	for ( int i=0; i<NUM_MSG_STRINGS; i++ )
//	{
//		ofDrawBitmapString( msg_strings[i], 10, 40+15*i );
//	}
}

void testApp::keyPressed  (int key){

	switch (key) {
		case 'A':
		case 'a':
			generator.frequency = 110.0;
			generator.stringnum = 1;
			cout << "\nset generator.frequency:" << generator.frequency;
			cout << "set generator.stringnum:" << generator.stringnum;
			generator.play();
			break;
		case 'S':
		case 's':
			generator.frequency = 330.0;
			generator.stringnum = 3;
			cout << "\nset generator.frequency:" << generator.frequency;
			cout << "set generator.stringnum:" << generator.stringnum;
			generator.play();
			break;
		case 'D':
		case 'd':
			generator.frequency = 220.0;
			generator.stringnum = 2;
			cout << "\nset generator.frequency:" << generator.frequency;
			cout << "set generator.stringnum:" << generator.stringnum;
			generator.play();
			break;
		case 'F':
		case 'f':
			generator.frequency = 440.0;
			generator.stringnum = 4;
			cout	<< "\nset generator.frequency: " << generator.frequency
					<< ", generator.stringnum: " << generator.stringnum;
			generator.play();
			break;
		case 'Z':
		case 'z':
			generator.frequency = 120.0;
			generator.stringnum = 1;
			cout << "\nset generator.frequency:" << generator.frequency;
			cout << "set generator.stringnum:" << generator.stringnum;
			generator.play();
			break;
		case 'X':
		case 'x':
			generator.frequency = 340.0;
			generator.stringnum = 3;
			cout << "\nset generator.frequency:" << generator.frequency;
			cout << "set generator.stringnum:" << generator.stringnum;
			generator.play();
			break;
		case 'C':
		case 'c':
			generator.frequency = 230.0;
			generator.stringnum = 2;
			cout << "\nset generator.frequency:" << generator.frequency;
			cout << "set generator.stringnum:" << generator.stringnum;
			generator.play();
			break;
		case 'V':
		case 'v':
			generator.frequency = 450.0;
			generator.stringnum = 4;
			cout << "\nset generator.frequency:" << generator.frequency;
			cout << "set generator.stringnum:" << generator.stringnum;
			generator.play();
			break;
		case '?':
			isShowOnscreenInstructions = !isShowOnscreenInstructions;
			break;
        case 'Q':
		case 'q':
			OF_EXIT_APP(0);
			break;
		default:
			break;
	}
}

void testApp::plotData(){
	///////////////////////////////
	// plot some data! (hand speed/acc)
	///////////////////////////////
	
	/* things to plot
	double frequency;
    double pluckpos;
    double loss_;
    double velocity_;
    int stringnum;
    
    also volume (I have no idea how) 
	
	*/
	
	ofSetColor(0, 0, 255);
	linePluckPos.addVertex(lineCounter, ofGetHeight()-1000*generator.pluckpos);
	cout << "plot linePluckPos : " <<  500*generator.pluckpos << endl;
	linePluckPos.draw();
	ofDrawBitmapString("pluck position", 130, 300);
	
	ofSetColor(0, 255, 0);
	lineStringNum.addVertex(lineCounter, ofGetHeight()-10*generator.stringnum);
	cout << "plot lineStringNum : " <<  generator.stringnum << endl;
	lineStringNum.draw();
	ofDrawBitmapString("string number", 130, 315);
	
	ofSetColor(255, 0, 0);
	lineVelocity.addVertex(lineCounter, ofGetHeight()-generator.velocity);
	cout << "plot lineVelocity : " <<  generator.velocity << endl;
	lineVelocity.draw();
	ofDrawBitmapString("velocity", 130, 330);
	
	ofSetColor(0, 255, 255);
	lineLoss.addVertex(lineCounter, ofGetHeight()-10000*(1.0-generator.loss));
	cout << "plot lineLoss : " <<  10000*(1.0-generator.loss) << endl;
	lineLoss.draw();
	ofDrawBitmapString("loss", 130, 345);
	
	
	
	// draw coloured boxes to indicate which note is playing
	/*
	from the current stringNum, produce plot showing history of them.
	store history data in brightness levels of boxes.
	*/
	for (int i=0;i<12;++i) {
		ofSetColor(boxTints[i], 0, 0);
		ofCircle(45+90*(i%6)+90*(i/6), 100+50*(i/6), 25);
		ofDrawBitmapString(ofToString(i), 30*i, 50);
		if (i==generator.stringnum) 
			boxTints[i]=255;
		else 
			boxTints[i] *= 0.9;
	}

	lineCounter++;
	if (lineCounter>640) {
		lineCounter = 0;
		linePluckPos.clear();
		lineStringNum.clear();
		lineVelocity.clear();
		lineLoss.clear();
	}

	///////////////////////////////
}
