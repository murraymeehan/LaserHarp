#include "motionDetector.h"

//--------------------------------------------------------------
void motionDetector::setup() {
	
	ofSetBackgroundAuto(true);
	ofBackground(0, 0, 0);
	
	setupRecording();
	
	setupDetectors();
	
}

void motionDetector::setupDetectors() {
	pluckParams.r 	= 0;
    pluckParams.z 	= 0;
    pluckParams.stringNum = 0;
	/*
	Pentatonic Scale:
		A set of verticle strings in front of the kinect.
		x axis is left to right
		y axis is bottom to top
		z axis is back to front
		///////////////////////////////////////////////
		F#		G		G#		A		A#		B		Top
		*	C	*	C#	*	D	*	D#	*	E	*	E#
		F#		G		G#		A		A#		B		Bottom
		*	C	*	C#	*	D	*	D#	*	E	*	E#
			*		*		*		*		*		*
		///////////////////////////////////////////////
	*/
	
	// String dimensions of the giant pentatonic laserharp
		
	int stringWidth = 150; //mm
	// Assemble end points of strings
	ofVec3f points[48];
	for (int i=0; i<6; i++) {
		float x=20+i*100;
		points[i] 		= ofVec3f( x,		0.0,	0 );
		points[6+i] 	= ofVec3f( x+50.0,	0.0,	0 );
		points[12+i] 	= ofVec3f( x,		0.0,	0 );
		points[18+i] 	= ofVec3f( x+50.0,	0.0,	0 );
	
		points[24+i] 	= ofVec3f( x+50.0,	480.0,	10000 );
		points[24+6+i] 	= ofVec3f( x+100.0,	480.0,	10000 );
		points[24+12+i] = ofVec3f( x+50.0,	480.0,	10000 );
		points[24+18+i] = ofVec3f( x+100.0,	480.0,	10000 );
	}
	
//	for (int i=0; i<6; i++) {
//		float x=20+i*2*50;
//		points[i] 		= ofVec3f( x,		0.0,	0 );
//		points[6+i] 	= ofVec3f( x+50.0,	0.0,	0 );
//		points[12+i] 	= ofVec3f( x,		0.0,	0 );
//		points[18+i] 	= ofVec3f( x+50.0,	0.0,	0 );
//	
//		points[24+i] 	= ofVec3f( x,		480.0,	10000 );
//		points[24+6+i] 	= ofVec3f( x+50.0,	480.0,	10000 );
//		points[24+12+i] = ofVec3f( x,		480.0,	10000 );
//		points[24+18+i] = ofVec3f( x+50.0,	480.0,	10000 );
//	}
	// Assemble strings
	numStrings = 24;
	for (int i=0;i<numStrings;i++) {
		stringConfig[i].C1 = points[i];
		stringConfig[i].C2 = points[i+24];
		stringConfig[i].radius = stringWidth; 
	}	
}


void motionDetector::setupRecording() {
	
	numHands		= 4;
	isLive			= true;
	isFiltering		= false;
	isRecording		= false;
	filterFactor 	= 0.1f; // turn this off to get more discrete values
	
	recordContext.setup();	//	recordContext.setupUsingXMLFile();
	recordDepth.setup(&recordContext);
	recordHandTracker.setup(&recordContext, numHands);
	if (isFiltering) {
		recordHandTracker.setSmoothing(filterFactor);		
		recordHandTracker.setFilterFactors(filterFactor);	
	}
	
	recordContext.toggleMirror();
	oniRecorder.setup(&recordContext, ONI_STREAMING);
}


void motionDetector::setupPlayback(string _filename) {

	playContext.shutdown();
	playContext.setupUsingRecording(ofToDataPath(_filename));
	playDepth.setup(&playContext);

	playHandTracker.setup(&playContext, 4);
	if (isFiltering) {
		playHandTracker.setSmoothing(filterFactor);	
		playHandTracker.setFilterFactors(filterFactor);	
	}
	playContext.toggleMirror();
}


//--------------------------------------------------------------
void motionDetector::update(){
	if (isLive) {
		// update all nodes
		recordContext.update();
		recordDepth.update();
		// update recording nodes
		if (isRecording) oniRecorder.update();
	} else {
		// update all nodes
		playContext.update();
		playDepth.update();
	}
}


