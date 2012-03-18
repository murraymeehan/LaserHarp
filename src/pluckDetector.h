#ifndef _pluckDetector
#define _pluckDetector

#ifndef _PLUCK_PARAMS
	#define _PLUCK_PARAMS
	typedef struct {
		float pos;
		float vel;
		float acc;
		float note;    
	}   pluckParameters;
#endif

#include "ofxOpenNI.h"
#include "ofVec3f.h"

class pluckDetector : public ofThread { 
  public:
	void setup();
	void update();
	void draw();
	void keyPressed  (int key);

	void				setupPlayback(string _filename);
	string				generateFileName();
	bool				isLive, isTracking, isRecording;
	bool				isTrackingHands, isFiltering;
	
	ofxOpenNIContext	recordContext, playContext;
	ofxDepthGenerator	recordDepth, playDepth;
	ofxHandGenerator	recordHandTracker, playHandTracker;
//	ofxUserGenerator	recordUser, playUser;
	ofxOpenNIRecorder	oniRecorder;
	int					nearThreshold, farThreshold;
	float				filterFactor;
	
    pluckParameters		pluckParams;
    bool 				pluckDetection();
};

#endif
