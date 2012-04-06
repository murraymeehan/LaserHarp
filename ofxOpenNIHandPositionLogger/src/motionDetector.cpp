#include "motionDetector.h"

#define USE_XML_NODE_CONFIG

//--------------------------------------------------------------
void motionDetector::setup() {

	// hand position tracking for Ryan
	myfile.open ("handPosition.txt");
//	img.allocate(640,480);
	
	countElapsedPlucks = 0;
	lastObjectTouched = -1;
	for (int i=0;i<12;i++) {
		timeObjectWasLastTouched[i] = 0;
	}
	
	// set background color
	ofBackground(0, 0, 0); 
	
	isRecording = false;
	isTracking		= false;
	isTrackingHands	= true;
	isFiltering		= false;
	isMasking		= true;
	nearThreshold = 200;
	farThreshold  = 1000;
	filterFactor = 0.1f;
	
	pluckParams.pos = 0;
    pluckParams.vel = 0;
    pluckParams.acc = 0;
    pluckParams.index = 0;

	if (filename=="") 
	{
		cout << "no .oni filename detected in cmd-line, running setupRecording() \n";
		isLive = true;
		setupRecording();
	} 
	else 
	{
		cout << ".oni filename detected in cmd-line, running setupPlayback(filename) \n";
		isLive = false;
		setupPlayback(filename);
	}
	
	setupDetectors();

}

void motionDetector::setupRecording(string _filename) {
	
#ifdef USE_XML_NODE_CONFIG
	//	recordContext.setupUsingXMLFile();
	recordContext.setupUsingXMLFile("/home/murray/499/ofxopenni_config.xml");
#else
	recordContext.setup();
#endif
	recordDepth.setup(&recordContext);

	recordUser.setup(&recordContext);
	recordUser.setSmoothing(filterFactor);	// built in openni skeleton smoothing...
	recordUser.setMaxNumberOfUsers(1);		// use this to set dynamic max number of users (NB: that a hard upper limit is defined by MAX_NUMBER_USERS in ofxUserGenerator)

	recordHandTracker.setup(&recordContext, 2);
	recordHandTracker.setSmoothing(filterFactor);		// built in openni hand track smoothing...
	recordHandTracker.setFilterFactors(filterFactor);	// custom smoothing/filtering (can also set per hand with setFilterFactor)...set them all to 0.1f to begin with

	recordContext.toggleRegisterViewport();
	recordContext.toggleMirror();

	oniRecorder.setup(&recordContext, ONI_STREAMING);
}

void motionDetector::setupPlayback(string _filename) {

	playContext.shutdown();
	playContext.setupUsingRecording(ofToDataPath(_filename));
	playDepth.setup(&playContext);
	
	playUser.setup(&playContext);
	playUser.setSmoothing(filterFactor);				// built in openni skeleton smoothing...

	playHandTracker.setup(&playContext, 4);
	playHandTracker.setSmoothing(filterFactor);			// built in openni hand track smoothing...
	playHandTracker.setFilterFactors(filterFactor);		// custom smoothing/filtering (can also set per hand with setFilterFactor)...set them all to 0.1f to begin with

	playContext.toggleRegisterViewport();
	playContext.toggleMirror();
}


