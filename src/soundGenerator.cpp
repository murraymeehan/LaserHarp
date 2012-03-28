#include "soundGenerator.h"
#include <stdlib.h>

void soundGenerator::setup() {
	
	freqArray = new float[12];
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
	
	// default values to play on an uninitialized note
	freq = 220.0;
	pluckpos = 0.5;
	loss = 1.0;
	stringnum = 0;
	
	// polyphonic!
	playbacknet = ofxMarsyasNetwork::mng.create("Series", "playbacknet");
	fanout = ofxMarsyasNetwork::mng.create("Fanout", "fanout");
	playbacknet->addMarSystem(fanout);
	fanout->addMarSystem(ofxMarsyasNetwork::mng.create("Plucked", "src0"));
//	fanout->updControl("Plucked/src0/mrs_real/frequency",freqArray[0]);
//	fanout->updControl("Plucked/src0/mrs_real/pluckpos",0.5);
//	fanout->updControl("Plucked/src0/mrs_real/loss",0.99);
//	fanout->updControl("Plucked/src0/mrs_natural/stringnum",0);
	fanout->updControl("Plucked/src0/mrs_real/nton",0);
	fanout->addMarSystem(ofxMarsyasNetwork::mng.create("Plucked", "src1"));
//	fanout->updControl("Plucked/src1/mrs_real/frequency",freqArray[1]);
//	fanout->updControl("Plucked/src1/mrs_real/pluckpos",0.5);
//	fanout->updControl("Plucked/src1/mrs_real/loss",0.99);
//	fanout->updControl("Plucked/src1/mrs_natural/stringnum",1);
	fanout->updControl("Plucked/src1/mrs_real/nton",0);
	fanout->addMarSystem(ofxMarsyasNetwork::mng.create("Plucked", "src2"));
//	fanout->updControl("Plucked/src2/mrs_real/frequency",freqArray[2]);
//	fanout->updControl("Plucked/src2/mrs_real/pluckpos",0.5);
//	fanout->updControl("Plucked/src2/mrs_real/loss",0.99);
//	fanout->updControl("Plucked/src2/mrs_natural/stringnum",2);
	fanout->updControl("Plucked/src2/mrs_real/nton",0);
	fanout->addMarSystem(ofxMarsyasNetwork::mng.create("Plucked", "src3"));
//	fanout->updControl("Plucked/src3/mrs_real/frequency",freqArray[3]);
//	fanout->updControl("Plucked/src3/mrs_real/pluckpos",0.5);
//	fanout->updControl("Plucked/src3/mrs_real/loss",0.99);
//	fanout->updControl("Plucked/src3/mrs_natural/stringnum",3);
	fanout->updControl("Plucked/src3/mrs_real/nton",0);
	fanout->addMarSystem(ofxMarsyasNetwork::mng.create("Plucked", "src4"));
//	fanout->updControl("Plucked/src4/mrs_real/frequency",freqArray[4]);
//	fanout->updControl("Plucked/src4/mrs_real/pluckpos",0.5);
//	fanout->updControl("Plucked/src4/mrs_real/loss",0.99);
//	fanout->updControl("Plucked/src4/mrs_natural/stringnum",4);
	fanout->updControl("Plucked/src4/mrs_real/nton",0);
	fanout->addMarSystem(ofxMarsyasNetwork::mng.create("Plucked", "src5"));
//	fanout->updControl("Plucked/src5/mrs_real/frequency",freqArray[5]);
//	fanout->updControl("Plucked/src5/mrs_real/pluckpos",0.5);
//	fanout->updControl("Plucked/src5/mrs_real/loss",0.99);
//	fanout->updControl("Plucked/src5/mrs_natural/stringnum",5);
	fanout->updControl("Plucked/src5/mrs_real/nton",0);
	fanout->addMarSystem(ofxMarsyasNetwork::mng.create("Plucked", "src6"));
//	fanout->updControl("Plucked/src6/mrs_real/frequency",freqArray[6]);
//	fanout->updControl("Plucked/src6/mrs_real/pluckpos",0.5);
//	fanout->updControl("Plucked/src6/mrs_real/loss",0.99);
//	fanout->updControl("Plucked/src6/mrs_natural/stringnum",6);
	fanout->updControl("Plucked/src6/mrs_real/nton",0);
	fanout->addMarSystem(ofxMarsyasNetwork::mng.create("Plucked", "src7"));
//	fanout->updControl("Plucked/src7/mrs_real/frequency",freqArray[7]);
//	fanout->updControl("Plucked/src7/mrs_real/pluckpos",0.5);
//	fanout->updControl("Plucked/src7/mrs_real/loss",0.99);
//	fanout->updControl("Plucked/src7/mrs_natural/stringnum",7);
	fanout->updControl("Plucked/src7/mrs_real/nton",0);
	fanout->addMarSystem(ofxMarsyasNetwork::mng.create("Plucked", "src8"));
//	fanout->updControl("Plucked/src8/mrs_real/frequency",freqArray[8]);
//	fanout->updControl("Plucked/src8/mrs_real/pluckpos",0.5);
//	fanout->updControl("Plucked/src8/mrs_real/loss",0.99);
//	fanout->updControl("Plucked/src8/mrs_natural/stringnum",8);
	fanout->updControl("Plucked/src8/mrs_real/nton",0);
	fanout->addMarSystem(ofxMarsyasNetwork::mng.create("Plucked", "src9"));
//	fanout->updControl("Plucked/src9/mrs_real/frequency",freqArray[9]);
//	fanout->updControl("Plucked/src9/mrs_real/pluckpos",0.5);
//	fanout->updControl("Plucked/src9/mrs_real/loss",0.99);
//	fanout->updControl("Plucked/src9/mrs_natural/stringnum",9);
	fanout->updControl("Plucked/src9/mrs_real/nton",0);
	fanout->addMarSystem(ofxMarsyasNetwork::mng.create("Plucked", "src10"));
//	fanout->updControl("Plucked/src10/mrs_real/frequency",freqArray[10]);
//	fanout->updControl("Plucked/src10/mrs_real/pluckpos",0.5);
//	fanout->updControl("Plucked/src10/mrs_real/loss",0.99);
//	fanout->updControl("Plucked/src10/mrs_natural/stringnum",10);
	fanout->updControl("Plucked/src10/mrs_real/nton",0);
	fanout->addMarSystem(ofxMarsyasNetwork::mng.create("Plucked", "src11"));
//	fanout->updControl("Plucked/src11/mrs_real/frequency",freqArray[11]);
//	fanout->updControl("Plucked/src11/mrs_real/pluckpos",0.5);
//	fanout->updControl("Plucked/src11/mrs_real/loss",0.99);
//	fanout->updControl("Plucked/src11/mrs_natural/stringnum",11);
	fanout->updControl("Plucked/src11/mrs_real/nton",0);
	fanout->addMarSystem(ofxMarsyasNetwork::mng.create("Plucked", "src12"));
//	fanout->updControl("Plucked/src12/mrs_real/frequency",freqArray[12]);
//	fanout->updControl("Plucked/src12/mrs_real/pluckpos",0.5);
//	fanout->updControl("Plucked/src12/mrs_real/loss",0.99);
//	fanout->updControl("Plucked/src12/mrs_natural/stringnum",12);
	fanout->updControl("Plucked/src12/mrs_real/nton",0);
	
	//playbacknet->addMarSystem(ofxMarsyasNetwork::mng.create("Sum", "sum"));
	playbacknet->addMarSystem(ofxMarsyasNetwork::mng.create("MixToMono", "sum"));
	
	// I set the initial gain to zero to supress the sound of newly created
	//   strings, which play as soon as they are created.
	// the mystery: why does a gain of 0 not supress all output?
	// 		in fact, gains of +20.0 and -20.0 sound about the same. 
	playbacknet->addMarSystem(ofxMarsyasNetwork::mng.create("Gain", "gt"));
	playbacknet->updControl("AudioSink/gt/mrs_real/gain", 0.0);
	
	playbacknet->addMarSystem(ofxMarsyasNetwork::mng.create("AudioSink", "dest"));
	playbacknet->updControl("AudioSink/dest/mrs_bool/initAudio", true);
}