//--------------------------------------------------------------
void motionDetector::draw(){
	ofSetColor(255, 255, 255);
	glPushMatrix();
	
	if (isLive)
		recordDepth.draw(0,0,ofGetWidth(),ofGetHeight());
	else 
		playDepth.draw(0,0,ofGetWidth(),ofGetHeight());
	
	float a = ofGetWidth()/640;
	ofScale(a,a,a);
	if (isLive) 
		recordHandTracker.drawHands();
	else 
		playHandTracker.drawHands();
	ofScale(1/a,1/a,1/a);
	
	pluckDetection();
	
//	// draw a box in middle of each string, to help the play-tester find it
//	for (int i=0;i<numStrings;i++) {
//		if (i==pluckParams.stringNum) 
//			ofSetColor( 255.0, 0.0, 0.0 );
//		else 
//			ofSetColor( 0.0, i*255/numStrings, (numStrings-i)*255/numStrings );
//		ofBox(	0.5*(stringConfig[i].C1.x + stringConfig[i].C2.x)*ofGetWidth()/640, 
//				0.5*(stringConfig[i].C1.y + stringConfig[i].C2.y)*ofGetHeight()/480,
//				0,
//				50.0*ofGetWidth()/640);
//	}
//	
	glPopMatrix();
	ofSetColor(255, 0, 0);
	
	stringstream msg;
	msg
	<< "  R/r : start/stop recording  : " << (string)(!isRecording ? "READY" : "RECORDING") << endl
	<< "  P/p : playback/live streams : " << (string)(isLive ? "LIVE STREAM" : "PLAY STREAM") << endl
	<< "  [/] : enable/disable hand filtering : " << isFiltering << endl;
	if (isFiltering) {
		msg << "  ;/' : handFiltering param : " << filterFactor << endl
		<< "  ;/' : handSmoothing param : " << recordHandTracker.getSmoothing() << endl;
	};
	msg
	<< "  Q/q : quit program : " << endl
	<< endl
	<< "File  : " << oniRecorder.getCurrentFileName() << endl
	<< "FPS   : " << ofToString(ofGetFrameRate()) << "  " 
	<< "hand  : " << handPos.x << " , " << handPos.y << " , " << handPos.z << endl
	<< endl;
 	
	ofDrawBitmapString(msg.str(), ofGetWidth()/32, ofGetHeight()/1.5);

}


//--------------------------------------------------------------
bool motionDetector::isPointInsideBox(int stringNum, ofVec3f P, ofVec3f C1, ofVec3f C2, float CR){
    
//    cout 	<<    "P=[" << (int)P.x  << "," << (int)P.y << "," << (int)P.z
//			<< "] C1=[" << (int)C1.x << "," << (int)C1.y << "," << (int)C1.z
//			<< "] C2=[" << (int)C2.x << "," << (int)C2.y << "," << (int)C2.z
//			<< "] CR=[" << (int)CR   << "] " << endl;
	
	/*
	plain-english logic:
		if point is inside a string, and was not previously inside the same string, 
			update pluckParams and return true
		if point is inside a string, and was previously inside the same string, 
			updated pluckParams and return false.
		otherwise, update pluckParams and return false;
	concise logic:
		if point is inside a string and was not previously inside that string
			update params & return true
		else 
			update params and return false;
	*/ 
	
//	if (abs(P.x - C1.x) < abs(C2.x - C1.x)) cout << "f 1 ";
//	if (abs(P.x - C2.x) < abs(C2.x - C1.x)) cout << "f 2 ";
//	if (abs(P.y - C1.y) < abs(C2.y - C1.y)) cout << "f 3 ";
//	if (abs(P.y - C2.y) < abs(C2.y - C1.y)) cout << "f 4 ";
//	if (abs(P.z - C1.z) < abs(C2.z - C1.z)) cout << "f 5 ";
//	if (abs(P.z - C2.z) < abs(C2.z - C1.z)) cout << "f 6 ";
//	if (pluckParams.stringNum != stringNum) cout << "f 7 ";
	
    if (abs(P.x - C1.x) < abs(C2.x - C1.x)) {
//	if (abs(P.x - C2.x) < abs(C2.x - C1.x)) {
//	if (abs(P.y - C1.y) < abs(C2.y - C1.y)) {
//	if (abs(P.y - C2.y) < abs(C2.y - C1.y)) {
//	if (abs(P.z - C1.z) < abs(C2.z - C1.z)) {
//	if (abs(P.z - C2.z) < abs(C2.z - C1.z)) {
	if (pluckParams.stringNum != stringNum) {
		pluckParams.z = abs((C1.z-P.z)/(C2.z - C1.z));
		pluckParams.r = CR;
		pluckParams.stringNum = stringNum;
		printf("%d",stringNum);
		return true;
    
//	} } } } 
//	}
	 } 
	 printf("X");
	 
	 }
	  
	
	pluckParams.z = abs((C1.z-P.z)/(C2.z - C1.z));
	pluckParams.r = CR;
	pluckParams.stringNum = stringNum;
	pluckParams.stringNum = 9999;
	
	return false;
	
}