//--------------------------------------------------------------
void motionDetector::update(){
	
	if (isLive) {

		// update all nodes
		recordContext.update();
		recordDepth.update();
		
//		// get a cropped depth image of hand for Ryan
//		depthImage.setFromPixels(recordDepth.getGrayPixels(), 640, 480);
		
		// demo getting depth pixels directly from depth gen
		depthRangeMask.setFromPixels(recordDepth.getDepthPixels(nearThreshold, farThreshold),
									 recordDepth.getWidth(), recordDepth.getHeight(), 
									 OF_IMAGE_GRAYSCALE);

		// update tracking/recording nodes
		if (isTracking) recordUser.update();
		if (isRecording) oniRecorder.update();

		// demo getting pixels from user gen
		if (isTracking && isMasking) {
			allUserMasks.setFromPixels(recordUser.getUserPixels(), recordUser.getWidth(), recordUser.getHeight(), OF_IMAGE_GRAYSCALE);
			user1Mask.setFromPixels(recordUser.getUserPixels(1), recordUser.getWidth(), recordUser.getHeight(), OF_IMAGE_GRAYSCALE);
			user2Mask.setFromPixels(recordUser.getUserPixels(2), recordUser.getWidth(), recordUser.getHeight(), OF_IMAGE_GRAYSCALE);
		}

	} else {

		// update all nodes
		playContext.update();
		playDepth.update();

//		// get a cropped depth image of hand for Ryan
//		depthImage.setFromPixels(playDepth.getGrayPixels(), 640, 480);
		
		// demo getting depth pixels directly from depth gen
		depthRangeMask.setFromPixels(playDepth.getDepthPixels(nearThreshold, farThreshold),
									 playDepth.getWidth(), playDepth.getHeight(), OF_IMAGE_GRAYSCALE);

		// update tracking/recording nodes
		if (isTracking) playUser.update();

		// demo getting pixels from user gen
		if (isTracking && isMasking) {
			allUserMasks.setFromPixels(playUser.getUserPixels(), playUser.getWidth(), playUser.getHeight(), OF_IMAGE_GRAYSCALE);
			user1Mask.setFromPixels(playUser.getUserPixels(1), playUser.getWidth(), playUser.getHeight(), OF_IMAGE_GRAYSCALE);
			user2Mask.setFromPixels(playUser.getUserPixels(2), playUser.getWidth(), playUser.getHeight(), OF_IMAGE_GRAYSCALE);
		}
	}
}


//--------------------------------------------------------------
void motionDetector::draw(){

	ofSetColor(255, 255, 255);
	glPushMatrix();
	//glScalef(0.75, 0.75, 0.75);
	if (isLive) 
	{
		recordDepth.draw(0,0,640,480);
		if (isTracking) 
			recordUser.draw();
		if (isTrackingHands) {
			recordHandTracker.drawHands();
//			depthImage.setFromPixels(depth.getGrayPixels(), 640, 480);
		}
	} else {
		playDepth.draw(0,0,640,480);
		if (isTracking) 
			playUser.draw();
		if (isTrackingHands) {
			playHandTracker.drawHands();
//			depthImage.setFromPixels(depth.getGrayPixels(), 640, 480);
		}
	}
	glPopMatrix();
	
	string statusPlay		= (string)(isLive ? "LIVE STREAM" : "PLAY STREAM");
	string statusRec		= (string)(!isRecording ? "READY" : "RECORDING");
	string statusSkeleton	= (string)(isTracking ? "TRACKING USERS: " + (string)(isLive ? ofToString(recordUser.getNumberOfTrackedUsers()) : ofToString(playUser.getNumberOfTrackedUsers())) + "" : "NOT TRACKING USERS");
	string statusSmoothSkel = (string)(isLive ? ofToString(recordUser.getSmoothing()) : ofToString(playUser.getSmoothing()));
	string statusHands		= (string)(isTrackingHands ? "TRACKING HANDS: " + (string)(isLive ? ofToString(recordHandTracker.getNumTrackedHands()) : ofToString(playHandTracker.getNumTrackedHands())) + ""  : "NOT TRACKING");
	string statusFilter		= (string)(isFiltering ? "FILTERING" : "NOT FILTERING");
	string statusFilterLvl	= ofToString(filterFactor);
	string statusSmoothHand = (string)(isLive ? ofToString(recordHandTracker.getSmoothing()) : ofToString(playHandTracker.getSmoothing()));

	stringstream msg;
	msg
		<< "    s : start/stop recording  : " << statusRec << endl
		<< "    p : playback/live streams : " << statusPlay << endl
		<< "    t : skeleton tracking     : " << statusSkeleton << endl
		<< "( / ) : smooth skely (openni) : " << statusSmoothSkel << endl
		<< "    h : hand tracking         : " << statusHands << endl
		<< "    f : filter hands (custom) : " << statusFilter << endl
		<< "[ / ] : filter hands factor   : " << statusFilterLvl << endl
		<< "; / ' : smooth hands (openni) : " << statusSmoothHand << endl
		<< "- / + : nearThreshold         : " << ofToString(nearThreshold) << endl
		<< "< / > : farThreshold          : " << ofToString(farThreshold) << endl
		<< "y / Y : tilt up / down        : " << endl
		<< endl
		<< "File  : " << oniRecorder.getCurrentFileName() << endl
		<< "FPS   : " << ofToString(ofGetFrameRate()) << "  " 
		<< endl;
	ofSetColor(255, 255, 0);
	ofDrawBitmapString(msg.str(), 20, 320);
}


