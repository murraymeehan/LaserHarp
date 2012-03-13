#ifndef _pluckGenerator
#define _pluckGenerator

#ifndef _PLUCK_PARAMS
	#define _PLUCK_PARAMS
	typedef struct {
		float pos;
		float vel;
		float acc;
		float note;    
	}   pluckParameters;
#endif

#include "ofxMarsyasNetwork.h"

class pluckGenerator : public ofxMarsyasNetwork { 

public:
	ofMarNetwork(string name = "ofMarNetwork");
	MarSystem* playbacknet;
	
	void setup();	
	void update();
	void draw();
	
	void play(pluckParameters params); 
	bool isStringPlaying;
	
	int t;

};



#endif


