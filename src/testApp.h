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

class testApp : public ofBaseApp{

public:
	void setup();
	void update();
	void draw();
	void keyPressed  (int key);
	
	pluckDetector detector;
	pluckGeneratorThreaded TO;
	
};

#endif
