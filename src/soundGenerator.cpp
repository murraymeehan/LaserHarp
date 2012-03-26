#include "soundGenerator.h"
#include <stdlib.h>

void soundGenerator::setup() {
	
	stringFrequencies = new float[12];
	stringFrequencies[0] = 82.4;
	stringFrequencies[1] = 87.3;
	stringFrequencies[2] = 110;
	stringFrequencies[3] = 116.541;
	stringFrequencies[4] = 146.8;
	stringFrequencies[5] = 155.529;
	stringFrequencies[6] = 196;
	stringFrequencies[7] = 207.655;
	stringFrequencies[8] = 246.9;
	stringFrequencies[9] = 261.581;
	stringFrequencies[10] = 329.6;
	stringFrequencies[11] = 349.199;
	
	// polyphonic!
	playbacknet = ofxMarsyasNetwork::mng.create("Series", "playbacknet");
	fanout = ofxMarsyasNetwork::mng.create("Fanout", "fanout");
	playbacknet->addMarSystem(fanout);
	fanout->addMarSystem(ofxMarsyasNetwork::mng.create("Plucked", "src0"));
	fanout->updControl("Plucked/src0/mrs_real/frequency",1*110.0);
	fanout->updControl("Plucked/src0/mrs_real/pluckpos",0.5);
	fanout->updControl("Plucked/src0/mrs_real/loss",0.99);
	fanout->updControl("Plucked/src0/mrs_natural/stringnum",0);
	fanout->updControl("Plucked/src0/mrs_real/nton",0);
	fanout->addMarSystem(ofxMarsyasNetwork::mng.create("Plucked", "src1"));
	fanout->updControl("Plucked/src1/mrs_real/frequency",2*110.0);
	fanout->updControl("Plucked/src1/mrs_real/pluckpos",0.5);
	fanout->updControl("Plucked/src1/mrs_real/loss",0.99);
	fanout->updControl("Plucked/src1/mrs_natural/stringnum",2);
	fanout->updControl("Plucked/src1/mrs_real/nton",0);
	fanout->addMarSystem(ofxMarsyasNetwork::mng.create("Plucked", "src2"));
	fanout->updControl("Plucked/src2/mrs_real/frequency",3*110.0);
	fanout->updControl("Plucked/src2/mrs_real/pluckpos",0.5);
	fanout->updControl("Plucked/src2/mrs_real/loss",0.99);
	fanout->updControl("Plucked/src2/mrs_natural/stringnum",4);
	fanout->updControl("Plucked/src2/mrs_real/nton",0);
	fanout->addMarSystem(ofxMarsyasNetwork::mng.create("Plucked", "src3"));
	fanout->updControl("Plucked/src3/mrs_real/frequency",4*110.0);
	fanout->updControl("Plucked/src3/mrs_real/pluckpos",0.5);
	fanout->updControl("Plucked/src3/mrs_real/loss",0.99);
	fanout->updControl("Plucked/src3/mrs_natural/stringnum",6);
	fanout->updControl("Plucked/src3/mrs_real/nton",0);
	fanout->addMarSystem(ofxMarsyasNetwork::mng.create("Plucked", "src4"));
	fanout->updControl("Plucked/src4/mrs_real/frequency",5*110.0);
	fanout->updControl("Plucked/src4/mrs_real/pluckpos",0.5);
	fanout->updControl("Plucked/src4/mrs_real/loss",0.99);
	fanout->updControl("Plucked/src4/mrs_natural/stringnum",8);
	fanout->updControl("Plucked/src4/mrs_real/nton",0);
	fanout->addMarSystem(ofxMarsyasNetwork::mng.create("Plucked", "src5"));
	fanout->updControl("Plucked/src5/mrs_real/frequency",4*110.0);
	fanout->updControl("Plucked/src5/mrs_real/pluckpos",0.5);
	fanout->updControl("Plucked/src5/mrs_real/loss",0.99);
	fanout->updControl("Plucked/src5/mrs_natural/stringnum",10);
	fanout->updControl("Plucked/src5/mrs_real/nton",0);
	fanout->addMarSystem(ofxMarsyasNetwork::mng.create("Plucked", "src6"));
	fanout->updControl("Plucked/src6/mrs_real/frequency",3*110.0);
	fanout->updControl("Plucked/src6/mrs_real/pluckpos",0.5);
	fanout->updControl("Plucked/src6/mrs_real/loss",0.99);
	fanout->updControl("Plucked/src6/mrs_natural/stringnum",12);
	fanout->updControl("Plucked/src6/mrs_real/nton",0);
	
	//playbacknet->addMarSystem(ofxMarsyasNetwork::mng.create("Sum", "sum"));
	//playbacknet->addMarSystem(ofxMarsyasNetwork::mng.create("MixToMono", "sum"));
	playbacknet->addMarSystem(ofxMarsyasNetwork::mng.create("AudioSink", "dest"));
	playbacknet->updControl("AudioSink/dest/mrs_bool/initAudio", true);
	
}


void soundGenerator::update() {
	playbacknet->tick();
}

void soundGenerator::playSample(int note, float velocity, float position) {

	switch (note) {
		case '0':
				play(110.0, 0.5, 0.99, 0); // frequency, pluckpos, loss, stringnum
				break;
		case '1':
				play(220.0, 0.5, 0.99, 1); // frequency, pluckpos, loss, stringnum
				break;
		case '2':
				play(330.0, 0.5, 0.99, 2); // frequency, pluckpos, loss, stringnum
				break;
		case '3':
				play(440.0, 0.5, 0.99, 3); // frequency, pluckpos, loss, stringnum
				break;
		case 'Q':
		case 'q':
			OF_EXIT_APP(0);
			break;
		default:
			break;
    }

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
		default:
			name = "src6";
			break;
	}
	
	fanout->updControl("Plucked/" + name + "/mrs_real/nton",(double)0.0);
	fanout->updControl("Plucked/" + name + "/mrs_real/frequency",(double)frequency);
	fanout->updControl("Plucked/" + name + "/mrs_real/pluckpos",(double)pluckpos);
	fanout->updControl("Plucked/" + name + "/mrs_real/loss",(double)loss);
	fanout->updControl("Plucked/" + name + "/mrs_natural/stringnum",stringnum);
	fanout->updControl("Plucked/" + name + "/mrs_real/nton",(double)1.0);
	
}

