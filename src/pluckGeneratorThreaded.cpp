#include "pluckGeneratorThreaded.h"

pluckGeneratorThreaded::pluckGeneratorThreaded(){
	durationSamples = 300;
	isInitialized = false;
}

void pluckGeneratorThreaded::start(){
	startThread(false, true);   // blocking, verbose
}

void pluckGeneratorThreaded::stop(){
    stopThread();
}

void pluckGeneratorThreaded::threadedFunction(){

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
		else 
		{
			if (durationSamples > 0)
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
}		

