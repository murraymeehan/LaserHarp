#include "testApp.h"

using namespace std;
using namespace Marsyas;
//--------------------------------------------------------------
void testApp::setup() {

	isLive			= true;
	isTracking		= false;
	isTrackingHands	= true;
	isFiltering		= false;
	isRecording		= false;
	isCloud			= false;
	isCPBkgnd		= true;
	isMasking		= true;

	nearThreshold = 500;
	farThreshold  = 1000;

	filterFactor = 0.1f;

	setupRecording();

	ofBackground(0, 0, 0);

}

void testApp::setupRecording(string _filename) {

#if defined (HARDWARE_CONTROLS)
	hardware.setup();				// libusb direct control of motor, LED and accelerometers
	hardware.setLedOption(LED_OFF); // turn off the led just for yacks (or for live installation/performances ;-)
#endif

#ifdef USE_XML_NODE_CONFIG
	recordContext.setupUsingXMLFile();
#else
	recordContext.setup();	// all nodes created by code -> NOT using the xml config file at all
#endif
	recordDepth.setup(&recordContext);
	recordImage.setup(&recordContext);

	recordUser.setup(&recordContext);
	recordUser.setSmoothing(filterFactor);				// built in openni skeleton smoothing...
	recordUser.setUseMaskPixels(isMasking);
	recordUser.setUseCloudPoints(isCloud);
	recordUser.setMaxNumberOfUsers(2);					// use this to set dynamic max number of users (NB: that a hard upper limit is defined by MAX_NUMBER_USERS in ofxUserGenerator)

	recordHandTracker.setup(&recordContext, 4);
	recordHandTracker.setSmoothing(filterFactor);		// built in openni hand track smoothing...
	recordHandTracker.setFilterFactors(filterFactor);	// custom smoothing/filtering (can also set per hand with setFilterFactor)...set them all to 0.1f to begin with

	recordContext.toggleRegisterViewport();
	recordContext.toggleMirror();

	oniRecorder.setup(&recordContext, ONI_STREAMING);
	//oniRecorder.setup(&recordContext, ONI_CYCLIC, 60);
	//read the warning in ofxOpenNIRecorder about memory usage with ONI_CYCLIC recording!!!

}

void testApp::setupPlayback(string _filename) {

	playContext.shutdown();
	playContext.setupUsingRecording(ofToDataPath(_filename));
	playDepth.setup(&playContext);
	playImage.setup(&playContext);

	playUser.setup(&playContext);
	playUser.setSmoothing(filterFactor);				// built in openni skeleton smoothing...
	playUser.setUseMaskPixels(isMasking);
	playUser.setUseCloudPoints(isCloud);

	playHandTracker.setup(&playContext, 4);
	playHandTracker.setSmoothing(filterFactor);			// built in openni hand track smoothing...
	playHandTracker.setFilterFactors(filterFactor);		// custom smoothing/filtering (can also set per hand with setFilterFactor)...set them all to 0.1f to begin with

	playContext.toggleRegisterViewport();
	playContext.toggleMirror();

}

