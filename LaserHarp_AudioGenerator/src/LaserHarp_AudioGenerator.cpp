#include "LaserHarp_AudioGenerator.h"

void LaserHarp_AudioGenerator::setup() {
	
	setTouchObjectProperties();

	//String names
	namesArray = new string[12];
	namesArray[0] = "src0";
	namesArray[1] = "src1";
	namesArray[2] = "src2";
	namesArray[3] = "src3";
	namesArray[4] = "src4";
	namesArray[5] = "src5";
	namesArray[6] = "src6";
	namesArray[7] = "src7";
	namesArray[8] = "src8";
	namesArray[9] = "src9";
	namesArray[10] = "src10";
	namesArray[11] = "src11";
	
	/* Set default values to play on an uninitialized note 
		(defaults are not required if these values are always 
		obtainable from the pluckParams sent by motionDetector)
	*/
	frequency = 220.0;
	pluckpos = 0.5;
	loss = 1.0;
	stringnum = 0;
	
	playbacknet = ofxMarsyasNetwork::mng.create("Series", "playbacknet");
	fanout = ofxMarsyasNetwork::mng.create("Fanout", "fanout");
	playbacknet->addMarSystem(fanout);
	for (int i=0;i<12;i++) {
		fanout->addMarSystem(ofxMarsyasNetwork::mng.create("Plucked", namesArray[i]));
		fanout->updControl("Plucked/"+namesArray[i]+"/mrs_real/nton",(double)0);
		fanout->updControl("Plucked/"+namesArray[i]+"/mrs_real/frequency",(double)freqArray[stringnum]);
	}
	/* Mixing the fanout sub-systems together can work in a few different ways. 
	Using MixToMono sounds more pleasant than Sum or neither.
	//playbacknet->addMarSystem(ofxMarsyasNetwork::mng.create("Sum", "sum"));
	*/
	playbacknet->addMarSystem(ofxMarsyasNetwork::mng.create("MixToMono", "sum"));
	/* 	Setting gain to 0.0, or any other value, has no effect. 
		Is Gain a defective MarSystem? Am I using it wrong? 
	playbacknet->addMarSystem(ofxMarsyasNetwork::mng.create("Gain", "gt"));
	playbacknet->updControl("AudioSink/gt/mrs_real/gain", 0.0);
	*/
	
	///////////////////////////////////////
	// TODO: try adding this for mean + variance patches
	// http://marsyas.info/docs/manual/marsyas-user/Patching-example-of-Feature-extraction.html
	// conclusion: well that sure is irritating. just a constant buzzing.	
//	playbacknet->addMarSystem(ofxMarsyasNetwork::mng.create("Mean", "mean"));
	
	
	///////////////////////////////////////
	//
	
	
//	///////////////////////////////////////
//	// borrowed from marsyas pitchextract example app
//	// high and low bandpass filters
//	///////////////////////////////////////
//	playbacknet->addMarSystem(ofxMarsyasNetwork::mng.create("Fanout", "filters"));
//	vector<float> al(5),bl(5);

//	al(0) = 1.0;
//	al(1) = -3.9680;
//	al(2) = 5.9062;
//	al(3) = -3.9084;
//	al(4) = 0.9702;

//	bl(0) = 0.0001125;
//	bl(1) = 0.0;
//	bl(2) = -0.0002250;
//	bl(3) = 0.0;
//	bl(4) = 0.0001125;

//	lfilter = mng.create("Series", "lfilter");
//	lfilter->addMarSystem(mng.create("Filter", "llfilter"));
//	lfilter->updControl("Filter/llfilter/mrs_realvec/ncoeffs", bl);
//	lfilter->updControl("Filter/llfilter/mrs_realvec/dcoeffs", al);

//	lowpkr = mng.create("PeakerAdaptive","lowpkr");
//	lowpkr->updControl("mrs_real/peakSpacing", 0.3);
//	lowpkr->updControl("mrs_real/peakStrength", 0.95);
//	lowpkr->updControl("mrs_natural/peakStart", 0);
//	lowpkr->updControl("mrs_natural/peakEnd", hopSize);

//	lowpkr->updControl("mrs_real/peakGain", 1.0);
//	lowpkr->updControl("mrs_natural/peakStrengthReset", 4);
//	lowpkr->updControl("mrs_real/peakDecay", 0.99);
//	lfilter->addMarSystem(lowpkr);

//	filters->addMarSystem(lfilter);	
//	///////////////////////////////////////
	
	
	
	playbacknet->addMarSystem(ofxMarsyasNetwork::mng.create("AudioSink", "dest"));
	playbacknet->updControl("AudioSink/dest/mrs_bool/initAudio", true);
}

void LaserHarp_AudioGenerator::update() 
{
	playbacknet->tick();
}

void LaserHarp_AudioGenerator::playSample(pluckParameters params) 
{
	play(params);
}

void
LaserHarp_AudioGenerator::play(pluckParameters params) 
{	
	// available values: params.r, params.z, params.stringnum
	frequency = freqArray[stringnum]; //	frequency = 55.0*(1+params.stringnum);
    pluckpos = params.z;
    loss = params.loss;
    velocity  = params.velocity;
    stringnum = params.stringnum;
	
	play();
}

void
LaserHarp_AudioGenerator::play(double r_, double z_, int stringnum_, double velocity_, double loss_) 
{	
	frequency = freqArray[stringnum]; //frequency = 55.0*(1+stringnum_);
    pluckpos = z_;
    loss = loss_;
    velocity = velocity_;
    stringnum = stringnum_;
    
    play();
}

void
LaserHarp_AudioGenerator::play() 
{	
	fanout->updControl("Plucked/" + namesArray[stringnum] + "/mrs_real/nton",(double)0.0);
	fanout->updControl("Plucked/" + namesArray[stringnum] + "/mrs_real/frequency",(double)frequency);
	fanout->updControl("Plucked/" + namesArray[stringnum] + "/mrs_real/pluckpos",(double)pluckpos);
	fanout->updControl("Plucked/" + namesArray[stringnum] + "/mrs_real/loss",(double)loss);
	fanout->updControl("Plucked/" + namesArray[stringnum] + "/mrs_real/velocity",(double)velocity);
	fanout->updControl("Plucked/" + namesArray[stringnum] + "/mrs_natural/stringnum",(int)stringnum);
	fanout->updControl("Plucked/" + namesArray[stringnum] + "/mrs_real/nton",(double)1.0);
}

void LaserHarp_AudioGenerator::setTouchObjectProperties() {
	int pick = 1;
	freqArray = new float[12];
	switch (pick) {
		case 1: // Ryan's Pick
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
			break;
		case 0: // Murray's Pick
		default:
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
			break;
	}
}


