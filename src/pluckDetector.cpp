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
	
	pluckParams.pos = 0;
    pluckParams.vel = 0;
    pluckParams.acc = 0;
    pluckParams.note = 0;
	
//#ifdef USE_XML_NODE_CONFIG
//	recordContext.setupUsingXMLFile();
//#else
	recordContext.setup();	// all nodes created by code -> NOT using the xml config file at all
//#endif

	recordDepth.setup(&recordContext);

	recordHandTracker.setup(&recordContext, 4);
	recordHandTracker.setSmoothing(filterFactor);		// built in openni hand track smoothing...
	recordHandTracker.setFilterFactors(filterFactor);	// custom smoothing/filtering (can also set per hand with setFilterFactor)...set them all to 0.1f to begin with  

	recordContext.toggleMirror();

//	oniRecorder.setup(&recordContext, ONI_STREAMING);

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
	string statusHands		= (string)(isTrackingHands ? "TRACKING HANDS: " + (string)(isLive ? ofToString(recordHandTracker.getNumTrackedHands()) : ofToString(playHandTracker.getNumTrackedHands())) + ""  : "NOT TRACKING");
	string statusFilterLvl	= ofToString(filterFactor);
	string statusSmoothHand = (string)(isLive ? ofToString(recordHandTracker.getSmoothing()) : ofToString(playHandTracker.getSmoothing()));

	stringstream msg;
	msg
	<< "    s : start/stop recording  : " << statusRec << endl
	<< "    p : playback/live streams : " << statusPlay << endl
	<< "    h : hand tracking         : " << statusHands << endl
	<< "[ / ] : filter hands factor   : " << statusFilterLvl << endl
	<< "; / ' : smooth hands (openni) : " << statusSmoothHand << endl
	<< "- / + : nearThreshold         : " << ofToString(nearThreshold) << endl
	<< "< / > : farThreshold          : " << ofToString(farThreshold) << endl
	<< "y / Y : tilt up / down        : " << endl
	<< endl
	<< "File  : " << oniRecorder.getCurrentFileName() << endl
	<< "FPS   : " << ofToString(ofGetFrameRate()) << "  " 
	<< endl;

	ofDrawBitmapString(msg.str(), 20, 320);

}



//--------------------------------------------------------------
bool pluckDetector::pluckDetection(){

    if (recordHandTracker.getNumTrackedHands() > 0) 
    {
        // if at least 1 hand is detected, check hand #1
        ofxTrackedHand MurrayHand = *recordHandTracker.getHand(0);
        float x = MurrayHand.projectPos.x;
        float y = MurrayHand.projectPos.y;
        float z = MurrayHand.projectPos.z;
		
		// Within set radius of point
//		414.664429 , 323.526642 , 1124.888428
		float xt=414;
		float yt=323;
		float zt=1124;

		float radius = 100;
		
		float dist=sqrtf((x-xt)*(x-xt)+(y-yt)*(y-yt)+(z-zt)*(z-zt)); 
        if (dist < radius)  
        {
        	pluckParams.pos = x;
		    pluckParams.vel = y;
		    pluckParams.acc = z;
		    pluckParams.note = dist;
            printf(" Pluck: [%f , %f , %f , dist= %f ]\n",x,y,z,dist);
        	return true;
        } 
        else 
        {
	        printf(" Missed: [%f , %f , %f , dist= %f ]\n",x,y,z,dist);
		}
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
		        break;
		default:
			break;
	}
}

