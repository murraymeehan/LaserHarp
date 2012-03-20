#include "pluckGenerator.h"

pluckGenerator::pluckGenerator(){
	isInitialized = false;
	pluckParams.r 	= 0;
    pluckParams.z 	= 0;
    pluckParams.stringNum = 0;
}

void pluckGenerator::start(){
	isInitialized = true;
	durationSamples = 300;
	startThread(false, true);   // blocking, verbose
}

void pluckGenerator::stop(){
    stopThread();
    isInitialized = false;
}

void pluckGenerator::threadedFunction(){

	while( isThreadRunning() != 0 )
	{
		if (!isInitialized) 
		{
			playbacknet = ofxMarsyasNetwork::mng.create("Series", "playbacknet");
			playbacknet->addMarSystem(ofxMarsyasNetwork::mng.create("MyPlucked", "src"));
			playbacknet->addMarSystem(ofxMarsyasNetwork::mng.create("Gain", "gt"));
			playbacknet->addMarSystem(ofxMarsyasNetwork::mng.create("AudioSink", "dest"));
			playbacknet->updControl("AudioSink/dest/mrs_bool/initAudio", true);
			isInitialized=true;
		} 
		else if (durationSamples > 0)
		{
			playbacknet->tick();
			durationSamples--;
		} 
		else 
		{
			break;
		}
	}
}		

