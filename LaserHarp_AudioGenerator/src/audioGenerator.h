#include "ofMain.h"
#include "ofxMarsyasNetwork.h"
#include <stdlib.h>

typedef struct 
{
	int stringnum;	// 0 <= stringnum < numObjects
	float pluckpos;// 0 <= pluckpos <= 1			//was called z
	float velocity;	// 0 <= velocity <= 1
	float loss;		// 0 <= loss <= 1
} playEventParameters;


class audioGenerator: public ofThread {
	public:
		void setup();
		void update();
		void exit();
		
		void play(int stringnum, double pluckpos, double velocity, double loss);
		playEventParameters playEvent;
		void play(playEventParameters playEvent_) ;
		
		ofxMarsyasNetwork::MarSystem* playbacknet;
		ofxMarsyasNetwork::MarSystem* fanout;
		
		void setupTouchObjects();
		float* freqArray; 
	    string* objNames;
	    
};
