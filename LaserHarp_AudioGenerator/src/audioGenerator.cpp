#include "audioGenerator.h"

#define LH_RYANS_PICK
//#define LH_MURRAYS_PICK

void audioGenerator::setup()
{
	setupTouchObjects();
	
	playbacknet = ofxMarsyasNetwork::mng.create("Series", "playbacknet");
	fanout = ofxMarsyasNetwork::mng.create("Fanout", "fanout");
	playbacknet->addMarSystem(fanout);
	for (int i=0;i<12;i++) {
		fanout->addMarSystem(ofxMarsyasNetwork::mng.create("Plucked", objNames[i]));
		fanout->updControl("Plucked/"+objNames[i]+"/mrs_real/nton",(double)0.0);
		fanout->updControl("Plucked/"+objNames[i]+"/mrs_real/frequency",(double)freqArray[i]);
	}
	playbacknet->addMarSystem(ofxMarsyasNetwork::mng.create("MixToMono", "sum"));
	playbacknet->addMarSystem(ofxMarsyasNetwork::mng.create("AudioSink", "dest"));
	playbacknet->updControl("AudioSink/dest/mrs_bool/initAudio", true);
}

void audioGenerator::update() 
{
	playbacknet->tick();
}

void audioGenerator::play(int stringnum, double pluckpos, double velocity, double loss) 
{	
	playEvent.stringnum = stringnum;
	playEvent.pluckpos = pluckpos;
	playEvent.velocity = velocity;
	playEvent.loss = loss;
	
    play(playEvent);
}
void audioGenerator::play(playEventParameters playEvent_) 
{	
	playEvent = playEvent_;
	fanout->updControl("Plucked/"+objNames[playEvent.stringnum]+"/mrs_real/nton",(double)0.0);
	fanout->updControl("Plucked/"+objNames[playEvent.stringnum]+"/mrs_real/frequency",(double)freqArray[playEvent.stringnum]);
	fanout->updControl("Plucked/"+objNames[playEvent.stringnum]+"/mrs_real/pluckpos",(double)playEvent.pluckpos);
	fanout->updControl("Plucked/"+objNames[playEvent.stringnum]+"/mrs_real/loss",0.995 + 0.005*(double)playEvent.loss);
	fanout->updControl("Plucked/"+objNames[playEvent.stringnum]+"/mrs_real/velocity",(double)playEvent.velocity*255.0);
	fanout->updControl("Plucked/"+objNames[playEvent.stringnum]+"/mrs_natural/stringnum",(int)playEvent.stringnum);
	fanout->updControl("Plucked/"+objNames[playEvent.stringnum]+"/mrs_real/nton",(double)1.0);

	cout << "playEvent" << " : " << playEvent.pluckpos 
		<< " : " << playEvent.stringnum << " : " << playEvent.velocity 
		<< " : " << freqArray[playEvent.stringnum] << " : " << playEvent.loss 
		<< endl;
}

void audioGenerator::setupTouchObjects()
 {
	objNames = new string[12];
	objNames[0] = "string1";
	objNames[1] = "string2";
	objNames[2] = "string3";
	objNames[3] = "string4";
	objNames[4] = "string5";
	objNames[5] = "string6";
	objNames[6] = "string7";
	objNames[7] = "string8";
	objNames[8] = "string9";
	objNames[9] = "string10";
	objNames[10] = "string11";
	objNames[11] = "string12";
	
	int pick = 1;
	freqArray = new float[12];
	switch (pick) {
		case 1: // Ryan's Pick, which sounds better
//	#ifdef LH_RYANS_PICK
			freqArray[0] = 165.0;
			freqArray[1] = 247.0;
			freqArray[2] = 330.0;
			freqArray[3] = 415.0;
			freqArray[4] = 494.0;
			freqArray[5] = 165.0;
			freqArray[6] = 247.0;
			freqArray[7] = 330.0;
			freqArray[8] = 415.0;
			freqArray[9] = 494.0;
			freqArray[10] = 196;
			freqArray[11] = 207.655;
//	#endif
			break;
		case 0: // Murray's Pick 
		default:
//	#ifdef LH_MURRAYS_PICK
			freqArray[0] = 82.4;
			freqArray[1] = 87.3;
			freqArray[2] = 110;
			freqArray[3] = 116.541;
			freqArray[4] = 146.8;
			freqArray[5] = 155.529;
			freqArray[6] = 196;
			freqArray[7] = 207.655;
			freqArray[8] = 246.9;
			freqArray[9] = 261.581;
			freqArray[10] = 329.6;
			freqArray[11] = 349.199;
//	#endif
			break;
	}
}

void audioGenerator::exit() 
{
	ofLog() << "Closing LaserHarp Audio Generator.";
}


