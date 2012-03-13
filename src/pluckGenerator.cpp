#include "pluckGenerator.h"

void pluckGenerator::setup() {
    // This models 1 string. We will need several.
    
//    isStringPlaying = false;
    
	playbacknet = mng.create("Series", "playbacknet");
	playbacknet->addMarSystem(mng.create("MyPlucked", "src"));
	playbacknet->addMarSystem(mng.create("Gain", "gt"));
	playbacknet->addMarSystem(mng.create("AudioSink", "dest"));
	playbacknet->updControl("AudioSink/dest/mrs_bool/initAudio", true);
}


//--------------------------------------------------------------
void pluckGenerator::update(){

	// Old way
	int t=0;
	for (t = 0; t < 10; t++)
	{
		playbacknet->tick();
	}	
	
//	// New way 1
//	if (isStringPlaying)
//		playbacknet->tick();
	
	// New way 2
//	if (t<30) {
//		playbacknet->tick();
//		t++;
//	}
	
//	if (isStringPlaying) {
//		//int t=0;
//		for (t = 0; t < 30; t++)
//		{
//			playbacknet->tick();
//		}	
//	}    
	
}

//--------------------------------------------------------------
void pluckGenerator::draw(){
}

//--------------------------------------------------------------
void pluckGenerator::play(pluckParameters params)
{
//////////	if (t>=30) {
//////////		playbacknet = mng.create("Series", "playbacknet");
//////////		playbacknet->addMarSystem(mng.create("MyPlucked", "src"));
//////////		playbacknet->addMarSystem(mng.create("Gain", "gt"));
//////////		playbacknet->addMarSystem(mng.create("AudioSink", "dest"));
//////////		playbacknet->updControl("AudioSink/dest/mrs_bool/initAudio", true);
////////////		isStringPlaying = true;
//////////		t = 0;
//////////	}
				// Apply parameters to MyPlucked 
					// not implemented in MyPlucked class yet
					// ie. playbacknet->updControl("MyPlucked/src/position/z", params[1]);
		
				// Initialize note:
		//		isStringPlaying = True;
		//		playbacknet->updControl("AudioSink/dest/mrs_bool/initAudio", true);
		//	} 
		//	else {
		//		playbacknet->updControl("AudioSink/dest/mrs_bool/initAudio", false);
//			}
}

