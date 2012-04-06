#include "motionDetector.h"

#define USE_XML_NODE_CONFIG

// Choose one:
//#define LASERHARP_PENTATONIC_SCALE
//#define LASERHARP_CHROMATIC_SCALE
//#define PIANO_SCALE
//#define CHOP_BLOCK_SCALE

//--------------------------------------------------------------
void motionDetector::setup() {

	
	// Definition of Scale	
//	numStrings = 12;
//	touchObjects = new touchObjectParam[numStrings];
	
	
	// status of keyboard controlled stuff
	isShowOnscreenInstructions = false;
	maxVel = maxAcc = 0.0;
	maxTime = 0.0;
	timeStep = 0.0;
	countElapsedPlucks = 0;
	lastObjectTouched = -1;
	for (int i=0;i<12;i++) {
		timeObjectWasLastTouched[i] = 0;
	}
	
	// onscreen data plotting
	lineCounter=0;
	
	// Kinect/OpenNI stuff
	isRecording = false;
	isTracking		= false;
	isTrackingHands	= true;
	isFiltering		= false;
	isMasking		= true;
	nearThreshold = 200;
	farThreshold  = 1000;
//	filterFactor = 0.1f;
	
	if (ONIfilename=="") 
	{
		cout << "no .oni filename detected in cmd-line, running setupRecording() \n";
		isLive = true;
		setupRecording();
	} 
	else 
	{
		cout << ".oni filename detected in cmd-line, running setupPlayback(filename) \n";
		isLive = false;
		setupPlayback(ONIfilename);
	}
	// end of Kinect/OpenNI stuff
	
	
	ofBackground(0, 0, 0); 
	setupDetectors();
	
	
}

void motionDetector::setupRecording(string _filename) {
	
	recordContext.setupUsingXMLFile("/home/murray/499/ofxopenni_config.xml");
	//recordContext.setupUsingXMLFile();
	//recordContext.setup();
	
	recordDepth.setup(&recordContext);
//	recordImage.setup(&recordContext);
	
	recordUser.setup(&recordContext);
//	recordUser.setSmoothing(filterFactor);	// built in openni skeleton smoothing...
	recordUser.setMaxNumberOfUsers(2);		// use this to set dynamic max number of users (NB: that a hard upper limit is defined by MAX_NUMBER_USERS in ofxUserGenerator)
	
	recordHandTracker.setup(&recordContext, 4); // last arg is # of hands
//	recordHandTracker.setSmoothing(filterFactor);		// built in openni hand track smoothing...
//	recordHandTracker.setFilterFactors(filterFactor);	// custom smoothing/filtering (can also set per hand with setFilterFactor)...set them all to 0.1f to begin with
	
	recordContext.toggleRegisterViewport();
	recordContext.toggleMirror();

	oniRecorder.setup(&recordContext, ONI_STREAMING);
}

void motionDetector::setupPlayback(string _filename) {

	playContext.shutdown();
	playContext.setupUsingRecording(ofToDataPath(_filename));
	playDepth.setup(&playContext);
//	playImage.setup(&recordContext);
	playUser.setup(&playContext);
//	playUser.setSmoothing(filterFactor);				// built in openni skeleton smoothing...
	playUser.setMaxNumberOfUsers(2);		// use this to set dynamic max number of users (NB: that a hard upper limit is defined by MAX_NUMBER_USERS in ofxUserGenerator)
	playHandTracker.setup(&playContext, 4);
//	playHandTracker.setSmoothing(filterFactor);			// built in openni hand track smoothing...
//	playHandTracker.setFilterFactors(filterFactor);		// custom smoothing/filtering (can also set per hand with setFilterFactor)...set them all to 0.1f to begin with
	playContext.toggleRegisterViewport();
	playContext.toggleMirror();
}


