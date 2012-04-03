#include "ofMain.h"
#include "ofxMarsyasNetwork.h"
#include <stdlib.h>

typedef struct {
	float r;
	float z;
	float stringnum; 
	float velocity;
	float loss;
}   pluckParameters;

class LaserHarp_AudioGenerator : public ofThread {
	public:
		void setup();
		void update();
		void playSample(pluckParameters params);
		void play(pluckParameters params) ;
		void play(double r_, double z_, int stringnum_, double velocity_, double loss_);
		void play();
		void setTouchObjectProperties();
		
		pluckParameters params;
		ofxMarsyasNetwork::MarSystem* playbacknet;
		ofxMarsyasNetwork::MarSystem* fanout;
		ofxMarsyasNetwork::MarSystem* filters;
		ofxMarsyasNetwork::MarSystem* lfilter;
		ofxMarsyasNetwork::MarSystem* lowpkr;
		
		float* freqArray; 
	    string* namesArray;
	    
	    double frequency;
	    double pluckpos;
	    double loss;
	    double velocity;
	    int stringnum;
	    
};
