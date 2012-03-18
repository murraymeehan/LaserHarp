#ifndef _TEST_APP
#define _TEST_APP

#ifndef _PLUCK_PARAMS
	#define _PLUCK_PARAMS
	typedef struct {
		float pos;
		float vel;
		float acc;
		float note;    
	}   pluckParameters;
#endif

#include "pluckDetector.h"
#include "pluckGeneratorThreaded.h"
#include <vector>

class testApp : public ofBaseApp{

public:
	void setup(){
		detector.setup();
	}
	
	void update(){
		detector.update();
		if (detector.pluckDetection())
			generator.start(); //detector.pluckParams
	}
	
	void draw(){
		detector.draw();
	}
	
	void keyPressed  (int key){
		if (key == 'a'){
		    generator.start();
		} else if (key == 's'){
		    generator.stop();
		}
	}
	
	pluckDetector detector;
	pluckGeneratorThreaded generator;
	
	// TODO: impliment a vector of some max size of pluckGenerators for multiple notes at a time.
	vector<pluckGeneratorThreaded> generatorVector;
};

#endif
