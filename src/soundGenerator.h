#ifndef _ARRAYV_SOUND_GENERATOR
#define _ARRAYV_SOUND_GENERATOR

#include "ofMain.h"
#include "ofxMarsyasNetwork.h"

#ifndef _PLUCK_PARAMS
	#define _PLUCK_PARAMS
    typedef struct {
		float r;
		float z;
		float stringNum; 
	}   pluckParameters;
#endif

class soundGenerator : public ofThread {
	public:
		void setup();
		void update();
//		void playSample(int note, float velocity, float position);
		void playSample(pluckParameters params);
		void play(float frequency, float pluckpos, float loss, int stringnum);
		void play();
		
		pluckParameters params;
		ofxMarsyasNetwork::MarSystem* playbacknet;
		ofxMarsyasNetwork::MarSystem* fanout;
		
		float* freqArray; 
	    float maxVelocity;
	    
	    double freq;
	    double pluckpos;
	    double loss;
	    int stringnum;
	    
};

#endif
