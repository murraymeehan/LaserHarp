#include "pluckDetector.h"

//--------------------------------------------------------------
void pluckDetector::setup() {

	ofBackground(0, 0, 0);

	isLive			= true;
	isTrackingHands	= true;
	isRecording		= false;

	nearThreshold = 200;
	farThreshold  = 1000;
	filterFactor = 0.1f;
	pluckParams.r = 0;
    pluckParams.z = 0;
    pluckParams.stringNum = 0;
	
	setupRecording();
	
	oniRecorder.setup(&recordContext, ONI_STREAMING);

}

void pluckDetector::setupRecording() {
//	recordContext.setupUsingXMLFile();
	recordContext.setup();	// all nodes created by code -> NOT using the xml config file at all

	recordDepth.setup(&recordContext);

	recordHandTracker.setup(&recordContext, 4);
	recordHandTracker.setSmoothing(filterFactor);		// built in openni hand track smoothing...
	recordHandTracker.setFilterFactors(filterFactor);	// custom smoothing/filtering (can also set per hand with setFilterFactor)...set them all to 0.1f to begin with  

	recordContext.toggleMirror();
}


void pluckDetector::setupPlayback(string _filename) {

	playContext.shutdown();
	playContext.setupUsingRecording(ofToDataPath(_filename));
	playDepth.setup(&playContext);

	playHandTracker.setup(&playContext, 4);
	playHandTracker.setSmoothing(filterFactor);			// built in openni hand track smoothing...
	playHandTracker.setFilterFactors(filterFactor);		// custom smoothing/filtering (can also set per hand with setFilterFactor)...set them all to 0.1f to begin with

	playContext.toggleMirror();
}


//--------------------------------------------------------------
void pluckDetector::update(){
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
void pluckDetector::draw(){
	ofSetColor(255, 255, 255);
	glPushMatrix();
	glScalef(0.75, 0.75, 0.75);
	
	if (isLive) {
		recordDepth.draw(0,0,640,480);
		recordHandTracker.drawHands();
		pluckDetection();
	} 
	else 
	{
		playDepth.draw(0,0,640,480);
		playHandTracker.drawHands();
		pluckDetection();
	}

	glPopMatrix();
	ofSetColor(255, 255, 0);

	string statusPlay		= (string)(isLive ? "LIVE STREAM" : "PLAY STREAM");
	string statusRec		= (string)(!isRecording ? "READY" : "RECORDING");
	stringstream msg;
	msg
	<< "    s : start/stop recording  : " << statusRec << endl
	<< "    p : playback/live streams : " << statusPlay << endl
	<< "- / + : nearThreshold         : " << ofToString(nearThreshold) << endl
	<< "< / > : farThreshold          : " << ofToString(farThreshold) << endl
	<< endl
	<< "File  : " << oniRecorder.getCurrentFileName() << endl
	<< "FPS   : " << ofToString(ofGetFrameRate()) << "  " 
	<< endl;

	ofDrawBitmapString(msg.str(), 20, 320);

}


//--------------------------------------------------------------
bool pluckDetector::isPointInCylinder(ofVec3f P, ofVec3f C1, ofVec3f C2, float CR){
    
    printf(" Inputs: P=[%f,%f,%f], C1=[%f,%f,%f], C2=[%f,%f,%f], CR=[%f]\n",P[0],P[1],P[2],C1[0],C1[1],C1[2],C2[0],C2[1],C2[2],CR);
    
    ofVec3f CN = ofVec3f(C2 - C1).getNormalized(); //ofVec3f CN1 = ofVec3f {C2 - C1}.getNormalized(); //?
//    CN = CN.getNormalized();
    
    float fDistanceToPlane1 = CN.dot(P-C1);
	float fDistanceToPlane2 = CN.dot(C2-P);
	
	printf(" fDistanceToPlane1: [%f]\n",fDistanceToPlane1);
	printf(" fDistanceToPlane2: [%f]\n",fDistanceToPlane2);
	
	if ( (fDistanceToPlane1 < 0 ) || (fDistanceToPlane2 < 0)) {
		printf("MISS");
		return false;
	} else {
		ofVec3f TempP =  P - (CN * fDistanceToPlane1) - C1;
		float fDistanceFromCenter = TempP.squareLength();
		printf(" fDistanceFromCenter: [%f]\n",fDistanceFromCenter);
		if (fDistanceFromCenter > CR) {
			printf("MISS");
			return false;
		} else {
			pluckParams.r = fDistanceFromCenter;
		    pluckParams.z = fDistanceToPlane1;
		    pluckParams.stringNum = 0;
		    printf("HIT!!!");
			return true;
		}
	}
}

//--------------------------------------------------------------
bool pluckDetector::pluckDetection(){

    if (recordHandTracker.getNumTrackedHands() > 0) // if at least 1 hand is detected, report hand #1
    {
        ofxTrackedHand hand = *recordHandTracker.getHand(0);
        ofVec3f handPos = ofVec3f(hand.projectPos);
        
        // parameters of the musical string
		ofVec3f C1(171.225281,296.035126,1117.386719 ); // 2 sides of my polyview monitor, marked w/ pencil
		ofVec3f C2(189.734222,269.359375,1447.929199);
		float r = C1.distance(C2) / 2; // I'm not sure what a reasonable default would be.
		
		// Method 1: is handPos inside a cylinder?
		if (isPointInCylinder(handPos, C1, C2, r)) {
			//printf(" Intersection: [%f,%f,%f , d1= %f ], d2= %f ]\n",handPos[0],handPos[1],handPos[2],d1,d2);
			return true;
		} else {
			//printf(" Miss: [%f,%f,%f , d1= %f ], d2= %f ]\n",handPos[0],handPos[1],handPos[2],d1,d2);
			return false;
		}
		
		/*
		// Method 2: Is hand within 'r' of either of the end points?
		float d1 = handPos.distance(C1); 
		float d2 = handPos.distance(C2);
		if (d1 < r) {
            printf(" Pluck1: [%f,%f,%f , d1= %f ], d2= %f ]\n",handPos[0],handPos[1],handPos[2],d1,d2);
        	return true;
        } else if (d2 < r) {
            printf(" Pluck2: [%f,%f,%f , d1= %f ], d2= %f ]\n",handPos[0],handPos[1],handPos[2],d1,d2);
        	return true;
        } else {
	        printf(" Missed: [%f,%f,%f , d1= %f ], d2= %f ]\n",handPos[0],handPos[1],handPos[2],d1,d2);
		}
		*/
    }
	return false;
}

//--------------------------------------------------------------
string pluckDetector::generateFileName() {

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
void pluckDetector::keyPressed(int key){

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
        case 'Q':
        case 'q':
        	OF_EXIT_APP(0);
	        break;
		default:
			break;
	}
}