//--------------------------------------------------------------
void motionDetector::update(){
	
	if (isLive) {

		// update all nodes
		recordContext.update();
		recordDepth.update();
//		recordImage.update();

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
//		playImage.update();

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
	/*
	The depth video from the kinect sensor is 
	@ 640x480 resolution, and all draw operations 
	are done with that in mind. (rX,rY are used as
	scaling factors)
	
	pixel range:
		((0,0)	(0,640)
		(480,0) (480,640) )
	*/
	float rX = 640.0;
	float rY = 480.0;
	
	ofSetColor(255, 255, 255);
	glPushMatrix();
	
//	float scaleFactor = min(ofGetHeight()/rX , ofGetWidth()/rY);
//	glScalef(scaleFactor,scaleFactor,scaleFactor);
	
	if (isLive) {
		recordDepth.draw(0,0,rX,rY);
//		recordImage.draw(rX, 0, rX, rY);
		if (isTracking) recordUser.draw();
		if (isTrackingHands) recordHandTracker.drawHands();
	} else {
		playDepth.draw(0,0,rX,rY);
//		playImage.draw(rX, 0, rX, rY);
		if (isTracking) playUser.draw();
		if (isTrackingHands) playHandTracker.drawHands();
	}
	
//	glScalef(1/scaleFactor,1/scaleFactor,1/scaleFactor);
	glPopMatrix();
	
	string statusPlay		= (string)(isLive ? "LIVE STREAM" : "PLAY STREAM");
	string statusRec		= (string)(!isRecording ? "READY" : "RECORDING");
	string statusSkeleton	= (string)(isTracking ? "TRACKING USERS: " + (string)(isLive ? ofToString(recordUser.getNumberOfTrackedUsers()) : ofToString(playUser.getNumberOfTrackedUsers())) + "" : "NOT TRACKING USERS");
	string statusSmoothSkel = (string)(isLive ? ofToString(recordUser.getSmoothing()) : ofToString(playUser.getSmoothing()));
	string statusHands		= (string)(isTrackingHands ? "TRACKING HANDS: " + (string)(isLive ? ofToString(recordHandTracker.getNumTrackedHands()) : ofToString(playHandTracker.getNumTrackedHands())) + ""  : "NOT TRACKING");
	string statusFilter		= (string)(isFiltering ? "FILTERING" : "NOT FILTERING");
//	string statusFilterLvl	= ofToString(filterFactor);
	string statusSmoothHand = (string)(isLive ? ofToString(recordHandTracker.getSmoothing()) : ofToString(playHandTracker.getSmoothing()));
	string statusShowInstructions = (string)(isShowOnscreenInstructions ? "SHOWING INSTRUCTIONS" : "NOT SHOWING INSTRUCTIONS");

	if (isShowOnscreenInstructions) {
		stringstream msg;
		msg
			<< "    s : start/stop recording  : " << statusRec << endl
			<< "    p : playback/live streams : " << statusPlay << endl
			<< "    t : skeleton tracking     : " << statusSkeleton << endl
			<< "( / ) : smooth skely (openni) : " << statusSmoothSkel << endl
			<< "    h : hand tracking         : " << statusHands << endl
	//		<< "    f : filter hands (custom) : " << statusFilter << endl
			<< "  f   : toggle fullscreen	  : " << endl
//			<< "[ / ] : filter hands factor   : " << statusFilterLvl << endl
//			<< "; / ' : smooth hands (openni) : " << statusSmoothHand << endl
			<< "- / + : nearThreshold         : " << ofToString(nearThreshold) << endl
			<< "< / > : farThreshold          : " << ofToString(farThreshold) << endl
			<< "  ?   : toggle show keys      : " << statusShowInstructions << endl
			<< endl
			<< "File  : " << oniRecorder.getCurrentFileName() << endl
			<< "FPS   : " << ofToString(ofGetFrameRate()) << "  " 
			<< endl;
		ofSetColor(255, 255, 0);
		ofDrawBitmapString(msg.str(), 20, 320);
	}
	
	// Labelling axis
	ofSetColor(255, 255, 255);
	ofDrawBitmapString("Val",20,773);
	ofDrawBitmapString("0", 80, 900);
	ofDrawBitmapString("255", 80, 745);
	
	int top = 1.0*ofGetHeight();
	int bottom = 0.0*ofGetHeight();
	int left = 0.0*ofGetWidth();
	int right = 1.0*ofGetWidth();
	
	ofSetColor(0, 255, 0);
	ofDrawBitmapString("velocity (current, max)", 10, 35);
	lineV.addVertex(left+lineCounter, bottom + top*playEvent.velocity/40.0);
	lineV.draw();
	
	ofSetColor(255, 0, 0);
	ofDrawBitmapString("acceleration (current, max)", 10, 50);
	lineA.addVertex(left+lineCounter, bottom + top*playEvent.acceleration/20.0);
	lineA.draw();
	
	ofSetColor(0, 0, 255);
	ofDrawBitmapString("jitter (time delay between reported hand positions)", 10, 65);
	lineTimeStepSize.addVertex(left+lineCounter, bottom + top*timeStep);
	lineTimeStepSize.draw();
	
	lineCounter+=3;
	if (lineCounter>right) {
		lineCounter = 0;
		lineA.clear();
		lineV.clear();
		lineAMax.clear();
		lineVMax.clear();
		lineTimeStepSize.clear();
	}
	
}


//--------------------------------------------------------------
string motionDetector::generateFileName() {

	string _root = "kinectRecord";
	string _timestamp = ofGetTimestampString();
	string _filename = (_root + ofGetTimestampString() + ".oni");
	
	return _filename;
}

//--------------------------------------------------------------
void motionDetector::keyPressed(int key)
{
	switch (key) {
		case 's':		case 'S':
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
		case 'p':		case 'P':
			if (oniRecorder.getCurrentFileName() != "" && !isRecording && isLive) {
				setupPlayback(oniRecorder.getCurrentFileName());
				isLive = false;
			} else {
				isLive = true;
			}
			break;
		case 't':		case 'T':
			isTracking = !isTracking;
			break;
		case 'h':		case 'H':
			isTrackingHands = !isTrackingHands;
			if(isLive) recordHandTracker.toggleTrackHands();
			if(!isLive) playHandTracker.toggleTrackHands();
			break;
		case 'f':		case 'F':
//			isFiltering = !isFiltering;
//			recordHandTracker.isFiltering = isFiltering;
//			playHandTracker.isFiltering = isFiltering;
			ofToggleFullscreen();
			break;
// 		case '9':	case '(':
// 			float smooth = recordUser.getSmoothing();
// 			if (smooth - 0.1f > 0.0f) {
// 				recordUser.setSmoothing(smooth - 0.1f);
// 				playUser.setSmoothing(smooth - 0.1f);
// 			}
// 			break;
// 		case '0':	case ')':
// 			float smooth = recordUser.getSmoothing();
// 			if (smooth + 0.1f <= 1.0f) {
// 				recordUser.setSmoothing(smooth + 0.1f);
// 				playUser.setSmoothing(smooth + 0.1f);
// 			}
// 			break;
// 		case '[':	case '{':
// 			if (filterFactor - 0.1f > 0.0f) {
// 				filterFactor = filterFactor - 0.1f;
// 				recordHandTracker.setFilterFactors(filterFactor);
// 				if (oniRecorder.getCurrentFileName() != "") playHandTracker.setFilterFactors(filterFactor);
// 			}
// 			break;
// 		case ']':	case '}':
// 			if (filterFactor + 0.1f <= 1.0f) {
// 				filterFactor = filterFactor + 0.1f;
// 				recordHandTracker.setFilterFactors(filterFactor);
// 				if (oniRecorder.getCurrentFileName() != "") playHandTracker.setFilterFactors(filterFactor);
// 			}
// 			break;
// 		case ';':	case ':':
// 			float smooth = recordHandTracker.getSmoothing();
// 			if (smooth - 0.1f > 0.0f) {
// 				recordHandTracker.setSmoothing(smooth -  0.1f);
// 				playHandTracker.setSmoothing(smooth -  0.1f);
// 			}
// 			break;
// 		case '\'':	case '\"':
// 			float smooth = recordHandTracker.getSmoothing();
// 			if (smooth + 0.1f <= 1.0f) {
// 				recordHandTracker.setSmoothing(smooth +  0.1f);
// 				playHandTracker.setSmoothing(smooth +  0.1f);
// 			}
// 			break;
		case '>':	case '.':
			farThreshold += 50;
			if (farThreshold > recordDepth.getMaxDepth()) farThreshold = recordDepth.getMaxDepth();
			break;
		case '<':	case ',':
			farThreshold -= 50;
			if (farThreshold < 0) farThreshold = 0;
			break;
		case '+':	case '=':
			nearThreshold += 50;
			if (nearThreshold > recordDepth.getMaxDepth()) nearThreshold = recordDepth.getMaxDepth();
			break;
		case '-':	case '_':
			nearThreshold -= 50;
			if (nearThreshold < 0) nearThreshold = 0;
			break;
		case 'r':
			recordContext.toggleRegisterViewport();
			break;
        case '?':
			isShowOnscreenInstructions = !isShowOnscreenInstructions;
			break;
        default:
			break;
	}
}


void motionDetector:: drawMasks() {

	// TODO: ooh, idea! make the masks be the physical locations of the strings, and display them over top of the video feed.
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
	
	//initialize values of touchEvent
	touchEvent.pluckpos = 0.0;
    touchEvent.velocity = 0.0;
    touchEvent.acceleration = 0.0;
    touchEvent.stringNum = 0;

	/* Note on Physical Orientation of Scales:
		x axis is left to right
		y axis is bottom to top
		z axis is back to front (as a user facing the kinect)
	*/
	

#ifdef LASERHARP_PENTATONIC_SCALE
	/*
	Pentatonic Scale:
		A set of verticle strings in front of the kinect.
		Position of notes on the Laser Board (the physical surface I play near)
			F#		G		G#		A		A#		B		Top
			*	C	*	C#	*	D	*	D#	*	E	*	E#
			F#		G		G#		A		A#		B		Bottom
			*	C	*	C#	*	D	*	D#	*	E	*	E#
				*		*		*		*		*		*
	Physical orientation note:
		x axis is left to right
		y axis is bottom to top
		z axis is back to front
		
	*/
	
	// Assemble touchObjects (aka 'strings')
	int stringWidth = 150; //mm
	ofVec3f points[48];
	for (int i=0; i<6; i++) {
		float x=20+i*100;
		// tops
		points[i] 		= ofVec3f( x,		0.0,	0 );
		points[6+i] 	= ofVec3f( x+50.0,	0.0,	0 );
		points[12+i] 	= ofVec3f( x,		0.0,	0 );
		points[18+i] 	= ofVec3f( x+50.0,	0.0,	0 );
		// bottoms
		points[24+i] 	= ofVec3f( x+50.0,	480.0,	10000 );
		points[24+6+i] 	= ofVec3f( x+100.0,	480.0,	10000 );
		points[24+12+i] = ofVec3f( x+50.0,	480.0,	10000 );
		points[24+18+i] = ofVec3f( x+100.0,	480.0,	10000 );
	}
	numStrings = 12;
	for (int i=0;i<numStrings;i++) {
		touchObjects[i].C1 = points[i];
		touchObjects[i].C2 = points[i+24];
		touchObjects[i].radius = stringWidth; 
	}	
#endif

#ifdef LASERHARP_CHROMATIC_SCALE

#endif
	
#ifdef CHOP_BLOCK_SCALE

#endif
	
#ifdef PIANO_SCALE

#endif
	
#ifdef

#endif
	
}

//--------------------------------------------------------------
bool motionDetector::detect(){
	
	int numHands;
	if (isLive) {
		numHands = recordHandTracker.getNumTrackedHands();
	} else {
		numHands = playHandTracker.getNumTrackedHands();
	}
	
	for (int handIndex = 0; handIndex < numHands; handIndex++) {
		if (isLive) {
			ofxTrackedHand hand = *recordHandTracker.getHand(handIndex);
			handPos = ofVec3f(hand.projectPos);
		} else {
			ofxTrackedHand hand = *playHandTracker.getHand(handIndex);
			handPos = ofVec3f(hand.projectPos);
		}
		
		if (oldPos != handPos) {
		oldVel = newVel;
		newVel = (handPos-oldPos);//ofGetLastFrameTime();
		oldPos = handPos;
		}
		touchEvent.velocity = newVel;
		
		if (oldVel != newVel) {
			oldAcc = newAcc;
			newAcc = (newVel-oldVel);///ofGetLastFrameTime();
			oldVel = newVel;
		}
		touchEvent.acceleration = newAcc;
		
		playEvent = getPlayEvent(touchEvent);
		
//		////////////////
	
		for (int i=0;i<numStrings;i++) {
			if (i != lastObjectTouched) {
				if (isPointInsideBox(i, handPos, touchObjects[i]))  {
					lastObjectTouched = i;
					double timeSinceObjectTouched = ofGetElapsedTimeMillis() - timeObjectWasLastTouched[i];
					if (timeSinceObjectTouched > 200) {
						timeObjectWasLastTouched[i] = ofGetElapsedTimeMillis();
						countElapsedPlucks++;
//						cout << "countElapsedPlucks: " << countElapsedPlucks << endl;
						return true;
					}
				}
			
			}
		}

	}
	
	return false;
	
}

//--------------------------------------------------------------
bool motionDetector::isPointInsideCylinder(int stringNum, ofVec3f P, touchObjectParam touchObject) {
	/*	This function is disabled until such time as I decide to go back to using cylindrical string-like touchObjects instead of boxes	*/
	if (touchEvent.stringNum != stringNum) 
	{
		ofVec3f lineA=(touchObject.C1-P);
		ofVec3f lineB=(touchObject.C1-touchObject.C2);
		double stringPos = lineA.dot(lineB)/lineB.squareLength();
		if ( (0 <= stringPos) && (1 <= stringPos) )
		{
			double stringRadius = lineA.getCrossed(lineB).squareLength() / lineB.squareLength(); // Remember: length is one of the slowest operations ofVec3f supports.
			if (stringRadius < (touchObject.radius*touchObject.radius)) 
			{
				touchEvent.pluckpos = stringPos;
				touchEvent.stringNum = stringNum;
				printf("Touched %d \n",stringNum);
				return true;
			}
		}
	}
	
	return false;
}

//--------------------------------------------------------------
bool motionDetector::isPointInsideBox(int stringNum, ofVec3f P, touchObjectParam touchObject) {
	/* 
	concept:
		if point is inside touchObject and it was not inside the string last time,
			update stringNum+pluckpos and return true
		if point is inside a string, and was previously inside the same string, 
			updated touchEvent and return false.
		otherwise, update touchEvent and return false;
	
	shorter but equivalent version of above, used in code below:
		update params
		if point is inside box and was not inside the box last time we checked, 
			return true
		otherwise, 
			return false;
	*/ 
	ofVec3f C1 = touchObject.C1;
	ofVec3f C2 = touchObject.C2;
	ofVec3f CR = touchObject.radius;
	
	////////////////
	// measurement of touchEvent.pluckPos, which belongs in detect()
	////////////////
//	ofVec3f lineA(C1-P);		//ofVec3f lineA = (ofVec3f) (C1-P);
//	ofVec3f lineB(C1-C2);	//ofVec3f lineB = (ofVec3f) (C1-C2);
//	// not sure which way is better
//	touchEvent.pluckpos = lineA.getNormalized().dot(lineB.getNormalized());
	//touchEvent.pluckpos = lineA.normalize().dot(lineB.normalize());
	////////////////

	if (touchEvent.stringNum != stringNum) 
	{
		if ( (abs(P.x - C1.x) < abs(C2.x - C1.x)) && (abs(P.x - C2.x) < abs(C2.x - C1.x)) ) 
		{
			if ( (abs(P.y - C1.y) < abs(C2.y - C1.y)) && (abs(P.y - C2.y) < abs(C2.y - C1.y)) ) 
			{
				if ( (abs(P.z - C1.z) < abs(C2.z - C1.z)) && (abs(P.z - C2.z) < abs(C2.z - C1.z)) ) 
				{
					ofVec3f lineA(touchObject.C1-P);
					ofVec3f lineB(touchObject.C1-touchObject.C2);
					
					// not sure which of these is right:
					touchEvent.pluckpos = lineA.dot(lineB)/lineB.squareLength();
					//touchEvent.pluckpos = lineA.getNormalized().dot(lineB.getNormalized());
					//touchEvent.pluckpos = lineA.normalize().dot(lineB.normalize());
					//touchEvent.pluckpos = lineA.dot(lineB)/lineB.length();
					
					touchEvent.stringNum = stringNum;
					printf("\n %d \n",stringNum);
					return true;
				} 
			}
		}
	}
	
	return false;
	
}

//--------------------------------------------------------------
playEventParameters motionDetector::getPlayEvent(touchEventParameters touchEvent) 
{
	
	float a=0.5;
	
	/*
	This following formula amplifiedthe noise. Either poor choice of corner frequency or Lowpass inappropriate.
	TODO: Matlab! I mean octave!
	
	a=0.5;
	playEvent.velocity = a*touchEvent.velocity.length() + (1-a)*playEvent.velocity;
	a=0.5;
	playEvent.acceleration = a*touchEvent.acceleration.length() + (1-a)*playEvent.acceleration; 
	*/
	
	playEventParameters playEvent;
	playEvent.stringNum = touchEvent.stringNum;
	playEvent.pluckpos = touchEvent.pluckpos;
	
	playEvent.velocity = touchEvent.velocity.length();
	if (playEvent.velocity > maxVel) 
		maxVel = playEvent.velocity;
	
	playEvent.acceleration = touchEvent.acceleration.length(); 
	if (playEvent.acceleration > maxAcc) 
		maxAcc = playEvent.acceleration;

	
	a=0.5;
	timeStep = a*ofGetLastFrameTime() + (1-a)*timeStep ;
	
//	float a=0.5;
//	timeStep = a*ofGetElapsedTimef() + (1-a)*timeStep ;

	//	timeStep = abs(ofGetElapsedTimef() - timeStep) * ;
	//	if (timeStep > maxTime) 
	//		maxTime = timeStep;
	
	cout 
//	<< "touchEvent: " 
//			<< ", " 
			<< playEvent.pluckpos
			<< ", " << playEvent.velocity 
//			<< ", " << maxVel 
			<< ", " << playEvent.acceleration
//			<< ", " << maxAcc 
//			<< ", " << maxTime 
			<< ", " << timeStep
			<< ", " << ofGetLastFrameTime()
			<< endl;
	
//	cout << "touchEvent: " 
//			<< " pluckpos: " << playEvent.pluckpos
//			<< " vel: " << playEvent.velocity 
//			<< " maxVel: " << maxVel 
//			<< " acc: " << playEvent.acceleration
//			<< " maxAcc " << maxAcc 
//			<< " maxTime: " << maxTime 
//			<< " time: " << timeStep
//			<< " ofGetElapsedTimef() : " << ofGetLastFrameTime()
//			<< endl;
//	
			
//	ofResetElapsedTimeCounter();
	
	return playEvent;
}

