/* Changelog:
20120123:
1. Find way to retrieve hand position

*/

# pragma once

#ifndef _TEST_APP
#define _TEST_APP
#if defined (TARGET_OSX) //|| defined(TARGET_LINUX) // only working on Mac/Linux at the moment (but on Linux you need to run as sudo...)
    // || defined (TARGET_LINUX) // linux hardware controls via libUSB aren't working in this distro of ofxOpenNI.
    #define HARDWARE_CONTROLS
#endif
//#define USE_XML_NODE_CONFIG // Uncomment this to use an XML config file to select Kinect's OpenNI node. Otherwise it is hardcoded.
//#define USE_IR // Uncomment this to use infra red instead of RGB cam...

#include "ofxOpenNI.h"
#include "ofxMarsyasNetwork.h"
#include "ofMain.h"

#include <cstdio>
#include <string>

#include "MarSystemManager.h"
#include "SoundFileSource.h"
#include "SoundFileSink.h"
#include "Gain.h"
#include "Series.h"
#include "CommandLineOptions.h"

class testApp : public ofBaseApp{

public:


	void setup();
	void update();
	void draw();

	void keyPressed  (int key);
	void windowResized(int w, int h);

	void	setupRecording(string _filename = "");
//	void	setupPlayback(string _filename);
	string	generateFileName(string ending = ".oni");

	bool				isLive, isTracking, isRecording, isCloud, isCPBkgnd, isMasking;
	bool				isTrackingHands, isFiltering;

	ofxOpenNIContext	recordContext, playContext;
	ofxDepthGenerator	recordDepth, playDepth;

#ifdef USE_IR
	ofxIRGenerator		recordImage, playImage;
#else
	ofxImageGenerator	recordImage, playImage;
#endif

	ofxHandGenerator	recordHandTracker, playHandTracker;

	ofxUserGenerator	recordUser, playUser;
	ofxOpenNIRecorder	oniRecorder;

//	void				drawMasks();
	void				drawPointCloud(ofxUserGenerator * user_generator, int userID);

	int					nearThreshold, farThreshold;
//	int					pointCloudRotationY;

	ofImage				allUserMasks, user1Mask, user2Mask, depthRangeMask;

//	float				filterFactor;

	void                detectStrum();
//	void                mwmPlayNote();
//    void                sfplay3(string sfName);
//    float               distance3D(float x1, float y1, float z1, float x2, float y2, float z2);
//    ofxMarsyasNetwork   mgn;

//    ofSoundPlayer  beats;
//    ofTrueTypeFont	font;

    ofstream logFile;
};

#endif
