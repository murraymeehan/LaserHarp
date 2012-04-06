#include "testApp.h"

void testApp::setup()
{
	generator.setup();
	
	receiver.setup( PORT );
	cout << "listening for osc messages on port " << PORT << "\n";
	current_msg_string = 0;
	
	ofSetWindowTitle("Audio Generator (LaserHarp)");
	ofBackground(0, 0, 0);
	
	displayInstructions = false;

	// visualizations
	boxTints = new int[12];
	for (int i=0;i<12;++i)
		boxTints[i] = 0; 
	
}

void testApp::update()
{
	checkMessage();
	generator.update();
}

void testApp::draw()
{

	//	// fun fact: maximum line width is graphics card dependant (http://forum.openframeworks.cc/index.php?topic=3751.0)
	//	ofSetLineWidth(3); 

	int top = 0.7*ofGetHeight();
	int bottom = 0.1*ofGetHeight();
	int left = 0.2*ofGetWidth();
	int right = 0.6*ofGetWidth();
	
	ofSetColor(0, 0, 255);
	ofDrawBitmapString("pluck position", 10, 35);
	linePluckPos.addVertex(left+lineCounter, bottom + top*generator.playEvent.pluckpos);
	linePluckPos.draw();
	
	ofSetColor(0, 255, 0);
	ofDrawBitmapString("string number", 10, 50);
	lineStringNum.addVertex(left+lineCounter, bottom + top*generator.playEvent.stringnum/12.0);
	lineStringNum.draw();
	
	ofSetColor(255, 0, 0);
	ofDrawBitmapString("velocity", 10, 65);
	lineVelocity.addVertex(left+lineCounter, bottom + top*generator.playEvent.velocity);
	lineVelocity.draw();
	
	ofSetColor(0, 255, 255);
	ofDrawBitmapString("loss", 10, 80);
	lineLoss.addVertex(left+lineCounter, bottom + top*generator.playEvent.loss);
	lineLoss.draw();
	
	for (int i=0;i<12;++i) {
		ofSetColor(boxTints[i], 0, 15*i);
		//ofCircle(45+90*(i%6)+90*(i/6), 100+50*(i/6), 25); // 2 offset rows
		ofCircle(ofGetWidth()*(0.1 +0.8*(i/12.0)), 0.9*ofGetHeight(), ofGetHeight()*0.07);
		ofSetColor(0, 0, 15*i);
		ofDrawBitmapString(ofToString(i), ofGetWidth()*(0.1 +0.8*(i/12.0)), 0.9*ofGetHeight());							
		boxTints[i] *= 0.9;
	}
	
	lineCounter++;
	if (lineCounter>right) {
		lineCounter = 0;
		linePluckPos.clear();
		lineStringNum.clear();
		lineVelocity.clear();
		lineLoss.clear();
	}
	
	ofSetColor(255, 255, 255);
	stringstream instructions;
	instructions<< "Press '1' to '0', '-', '=' to play sounds.\n"
				<< "The top and bottom rows are distinct strings,\n"
				<< "played at slightly different frequencies.\n"
				<< "Mix them together and see how they interact.\n"
				<< "  f :: toggle fullscreen mode\n"
				<< "  ? :: toggles display of on screen guide";
	
	if (displayInstructions) 
		ofDrawBitmapString(instructions.str(), 0, 110);

	string buf;
	buf = "listening for osc messages on port" + ofToString( PORT );
	ofDrawBitmapString( buf, 10, 20 );
	
}

