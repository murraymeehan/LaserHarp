#ifndef _ARRAYV_MOTION_DETECTOR
#define _ARRAYV_MOTION_DETECTOR

#include "ofxOpenNI.h"
#include "ofVectorMath.h"

#ifndef _PLUCK_PARAMS
	#define _PLUCK_PARAMS
typedef struct {
	float r;
	float z;
	float stringNum; 
}   pluckParameters;
#endif

typedef struct {
		ofVec3f C1;
		ofVec3f C2;
		float radius; 
	}   stringDetectorParam;

class motionDetector : public ofThread { 
  public:
	void 				setup();
	void 				update();
	void 				draw();
	void 				keyPressed  (int key);
	
	void				setupRecording();
	void				setupPlayback(string _filename);
	bool				isLive, isRecording, isFiltering;
	ofxOpenNIContext	recordContext, playContext;
	ofxDepthGenerator	recordDepth, playDepth;
	ofxHandGenerator	recordHandTracker, playHandTracker;
	ofxOpenNIRecorder	oniRecorder;
	float				filterFactor;
	int numHands;
    
	void				setupDetectors();
	
	bool 				pluckDetection();
    bool				isPointInsideCylinder(int stringNum, ofVec3f P, ofVec3f C1, ofVec3f C2, float CR);
    bool				isPointInsideBox(int stringNum, ofVec3f P, ofVec3f C1, ofVec3f C2, float CR);
    pluckParameters		pluckParams;
    ofVec3f 			handPos;
    int 				numStrings;
    
    stringDetectorParam stringConfig[24]; // # of stringConfig must be set manually here. no idea why.
};

#endif