void soundGenerator::update() 
{
	playbacknet->tick();
}

void soundGenerator::playSample(pluckParameters params) 
{
	play(55.0*(1+params.stringNum), 0.5, 0.99, params.stringNum);
}

void
soundGenerator::play(float frequency, float pluckpos, float loss, int stringnum)
{
	
	string name;
	switch (stringnum)
	{
		case 0:
			name = "src0";
			break;
		case 1:
			name = "src1";
			break;
		case 2:
			name = "src2";
			break;
		case 3:
			name = "src3";
			break;
		case 4:
			name = "src4";
			break;
		case 5:
			name = "src5";
			break;
		case 6:
			name = "src6";
			break;
		case 7:
			name = "src7";
			break;
		case 8:
			name = "src8";
			break;
		case 9:
			name = "src9";
			break;
		case 10:
			name = "src10";
			break;
		case 11:
			name = "src11";
			break;
		case 12:
			name = "src12";
			break;
		default:
			break;
	}
	
	fanout->updControl("Plucked/" + name + "/mrs_real/nton",(double)0.0);
	fanout->updControl("Plucked/" + name + "/mrs_real/frequency",(double)frequency);
	fanout->updControl("Plucked/" + name + "/mrs_real/pluckpos",(double)pluckpos);
	fanout->updControl("Plucked/" + name + "/mrs_real/loss",(double)loss);
	fanout->updControl("Plucked/" + name + "/mrs_natural/stringnum",(int)stringnum);
	fanout->updControl("Plucked/" + name + "/mrs_real/nton",(double)1.0);
	
}