void testApp::keyPressed  (int key)
{ 
	//play(int stringnum 0-11, 
	//		double pluckpos 0.0-1.0, 
	//		double velocity 0.0-1.0 (1.0 is a nice val), 
	//		double loss 0.995-1.0)
	switch (key) {
		case '1':		case '!':
			boxTints[0]=255;
			generator.play(0, 0.5, 0.9, 1.0);
			break;
		case '2':		case '@':
			boxTints[1]=255;
			generator.play(1, 0.8, 0.95, 0.996);
			break;
		case '3':		case '#':
			boxTints[2]=255;
			generator.play(2, 0.7, 1.0, 0.997);
			break;
		case '4':		case '$':
			boxTints[3]=255;
			generator.play(3, 0.6, 0.1, 0.998);
			break;
		case '5':		case '%':
			boxTints[4]=255;
			generator.play(4, 0.5, 1.0, 0.998);
			break;
		case '6':		case '^':
			boxTints[5]=255;
			generator.play(5, 0.4, 1.0, 0.998);
			break;
		case '7':		case '&':
			boxTints[6]=255;
			generator.play(6, 0.3, 1.0, 0.998);
			break;
		case '8':		case '*':
			boxTints[7]=255;
			generator.play(7, 0.2, 1.0, 0.998);
			break;
		case '9':		case '(':
			boxTints[8]=255;
			generator.play(8, 0.1, 1.0, 0.998);
			break;
		case '0':		case ')':
			boxTints[9]=255;
			generator.play(9, 0.15, 1.0, 0.998);
			break;
		case '-':		case '_':
			boxTints[10]=255;
			generator.play(10, 0.25, 1.0, 0.998);
			break;
		case '=':		case '+':
			boxTints[11]=255;
			generator.play(11, 0.35, 1.0, 0.998);
			break;
		case '?':
			displayInstructions = !displayInstructions;
			break;
        case 'f':		case 'F':
			ofToggleFullscreen();
			break;
		case 'q':		case 'Q':
			OF_EXIT_APP(0);
			break;
		default:
			break;
	}
	
}


void testApp::checkMessage()
{

	
//	// hide old messages (optional step?)
//	for ( int i=0; i<NUM_MSG_STRINGS; i++ )
//	{
//		if ( timers[i] < ofGetElapsedTimef() )
//			msg_strings[i] = "";
//	}

	/////////////////////////////
	// check for waiting messages
	while( receiver.hasWaitingMessages() )
	{
		// get the next message
		ofxOscMessage m;
		receiver.getNextMessage( &m );

		// check for mouse moved message
		if ( m.getAddress() == "/laserharp/playEvent" )
		{
			// receive playEventParameters sent bymotionDetector
			playEventParameters playEvent;
			playEvent.stringnum = m.getArgAsInt32( 0 );
			playEvent.pluckpos = m.getArgAsFloat( 1 );
			playEvent.velocity = m.getArgAsFloat( 2 );
			playEvent.loss = m.getArgAsFloat( 3 );
		
			// play the requested sound
			boxTints[playEvent.stringnum]=255;
			generator.play(playEvent);
			
		}
		else
		{
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
		
	//////////////////////////////
	//////////////////////////////
	//////////////////////////////
	
	/* // Convert from touchEventParameters to playEventParameters
	
	So, I decided that touchEvent->playEvent conversion 
		should be done on the motionDetector side, not the 
		audioGenerator, because that way less data needs 
		to be transmitted. 
		
		If data transmission is fast and processing power is
		limited on the device running the motionDetector, then 
		this is a bad idea. I am running the motionDetector on 
		a big powerful PC, so it's a good idea for my usage. 
		As a general rule, I imagine that any device with the resources
		to run the motionDetector can also handle the conversion.
		
	this section  will go in motionDetector's source folder
	
	typedef struct 
	{
		int stringnum;	// 0 <= stringnum < numObjects	//was stringnum; 
		float pluckpos;// 0 <= pluckpos <= 1			//was z
		float velocity;	// 0 <= velocity <= 1			//was velocity;
		float loss;		// 0 <= loss <= 1				//was loss;
	} playEventParameters;

	typedef struct 
	{
		int stringnum;			//was index_;
		ofVec3f position; 		//was pos_; 
				// this is a position vector relative to 
				// the object's centroid (which is the 
				// object's center for simple object shapes)
		ofVec3f velocity;		//was vel_;
		ofVec3f acceleration;	//was acc_;
	} touchEventParameters;
	
	*/

}

