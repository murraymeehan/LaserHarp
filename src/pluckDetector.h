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

typedef struct {
		ofVec3f C1;
		ofVec3f C2;
		float radius; 
	}   stringDetectorParam;

class pluckDetector : public ofThread { 
  public:
	void setup();
	void update();
	void draw();
	void keyPressed  (int key);

	void				setupDetectors();
	void				setupRecording();
	void				setupPlayback(string _filename);
	
	bool				isLive, isRecording, isFiltering;
	ofxOpenNIContext	recordContext, playContext;
	ofxDepthGenerator	recordDepth, playDepth;
	ofxHandGenerator	recordHandTracker, playHandTracker;
	ofxOpenNIRecorder	oniRecorder;
	float				filterFactor;
	
    pluckParameters		pluckParams;
    
    bool 				pluckDetection();
    bool				isPointInsideCylinder(ofVec3f P, ofVec3f C1, ofVec3f C2, float CR);
    
    int numStrings;
    stringDetectorParam 		strings[]; // 2 strings ought to be enough for any musician ;P
    
};

#endif
