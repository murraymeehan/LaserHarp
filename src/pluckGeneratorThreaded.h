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

		pluckGeneratorThreaded();

		void start();
        void stop();
		void threadedFunction();

};

#endif
