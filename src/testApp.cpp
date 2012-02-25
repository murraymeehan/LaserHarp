#include "testApp.h"

using namespace std;

void testApp::setup() {

	isRecording		= false;

	nearThreshold = 0;
	farThreshold  = 5000;

	setupRecording();
	ofBackground(0, 0, 0);

}

void testApp::setupRecording(string _filename) {

#ifdef USE_XML_NODE_CONFIG
	recordContext.setupUsingXMLFile();
#else
	recordContext.setup();	// all nodes created by code -> NOT using the xml config file at all
#endif
	recordDepth.setup(&recordContext);
	recordImage.setup(&recordContext);

	recordUser.setup(&recordContext);
	recordUser.setMaxNumberOfUsers(2);// use this to set dynamic max number of users (NB: that a hard upper limit is defined by MAX_NUMBER_USERS in ofxUserGenerator)

	recordHandTracker.setup(&recordContext, 4);

	recordContext.toggleRegisterViewport();

	oniRecorder.setup(&recordContext, ONI_STREAMING);

}

//--------------------------------------------------------------
void testApp::update(){

    // update all nodes
    recordContext.update();
    recordDepth.update();
    recordImage.update();

    // demo getting depth pixels directly from depth gen
    depthRangeMask.setFromPixels(recordDepth.getDepthPixels(nearThreshold, farThreshold),
                                 recordDepth.getWidth(), recordDepth.getHeight(), OF_IMAGE_GRAYSCALE);

    // update tracking/recording nodes
    recordUser.update();
    if (isRecording)
        oniRecorder.update();

}

//--------------------------------------------------------------
void testApp::detectStrum(){

    if (recordHandTracker.getNumTrackedHands() > 0) {

        // if there are 1 or more hands detected, this is the first hand
        ofxTrackedHand MurrayHand = *recordHandTracker.getHand(0);

        float x = MurrayHand.projectPos.x;
        float y = MurrayHand.projectPos.y;
        float z = MurrayHand.projectPos.z;
        int t = ofGetElapsedTimeMillis();

        printf(" %f , %f , %f , %i \n,",x,y,z,t);

//        string data2 = " %f , %f , %f , %i \n,"%(x,y,z,t);
//        stringstream data;
//        data << x << " , " << y << " , " << z << " , " << t << " \n";
        if (isRecording) {
//            Write values to text file
//            logFile << data;
            logFile << x << " , " << y << " , " << z << " , " << t << " \n";
        }

//        string _timestamp = ofToString(ofGetDay()) +
//	ofToString(ofGetMonth()) +
//	ofToString(ofGetYear()) +
//	ofToString(ofGetHours()) +
//	ofToString(ofGetMinutes()) +
//	ofToString(ofGetSeconds());
    }
}

//--------------------------------------------------------------
void testApp::draw(){

	ofSetColor(255, 255, 255);
	glPushMatrix();
	glScalef(0.75, 0.75, 0.75);

    recordDepth.draw(0,0,640,480);
    recordImage.draw(640, 0, 640, 480);

    depthRangeMask.draw(0, 480, 320, 240);	// can use this with openCV to make masks, find contours etc when not dealing with openNI 'User' like objects

//    recordUser.draw(); // uncomment this to draw a silly stickman skeleton
    recordHandTracker.drawHands();
    detectStrum();

	glPopMatrix();
	ofSetColor(255, 255, 0);

	string statusRec		= (string)(!isRecording ? "READY" : "RECORDING");
	stringstream msg;

	msg
	<< "    s : start/stop recording  : " << statusRec << endl
	<< endl
	<< "File  : " << oniRecorder.getCurrentFileName() << endl;

	ofDrawBitmapString(msg.str(), 20, 560);

}


//--------------------------------------------------------------
void testApp::keyPressed(int key){

	switch (key) {
		case 's':
		case 'S':
			if (isRecording) {
				oniRecorder.stopRecord();
				isRecording = false;
				logFile.close();
				break;
			} else {
				oniRecorder.startRecord(generateFileName());
				isRecording = true;
				//	logFile.open(generateFileName(".txt"));
                logFile.open("log.txt");
				break;
			}
			break;
		case 'q':
		default:
            if (isRecording) {
				oniRecorder.stopRecord();
            }
            logFile.close();
			break;
	}
}


string testApp::generateFileName(string suffix) {

	string _root = "kinectRecord";

	string _timestamp = ofToString(ofGetDay()) +
	ofToString(ofGetMonth()) +
	ofToString(ofGetYear()) +
	ofToString(ofGetHours()) +
	ofToString(ofGetMinutes()) +
	ofToString(ofGetSeconds());

//	string _filename = (_root + _timestamp + ".oni");
	string _filename = (_root + _timestamp + suffix);

	return _filename;

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