//--------------------------------------------------------------
bool motionDetector::detect(){

	if (isLive) {
		if (recordHandTracker.getNumTrackedHands() > 0) 
		{
		    ofxTrackedHand hand = *recordHandTracker.getHand(0);
		    handPos = ofVec3f(hand.projectPos);
	    } else return false;
	} else {
		if (playHandTracker.getNumTrackedHands() > 0) 
		{
			ofxTrackedHand hand = *playHandTracker.getHand(0);
			handPos = ofVec3f(hand.projectPos);
		} else return false;
	}
	
	// Hand position variables for Ryan
	posX = handPos.x;
	posY = handPos.y;
	posZ = handPos.z;
    time = ofGetElapsedTimeMillis();
    
	cout << posX << ", " << posY << ", " << posZ << ", " << time << ";\n";
	myfile << posX << ", " << posY << ", " << posZ << ", " << time << ";\n";

	return true;

	
}

//--------------------------------------------------------------
string motionDetector::generateFileName() {

	string _root = "kinectRecord";
	string _timestamp = ofToString(ofGetDay()) +
	ofToString(ofGetMonth()) +
	ofToString(ofGetYear()) +
	ofToString(ofGetHours()) +
	ofToString(ofGetMinutes()) +
	ofToString(ofGetSeconds());

	string _filename = (_root + _timestamp + ".oni");
	
	return _filename;
}