//--------------------------------------------------------------
void testApp::update(){

#ifdef HARDWARE_CONTROLS // only working on Mac at the moment
	hardware.update();
#endif

	if (isLive) {

		// update all nodes
		recordContext.update();
		recordDepth.update();
		recordImage.update();

		// demo getting depth pixels directly from depth gen
		depthRangeMask.setFromPixels(recordDepth.getDepthPixels(nearThreshold, farThreshold),
									 recordDepth.getWidth(), recordDepth.getHeight(), OF_IMAGE_GRAYSCALE);

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
		playImage.update();

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


// take advantage of Composite (Marsyas tutorial example to play a file)
//--------------------------------------------------------------
void testApp::sfplay3(string sfName)
{
    cout << "sfplay3: Playing " << sfName << endl;

    // get rid of some stuff taking advantage of
    // composite

    // Create a series Composite
    MarSystem* series = new Series("series");

    // create a soundfilesource for reading samples
    // from the file
    MarSystem* src = new SoundFileSource("src");
    src->updctrl("mrs_string/filename", sfName);

    // simple MarSystem a gain control
    MarSystem* gain = new Gain("gain");
    gain->updctrl("mrs_real/gain", 2.0);

    // SoundFileSink writes to a file
    MarSystem* dest = new SoundFileSink("dest");
    dest->updctrl("mrs_string/filename", "ajay.au");

    // add MarSystems to series Composite
    series->addMarSystem(src);
    series->addMarSystem(gain);
    series->addMarSystem(dest);

    // we only need to change window size at the composite level
    series->updctrl("mrs_natural/inSamples", 4096);

    cout << (*series) << endl;

    // tick method basically calls process
    // with an empty input buffer and an empty output buffer
    // because soundfilesource/sink read/write to files
    // as sideeffects this works


    series->updctrl("Gain/gain/mrs_real/gain", 0.0);
    for (int i=0; i<20; i++)
    {
        series->tick();
        series->updctrl("Gain/gain/mrs_real/gain",
						series->getctrl("Gain/gain/mrs_real/gain")->to<mrs_real>() + 0.1);
    }

    // Composite deletes the added MarSystems
    // so you must not delete them
    delete series;
}


//--------------------------------------------------------------
void testApp::mwmPlayNote(){
    // import marsyas and play some kind of noise...
//    string file="data/maid.wav"; // maid.wav is a fairly long song...
    string file="data/maidclip1.wav";
    sfplay3(file);
}

//--------------------------------------------------------------
void testApp::mwmDetectStrum(){
    float x_target=-207;
    float y_target=-180;
    float z_target=2257;
    float radius=100;

    if (recordHandTracker.getNumTrackedHands() > 0) {
        ofxTrackedHand MurrayHand = *recordHandTracker.getHand(0);// if there are 1 or more hands detected, this will print the result!
        float x = MurrayHand.rawPos.X;
        float y = MurrayHand.rawPos.Y;
        float z = MurrayHand.rawPos.Z;
        printf("[ %d , %d , %d ],",x,y,z);
        // recorded target location of desk lamp: xyz= -207   -180   2257

        float dist=sqrtf((x-x_target)*(x-x_target)+(y-y_target)*(y-y_target)+(z-z_target)*(z-z_target));
        if (dist < radius) {
            mwmPlayNote();
            printf(" DING DING DING %d %d %d , dist = %d ",x,y,z,dist);
        }
        printf("\n");


    }

}

//--------------------------------------------------------------
void testApp::draw(){

	ofSetColor(255, 255, 255);

	glPushMatrix();
	glScalef(0.75, 0.75, 0.75);

	if (isLive) {

		recordDepth.draw(0,0,640,480);
		recordImage.draw(640, 0, 640, 480);

		depthRangeMask.draw(0, 480, 320, 240);	// can use this with openCV to make masks, find contours etc when not dealing with openNI 'User' like objects

		if (isTracking) {
			recordUser.draw();

			if (isMasking) drawMasks();
			if (isCloud) drawPointCloud(&recordUser, 1);	// 0 gives you all point clouds; use userID to see point clouds for specific users

		}
		if (isTrackingHands)
			recordHandTracker.drawHands();
			/////// Murray /////////////
			mwmDetectStrum();
			/////// Murray /////////////
//			if (recordHandTracker.getNumTrackedHands() > 0) {
//                ofxTrackedHand MurrayHand = *recordHandTracker.getHand(0);// if there are 1 or more hands detected, this will print the result!
//                int x = MurrayHand.rawPos.X;
//                int y = MurrayHand.rawPos.Y;
//                int z = MurrayHand.rawPos.Z;
//                printf("[ %i , %i , %i ],",x,y,z);
//                // recorded target location of desk lamp: xyz= -207   -180   2257
//                int x_target=-207;
//                int y_target=-180;
//                int z_target=2257;
//                int radius=100;
//                int dist=sqrt((x-x_target)^2+(y-y_target)^2+(z-z_target)^2);
//                if (dist < radius) {
//                printf(" DING DING DING %i %i %i , dist = %i ",x,y,z,dist);
//                }
//
//                printf("\n");
//
//			}
			///////////// Murray /////////////

	} else {

		playDepth.draw(0,0,640,480);
		playImage.draw(640, 0, 640, 480);

		depthRangeMask.draw(0, 480, 320, 240);	// can use this with openCV to make masks, find contours etc when not dealing with openNI 'User' like objects

		if (isTracking) {
			playUser.draw();

			if (isMasking) drawMasks();
			if (isCloud) drawPointCloud(&playUser, 0);	// 0 gives you all point clouds; use userID to see point clouds for specific users

		}
		if (isTrackingHands)
			playHandTracker.drawHands();
	}

	glPopMatrix();

	ofSetColor(255, 255, 0);

	string statusPlay		= (string)(isLive ? "LIVE STREAM" : "PLAY STREAM");
	string statusRec		= (string)(!isRecording ? "READY" : "RECORDING");
	string statusSkeleton	= (string)(isTracking ? "TRACKING USERS: " + (string)(isLive ? ofToString(recordUser.getNumberOfTrackedUsers()) : ofToString(playUser.getNumberOfTrackedUsers())) + "" : "NOT TRACKING USERS");
	string statusSmoothSkel = (string)(isLive ? ofToString(recordUser.getSmoothing()) : ofToString(playUser.getSmoothing()));
	string statusHands		= (string)(isTrackingHands ? "TRACKING HANDS: " + (string)(isLive ? ofToString(recordHandTracker.getNumTrackedHands()) : ofToString(playHandTracker.getNumTrackedHands())) + ""  : "NOT TRACKING");
	string statusFilter		= (string)(isFiltering ? "FILTERING" : "NOT FILTERING");
	string statusFilterLvl	= ofToString(filterFactor);
	string statusSmoothHand = (string)(isLive ? ofToString(recordHandTracker.getSmoothing()) : ofToString(playHandTracker.getSmoothing()));
	string statusMask		= (string)(!isMasking ? "HIDE" : (isTracking ? "SHOW" : "YOU NEED TO TURN ON TRACKING!!"));
	string statusCloud		= (string)(isCloud ? "ON" : "OFF");
	string statusCloudData	= (string)(isCPBkgnd ? "SHOW BACKGROUND" : (isTracking ? "SHOW USER" : "YOU NEED TO TURN ON TRACKING!!"));

	string statusHardware;

#ifdef HARDWARE_CONTROLS // only working on Mac at the moment
	ofPoint statusAccelerometers = hardware.getAccelerometers();
	stringstream	statusHardwareStream;

	statusHardwareStream
	<< "ACCELEROMETERS:"
	<< " TILT: " << hardware.getTiltAngle() << "/" << hardware.tilt_angle
	<< " x - " << statusAccelerometers.x
	<< " y - " << statusAccelerometers.y
	<< " z - " << statusAccelerometers.z;

	statusHardware = statusHardwareStream.str();
#endif

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
	<< "    m : drawing masks         : " << statusMask << endl
	<< "    c : draw cloud points     : " << statusCloud << endl
	<< "    b : cloud user data       : " << statusCloudData << endl
	<< "- / + : nearThreshold         : " << ofToString(nearThreshold) << endl
	<< "< / > : farThreshold          : " << ofToString(farThreshold) << endl
	<< "y / Y : tilt up / down        : " << endl
	<< endl
	<< "File  : " << oniRecorder.getCurrentFileName() << endl
	<< "FPS   : " << ofToString(ofGetFrameRate()) << "  " << statusHardware << endl;

	ofDrawBitmapString(msg.str(), 20, 560);

}

void testApp:: drawMasks() {
	glPushMatrix();
	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE_MINUS_DST_COLOR, GL_ZERO);
	allUserMasks.draw(640, 0, 640, 480);
	glDisable(GL_BLEND);
	glPopMatrix();
	user1Mask.draw(320, 480, 320, 240);
	user2Mask.draw(640, 480, 320, 240);

}

void testApp::drawPointCloud(ofxUserGenerator * user_generator, int userID) {

	glPushMatrix();

	int w = user_generator->getWidth();
	int h = user_generator->getHeight();

	glTranslatef(w, h/2, -500);
	ofRotateY(pointCloudRotationY);

	glBegin(GL_POINTS);

	int step = 1;

	for(int y = 0; y < h; y += step) {
		for(int x = 0; x < w; x += step) {
			ofPoint pos = user_generator->getWorldCoordinateAt(x, y, userID);
			if (pos.z == 0 && isCPBkgnd) continue;	// gets rid of background -> still a bit weird if userID > 0...
			ofColor color = user_generator->getWorldColorAt(x,y, userID);
			glColor4ub((unsigned char)color.r, (unsigned char)color.g, (unsigned char)color.b, (unsigned char)color.a);
			glVertex3f(pos.x, pos.y, pos.z);
		}
	}

	glEnd();

	glColor3f(1.0f, 1.0f, 1.0f);

	glPopMatrix();
}


//--------------------------------------------------------------
void testApp::keyPressed(int key){

	float smooth;

	switch (key) {
#ifdef HARDWARE_CONTROLS // only working on Mac at the moment
		case 'y': // up key
			hardware.setTiltAngle(hardware.tilt_angle++);
			break;
		case 'Y': // down key
			hardware.setTiltAngle(hardware.tilt_angle--);
			break;
#endif
#ifndef HARDWARE_CONTROLS
		case 'y': //OF_KEY_UP: // up key
			ofSetLogLevel( OF_LOG_VERBOSE );
			ofLog() << "Tilt not working on Linux (or windows) right now."; // TODO: printf doesn't work.
//			hardware.setTiltAngle(hardware.tilt_angle++);
			break;
		case 'Y': //OF_KEY_DOWN: // down key
            ofSetLogLevel( OF_LOG_VERBOSE );
			ofLog() << "Tilt not working on Linux (or windows) right now.";
//			hardware.setTiltAngle(hardware.tilt_angle--);
			break;
#endif
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
		case 'm':
		case 'M':
			isMasking = !isMasking;
			recordUser.setUseMaskPixels(isMasking);
			playUser.setUseMaskPixels(isMasking);
			break;
		case 'c':
		case 'C':
			isCloud = !isCloud;
			recordUser.setUseCloudPoints(isCloud);
			playUser.setUseCloudPoints(isCloud);
			break;
		case 'b':
		case 'B':
			isCPBkgnd = !isCPBkgnd;
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
        case 'q':
		default:
			break;
	}
}

string testApp::generateFileName() {

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
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

	if (isCloud) pointCloudRotationY = x;

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

