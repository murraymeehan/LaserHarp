#ifndef _pluckDetector
#define _pluckDetector

#ifndef _PLUCK_PARAMS
	#define _PLUCK_PARAMS
    typedef struct {
		float r;
		float z;
		float stringNum; 
	}   pluckParameters;
#endif

#include "ofxOpenNI.h"
#include "ofVectorMath.h"

class pluckDetector : public ofThread { 
  public:
	void setup();
	void update();
	void draw();
	void keyPressed  (int key);

	void				setupRecording();
	void				setupPlayback(string _filename);
	string				generateFileName();
	bool				isLive, isTracking, isRecording;
	bool				isTrackingHands, isFiltering;
	
	ofxOpenNIContext	recordContext, playContext;
	ofxDepthGenerator	recordDepth, playDepth;
	ofxHandGenerator	recordHandTracker, playHandTracker;
	ofxOpenNIRecorder	oniRecorder;
	int					nearThreshold, farThreshold;
	float				filterFactor;
	
    pluckParameters		pluckParams;
    bool 				pluckDetection();
    bool				isPointInCylinder(ofVec3f P, ofVec3f C1, ofVec3f C2, float CR);
    
    vector<ofVec3f> 	strings;
    
    // to draw the strings on the screen, see http://www.openframeworks.cc/documentation/ofxVectorGraphics/ofxVectorGraphics.html
    
};

#endif