//--------------------------------------------------------------
void motionDetector::keyPressed(int key){
	float smooth;
	switch (key) {
			
		case 's':
		case 'S':
			if (isRecording) {
				oniRecorder.stopRecord();
				isRecording = false;
				break;
			} else {
				oniRecorder.startRecord(generateFileName());
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
		case 't':
		case 'T':
			isTracking = !isTracking;
			break;
		case 'h':
		case 'H':
			isTrackingHands = !isTrackingHands;
			if(isLive) recordHandTracker.toggleTrackHands();
			if(!isLive) playHandTracker.toggleTrackHands();
			break;
		case 'f':
		case 'F':
			isFiltering = !isFiltering;
			recordHandTracker.isFiltering = isFiltering;
			playHandTracker.isFiltering = isFiltering;
			break;
		case '9':
		case '(':
			smooth = recordUser.getSmoothing();
			if (smooth - 0.1f > 0.0f) {
				recordUser.setSmoothing(smooth - 0.1f);
				playUser.setSmoothing(smooth - 0.1f);
			}
			break;
		case '0':
		case ')':
			smooth = recordUser.getSmoothing();
			if (smooth + 0.1f <= 1.0f) {
				recordUser.setSmoothing(smooth + 0.1f);
				playUser.setSmoothing(smooth + 0.1f);
			}
			break;
		case '[':
		case '{':
			if (filterFactor - 0.1f > 0.0f) {
				filterFactor = filterFactor - 0.1f;
				recordHandTracker.setFilterFactors(filterFactor);
				if (oniRecorder.getCurrentFileName() != "") playHandTracker.setFilterFactors(filterFactor);
			}
			break;
		case ']':
		case '}':
			if (filterFactor + 0.1f <= 1.0f) {
				filterFactor = filterFactor + 0.1f;
				recordHandTracker.setFilterFactors(filterFactor);
				if (oniRecorder.getCurrentFileName() != "") playHandTracker.setFilterFactors(filterFactor);
			}
			break;
		case ';':
		case ':':
			smooth = recordHandTracker.getSmoothing();
			if (smooth - 0.1f > 0.0f) {
				recordHandTracker.setSmoothing(smooth -  0.1f);
				playHandTracker.setSmoothing(smooth -  0.1f);
			}
			break;
		case '\'':
		case '\"':
			smooth = recordHandTracker.getSmoothing();
			if (smooth + 0.1f <= 1.0f) {
				recordHandTracker.setSmoothing(smooth +  0.1f);
				playHandTracker.setSmoothing(smooth +  0.1f);
			}
			break;
		case '>':
		case '.':
			farThreshold += 50;
			if (farThreshold > recordDepth.getMaxDepth()) farThreshold = recordDepth.getMaxDepth();
			break;
		case '<':
		case ',':
			farThreshold -= 50;
			if (farThreshold < 0) farThreshold = 0;
			break;
		case '+':
		case '=':
			nearThreshold += 50;
			if (nearThreshold > recordDepth.getMaxDepth()) nearThreshold = recordDepth.getMaxDepth();
			break;
		case '-':
		case '_':
			nearThreshold -= 50;
			if (nearThreshold < 0) nearThreshold = 0;
			break;
		case 'r':
			recordContext.toggleRegisterViewport();
			break;
        case 'Q':
        case 'q':
    		myfile.close();
	        break;
		default:
			break;
	}
}


void motionDetector:: drawMasks() {
	glPushMatrix();
	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE_MINUS_DST_COLOR, GL_ZERO);
	allUserMasks.draw(640, 0, 640, 480);
	glDisable(GL_BLEND);
    glPopMatrix();
	user1Mask.draw(320, 480, 320, 240);
	user2Mask.draw(640, 480, 320, 240);
	
}


void motionDetector::setupDetectors() {
	pluckParams.index 	= 0;
    pluckParams.pos 	= 0;
    pluckParams.vel = 0;
    pluckParams.acc = 0;
    
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



//--------------------------------------------------------------
bool motionDetector::isPointInsideBox(int stringNum, ofVec3f P, ofVec3f C1, ofVec3f C2, float CR){
    
    cout 	<<    "P=[" << (int)P.x  << "," << (int)P.y << "," << (int)P.z
			<< "] C1=[" << (int)C1.x << "," << (int)C1.y << "," << (int)C1.z
			<< "] C2=[" << (int)C2.x << "," << (int)C2.y << "," << (int)C2.z
			<< "] CR=[" << (int)CR   << "] " << endl;
	
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
	if (abs(P.x - C2.x) < abs(C2.x - C1.x)) {
//	if (abs(P.y - C1.y) < abs(C2.y - C1.y)) {
//	if (abs(P.y - C2.y) < abs(C2.y - C1.y)) {
//	if (abs(P.z - C1.z) < abs(C2.z - C1.z)) {
//	if (abs(P.z - C2.z) < abs(C2.z - C1.z)) {
	if (pluckParams.index != stringNum) {
		pluckParams.pos = abs((C1.z-P.z)/(C2.z - C1.z));
		pluckParams.vel = CR;
		pluckParams.acc = CR;
		pluckParams.index = stringNum;
		printf("\n %d \n",stringNum);
		return true;
    
	} } //	} } } } 
	 printf("X");
	 
	 }
	  
	pluckParams.pos = abs((C1.z-P.z)/(C2.z - C1.z));
	pluckParams.vel = CR;
	pluckParams.acc = CR;
	pluckParams.index = stringNum;
	
	return false;
	
}
