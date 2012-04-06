#ifndef _motionDetector
#define _motionDetector

/*
Features: 

plays a .oni file from command line arguement
records data to data/ directory, if it exists

TODO: 
basic touch detection (available in master branch)
reporting touchParameters (touchParameters)
train touch detection to my physical setup, and record specifications of my physical setup.
hide unwanted text which openNI prints out when scanning the environment
	possibly by setting ofLog level higher?
	might also be able to trim down ofxOpenNI to be more efficient (disable gesture Click/Wave/Raisehand detection, for instance)
enable run-from-XML-file setup

Long-term TODO: 
Confirm that avin2 sensor pack was installed correctly.
	(I should not mess with that 2 days before the demo)
*/

#include "ofxOpenNI.h"

// for logging hand position data to file
#include <iostream>
#include <fstream>
//#include "ofxCvGrayscaleImage.h"

typedef struct {
	float index;    
	float pos;
	float vel;
	float acc;
}   touchParameters;

typedef struct {
	ofVec3f C1;
	ofVec3f C2;
	float radius; 
}   stringDetectorParam;


class motionDetector { 
  public:
	void setup();
	void update();
	void draw();
	void keyPressed  (int key);
	
	void drawMasks();
	
	
	void				setupRecording(string _filename = "");
	void				setupPlayback(string _filename);
	void 				setupDetectors();
	string				generateFileName();
	bool				isLive, isTracking, isRecording, isCPBkgnd, isMasking;
	bool				isTrackingHands, isFiltering;
	ofxOpenNIContext	recordContext, playContext;
	ofxDepthGenerator	recordDepth, playDepth;
	ofxHandGenerator	recordHandTracker, playHandTracker;
	ofxUserGenerator	recordUser, playUser;
	ofxOpenNIRecorder	oniRecorder;
	int					nearThreshold, farThreshold;
	int					pointCloudRotationY;
	ofImage				allUserMasks, user1Mask, user2Mask, depthRangeMask;
	float				filterFactor;
	
    touchParameters		pluckParams;
    bool 				detect();
    ofVec3f 			handPos;
    int 				numStrings;
    
    string 				filename;
    
    //ported from main branch
    stringDetectorParam stringConfig[24]; // # of stringConfig must be set manually here. no idea why.
    bool				isPointInsideBox(int stringNum, ofVec3f P, ofVec3f C1, ofVec3f C2, float CR);
    
    int 				countElapsedPlucks;
    int 				lastObjectTouched;
    int 				timeObjectWasLastTouched[12];
    
    // For Ryan
    double posX,posY,posZ;
    int time;
	ofstream myfile;
//	ofxCvGrayscaleImage depthImage;
	
    
};

#endif
