#ifndef _THREADED_OBJECT
#define _THREADED_OBJECT

#include "ofMain.h"
#include "ofxMarsyasNetwork.h"

// please be careful - threading problems are notoriously hard
// to debug and working with threads can be quite difficult


class pluckGeneratorThreaded : public ofThread {
    // threaded fucntions that share data need to use lock (mutex)
    // and unlock in order to write to that data
    // otherwise it's possible to get crashes.
    //
    // also no opengl specific stuff will work in a thread...
    // threads can't create textures, or draw stuff on the screen
    // since opengl is single thread safe
    
	public:
		ofxMarsyasNetwork::MarSystem* playbacknet;
		bool isInitialized;
	    int durationSamples;  

		pluckGeneratorThreaded(){
			durationSamples = 300;
			isInitialized = false;
		}

		void start(){
			startThread(false, true);   // blocking, verbose
        }

        void stop(){
            stopThread();
        }

		void threadedFunction(){

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

		void draw() {}

};

#endif