//--------------------------------------------------------------
bool motionDetector::isPointInsideCylinder(int stringNum, ofVec3f P, ofVec3f C1, ofVec3f C2, float CR){
    
    printf("P=[%f,%f,%f]\n C1=[%f,%f,%f]\n C2=[%f,%f,%f],\n CR=[%f]\n",P[0],P[1],P[2],C1[0],C1[1],C1[2],C2[0],C2[1],C2[2],CR);
    
    ofVec3f CN = ofVec3f(C2 - C1).getNormalized();
    float fDistanceToPlane1 = CN.dot(P-C1);
	float fDistanceToPlane2 = CN.dot(C2-P);
	if ((fDistanceToPlane1 > 0 ) && (fDistanceToPlane2 > 0)) {
		ofVec3f TempP =  P - (CN * fDistanceToPlane1) - C1;
		float fDistanceFromCenter = TempP.squareLength();
		if (fDistanceFromCenter < CR) {
			pluckParams.r = fDistanceFromCenter;
		    pluckParams.z = fDistanceToPlane1;
		    pluckParams.stringNum = 0;
		    printf("HIT!\n");
			return true;
		}
	}
	printf("MISS\n");
	return false;
}

//--------------------------------------------------------------
bool motionDetector::pluckDetection(){

    if (recordHandTracker.getNumTrackedHands() > 0) // if at least 1 hand is detected, report hand #1
    {
        ofxTrackedHand hand = *recordHandTracker.getHand(0);
        handPos = ofVec3f(hand.projectPos);
        
        // Method 3: check if is handPos inside any of the string-cylinders
    	// TODO: Design decision: Should the user be able to pluck more than one string at a time? If so, don't use for-loop here.
        for (int i=0;i<numStrings;i++) {
//			if (isPointInsideCylinder(i, handPos, stringConfig[i].C1, stringConfig[i].C2, stringConfig[i].radius)) 
			if (isPointInsideBox(i, handPos, stringConfig[i].C1, stringConfig[i].C2, stringConfig[i].radius)) 
				return true;
        }
    }
	return false;
}

//--------------------------------------------------------------
void motionDetector::keyPressed(int key){

	float smooth;

	switch (key) {
		case 'f':
			isFiltering = !isFiltering;
			break;
		case 'r':
		case 'R':
			if (isRecording) {
				oniRecorder.stopRecord();
				isRecording = false;
				break;
			} else {
				string _root = "kinectRecord";
				string _timestamp = 
					ofToString(ofGetDay()) +
					ofToString(ofGetMonth()) +
					ofToString(ofGetYear()) +
					ofToString(ofGetHours()) +
					ofToString(ofGetMinutes()) + 
					ofToString(ofGetSeconds());
				string _filename = (_root + _timestamp + ".oni");
				oniRecorder.startRecord(_filename);
				isRecording = true;
				break;
			}
			break;
		case 'p':
		case 'P':
			if (oniRecorder.getCurrentFileName() != "" && !isRecording && isLive) {
				setupPlayback(oniRecorder.getCurrentFileName());
				isLive = false;
			} else {
				isLive = true;
			}
			break;
		case '[':
		case '{':
			if (isFiltering) {
				if (filterFactor - 0.1f > 0.0f) {
					filterFactor = filterFactor - 0.1f;
					recordHandTracker.setFilterFactors(filterFactor);
					if (oniRecorder.getCurrentFileName() != "") playHandTracker.setFilterFactors(filterFactor);
				}
			}
			break;
		case ']':
		case '}':
			if (isFiltering) {
				if (filterFactor + 0.1f <= 1.0f) {
					filterFactor = filterFactor + 0.1f;
					recordHandTracker.setFilterFactors(filterFactor);
					if (oniRecorder.getCurrentFileName() != "") playHandTracker.setFilterFactors(filterFactor);
				}
			}
			break;
		case ';':
		case ':':
			if (isFiltering) {
				smooth = recordHandTracker.getSmoothing();
				if (smooth - 0.1f > 0.0f) {
					recordHandTracker.setSmoothing(smooth -  0.1f);
					playHandTracker.setSmoothing(smooth -  0.1f);
				}
			}
			break;
		case '\'':
		case '\"':
			if (isFiltering) {
				smooth = recordHandTracker.getSmoothing();
				if (smooth + 0.1f <= 1.0f) {
					recordHandTracker.setSmoothing(smooth +  0.1f);
					playHandTracker.setSmoothing(smooth +  0.1f);
				}
			}
			break;
		case 'Q':
        case 'q':
        	OF_EXIT_APP(0);
	        break;
		default:
			break;
	}
}
