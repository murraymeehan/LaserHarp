#include "pluckDetector.h"

//--------------------------------------------------------------
void pluckDetector::setup() {

	setupDetectors();
	
	setupRecording();
	
}

void pluckDetector::setupDetectors() {
	
	pluckParams.r 	= 0;
    pluckParams.z 	= 0;
    pluckParams.stringNum = 0;
	
	/*
	Points on my polyview monitor
		Le Diagram
	----------------- 
	|0      1      2|	
	|               |		Monitor is 40cm wide, 35cm tall.
	|3      4      5|		Kinect is 110cm to right of monitor, pointing towards it.
	|               |		Everything shown is about 10cm in front of monitor.
	|6      7      8|		Hand blob printed to screen is about 5cm wide
	-----------------
	Octave/Matlab:
	#	x,y,z
	1	180.852341,214.302048,1468.243286
		150.295135,204.456268,1465.558105
	2	165.850174,207.792801,1280.754883
	3	164.678391,202.472794,1088.093872
		164.973907,195.862259,1086.240479
	4	189.734222,269.359375,1447.929199
	5	165.683777,277.729248,1287.288208
	6	171.225281,296.035126,1117.386719
	7	198.520508,331.658142,1467.137939
		(kinect was tracking wrist instead of palm)
	8	207.749512,342.652100,1281.703491
		(kinect was tracking wrist instead of palm)
	9	164.703644,368.955109,1086.351807
	*/

	// Some handy points around my monitor, recorded by hand
	ofVec3f POMPM[9]; //POMPM is PointsOnMyPolyviewMonitor
	POMPM[0] = ofVec3f( 180.852341,214.302048,1468.243286 );
	POMPM[1] = ofVec3f( 165.850174,207.792801,1280.754883 );
	POMPM[2] = ofVec3f( 164.678391,202.472794,1088.093872 );
	POMPM[3] = ofVec3f( 189.734222,269.359375,1447.929199 );
	POMPM[4] = ofVec3f( 165.683777,277.729248,1287.288208 );
	POMPM[5] = ofVec3f( 171.225281,296.035126,1117.386719 );
	POMPM[6] = ofVec3f( 198.520508,331.658142,1467.137939 );
	POMPM[7] = ofVec3f( 207.749512,342.652100,1281.703491 );
	POMPM[8] = ofVec3f( 164.703644,368.955109,1086.351807 );
	
	// Choosing some strings
	numStrings = 2;
	strings[0].C1 = POMPM[0];
	strings[0].C2 = POMPM[2];
	strings[0].radius = strings[0].C1.distance(strings[0].C2) / 2; // that's about 2cm
	strings[1].C1 = POMPM[6];
	strings[1].C2 = POMPM[8];
	strings[1].radius = strings[1].C1.distance(strings[1].C2) / 2; 
}

void pluckDetector::setupRecording() {

	ofBackground(0, 0, 0);

	isLive			= true;
	isFiltering		= false;
	isRecording		= false;
	filterFactor 	= 0.1f; // turn this off to get more discrete values
	
	//	recordContext.setupUsingXMLFile();
	recordContext.setup();	// all nodes created by code -> NOT using the xml config file at all

	recordDepth.setup(&recordContext);

	recordHandTracker.setup(&recordContext, 4);
	if (isFiltering) {
		recordHandTracker.setSmoothing(filterFactor);		// built in openni hand track smoothing...
		recordHandTracker.setFilterFactors(filterFactor);	// custom smoothing/filtering (can also set per hand with setFilterFactor)...set them all to 0.1f to begin with  
	}

	recordContext.toggleMirror();
	
	oniRecorder.setup(&recordContext, ONI_STREAMING);
}


