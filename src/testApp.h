#ifndef _TEST_APP
#define _TEST_APP

#ifndef _PLUCK_PARAMS
	#define _PLUCK_PARAMS
	typedef struct {
		float r;
		float z;
		float stringNum; 
	}   pluckParameters;
#endif

#include "pluckDetector.h"
#include "pluckGenerator.h"

class testApp : public ofBaseApp{

public:
	void setup();
	void update();
	void draw();
	void keyPressed  (int key);
	
	pluckDetector detector;
	pluckGenerator generator;
	
};

#endif
