#ifndef _motionDetector
#define _motionDetector

/*
Description:
	motionDetector is responsible for detecting physical contact with a 
		set of virtual objects ('touchObjects') seen by the Kinect sensor. 
	touchObject parameters are defined in function isPointInsideBox();

TODO: 
	For LaserBoard Pentatonic Scale: 
		automatic recognition of the laserboard,
		rearrangement of strings on top of it (using OpenCv)
	For Chop&Block Scale:
		Define and test
	For Piano Scale:
		Re-define and test
	allow recording of video and audio during play
	turn off OpenNI gesture detection on startup (Click/Wave/Raisehand)
	Check if avin2 addon to OpenNI is being used

Recent additions:
	fixed velocity/acceleration estimation
	play a .oni file from command line arguement
	records data to data/ directory, if it already exists
	basic touch detection
	reporting touchEventParameters
	uses XML-kinect-config-file found in bin/data/openni/config
	press '?' to hide/show explanation of shortcut keys
	press 'f' for fullscreen/
	plot velocity and acceleration on screen in motionDetector
	plot note params on screen in audioGenerator 
	
*/




#include "ofxOpenNI.h"
#include "ofxOsc.h"
#include "ofxOpenCv.h"
#include <vector>


typedef struct 
{
	ofVec3f C1;
	ofVec3f C2;
	float radius; 
}   touchObjectParam;

typedef struct 
{
	int stringNum;	// 0 <= stringNum < numObjects (12)
	float pluckpos;	// 0 <= pluckpos <= 1
	float velocity;	// 0 <= velocity <= 1
	float acceleration;	// 0 <= acceleration <= 1
} playEventParameters;

typedef struct 
{
	int stringNum;			//was index_;
	float pluckpos; 		//was pos_; 
			// this is a position vector relative to 
			// the object's centroid (which is the 
			// object's center for simple object shapes)
	ofVec3f velocity;		//was vel_;
	ofVec3f acceleration;	//was acc_;
} touchEventParameters;

class motionDetector { 
  public:
	void setup();
	void update();
	void draw();
	void keyPressed  (int key);
	
	// functions from ofxOpenNI-demoallfeatures example
	void 				drawMasks();
	void				setupRecording(string _filename = "");
	void				setupPlayback(string _filename);
	void 				setupDetectors();
	string				generateFileName();
	bool				isLive, isTracking, isRecording, isCPBkgnd, isMasking;
	bool				isTrackingHands, isFiltering, isShowOnscreenInstructions;
#ifdef USE_IR
	ofxIRGenerator		recordImage, playImage;
#else
	ofxImageGenerator	recordImage, playImage;
#endif
	ofxOpenNIContext	recordContext, playContext;
	ofxDepthGenerator	recordDepth, playDepth;
	ofxHandGenerator	recordHandTracker, playHandTracker;
	ofxUserGenerator	recordUser, playUser;
	ofxOpenNIRecorder	oniRecorder;
	int					nearThreshold, farThreshold;
	int					pointCloudRotationY;
	ofImage				allUserMasks, user1Mask, user2Mask, depthRangeMask;
//	float				filterFactor;
	
	string 				ONIfilename;
	// end of functions from ofxOpenNI-demoallfeatures example
	
	// touchObject (musical string)
    int 				numStrings;
    touchObjectParam 	touchObjects[24]; // Num of touchObjects must be set manually here. can switch to type* name[] and declare as new array in .cpp file.
//    touchObjectParam* 	touchObjects; // Num of touchObjects must be set manually here. can switch to type* name[] and declare as new array in .cpp file.
    
    // touchEvent detection (uses handPos + touchObject)
	bool 				detect();
	bool				isPointInsideBox(int stringNum, ofVec3f P, touchObjectParam touchObject);
	bool				isPointInsideCylinder(int stringNum, ofVec3f P, touchObjectParam touchObject);
	touchEventParameters	touchEvent;
    int 				countElapsedPlucks;
    int 				lastObjectTouched;
    int 				timeObjectWasLastTouched[12];
    float 				maxVel, maxAcc;
    double 				maxTime;
	double 				timeStep;
	ofVec3f 			handPos;
    ofVec3f 			oldPos, newPos, oldVel, newVel, oldAcc, newAcc; // these need to be perHand arrays
	
	// touchEvent -> playEvent conversion
	playEventParameters getPlayEvent(touchEventParameters touchEvent);
	playEventParameters playEvent;
    
	
	// Overhead on-screen display
	void plotData();
	ofPolyline 			lineA, lineV, lineAMax, lineVMax, lineTimeStepSize;
	int 				lineCounter;
	//bool 				displayInstructions;
	
	// TODO: video recording
	//void ofSaveFrame()
	//void ofSaveViewport(string filename)
	
    
};

#endif