void pluckDetector::setupPlayback(string _filename) {

	playContext.shutdown();
	playContext.setupUsingRecording(ofToDataPath(_filename));
	playDepth.setup(&playContext);

	playHandTracker.setup(&playContext, 4);
	if (isFiltering) {
		playHandTracker.setSmoothing(filterFactor);			// built in openni hand track smoothing...
		playHandTracker.setFilterFactors(filterFactor);		// custom smoothing/filtering (can also set per hand with setFilterFactor)...set them all to 0.1f to begin with
	}
	
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
	
	stringstream msg;
	msg
	<< "  S/s : start/stop recording  : " << (string)(!isRecording ? "READY" : "RECORDING") << endl
	<< "  P/p : playback/live streams : " << (string)(isLive ? "LIVE STREAM" : "PLAY STREAM") << endl
	<< "  [/] : enable/disable hand filtering : " << isFiltering << endl
	<< "  ;/' : handFiltering param : " << filterFactor << endl
	<< "  ;/' : handSmoothing param : " << recordHandTracker.getSmoothing() << endl
	<< "  Q/q : quit program : " << endl
	<< endl
	<< "File  : " << oniRecorder.getCurrentFileName() << endl
	<< "FPS   : " << ofToString(ofGetFrameRate()) << "  " 
	<< endl;
 	
	ofDrawBitmapString(msg.str(), 20, 320);

}


//--------------------------------------------------------------
bool pluckDetector::isPointInsideCylinder(ofVec3f P, ofVec3f C1, ofVec3f C2, float CR){
    
    printf("P=[%f,%f,%f], C1=[%f,%f,%f], C2=[%f,%f,%f], CR=[%f]\n",P[0],P[1],P[2],C1[0],C1[1],C1[2],C2[0],C2[1],C2[2],CR);
    
    ofVec3f CN = ofVec3f(C2 - C1).getNormalized();
    
    float fDistanceToPlane1 = CN.dot(P-C1);
	float fDistanceToPlane2 = CN.dot(C2-P);
	
	printf(" fDistanceToPlane1: [%f]\n fDistanceToPlane2: [%f]\n",fDistanceToPlane1,fDistanceToPlane2);
	
	if ((fDistanceToPlane1 > 0 ) && (fDistanceToPlane2 > 0)) {
		ofVec3f TempP =  P - (CN * fDistanceToPlane1) - C1;
		float fDistanceFromCenter = TempP.squareLength();
		printf(" fDistanceFromCenter: [%f]\n",fDistanceFromCenter);
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
bool pluckDetector::pluckDetection(){

    if (recordHandTracker.getNumTrackedHands() > 0) // if at least 1 hand is detected, report hand #1
    {
        ofxTrackedHand hand = *recordHandTracker.getHand(0);
        ofVec3f handPos = ofVec3f(hand.projectPos);
        
        // Method 3: check if is handPos inside any of the string-cylinders
    	// TODO: Design decision: Should the user be able to pluck more than one string at a time? If so, don't use for-loop here.
        for (int i=0;i<numStrings;i++) {
			if (isPointInsideCylinder(handPos, strings[i].C1, strings[i].C2, strings[i].radius)) 
				return true;
        }
        return false;
        
		/*
		// Method 2: check if is handPos inside a cylinder
		// (musical) string parameters:
		ofVec3f C1(171.225281,296.035126,1117.386719 ); // 2 sides of my polyview monitor, marked w/ pencil
		ofVec3f C2(189.734222,269.359375,1447.929199);
		float r = C1.distance(C2) / 2; // I'm not sure what a reasonable default would be.
		if (isPointInCylinder(handPos, C1, C2, r)) {
			//printf(" Intersection: [%f,%f,%f , d1= %f ], d2= %f ]\n",handPos[0],handPos[1],handPos[2],d1,d2);
			return true;
		} else {
			//printf(" Miss: [%f,%f,%f , d1= %f ], d2= %f ]\n",handPos[0],handPos[1],handPos[2],d1,d2);
			return false;
		}
		*/
		
		/*
		// Method 1: Is hand within 'r' of either of the end points?
		// (musical) string parameters:
		ofVec3f C1(171.225281,296.035126,1117.386719 ); // 2 sides of my polyview monitor, marked w/ pencil
		ofVec3f C2(189.734222,269.359375,1447.929199);
		float r = C1.distance(C2) / 2; // I'm not sure what a reasonable default would be.
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
void pluckDetector::keyPressed(int key){

	float smooth;

	switch (key) {
		case 'r':
		case 'R':
			if (isRecording) {
				oniRecorder.stopRecord();
				isRecording = false;
				break;
			} else {
//				oniRecorder.startRecord(generateFileName());
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

