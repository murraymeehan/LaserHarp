#include "arrayV.h"

void arrayV::setup(){
	controller.setup();
	detector.setup();
	generator.setup();
}
	
void arrayV::update(){
	controller.update();
	detector.update();
	generator.update();
	if (detector.pluckDetection()) {
		cout << "\nNow playing string #" << detector.pluckParams.stringNum;
//		generator.playSample(1, 2, 3); //detector.pluckParams
		generator.playSample(detector.pluckParams); 
	}
}

void arrayV::draw(){
	detector.draw();
}

void arrayV::keyPressed  (int key){
	controller.keyPressed(key);
	
	switch (key) {
		case 'A':
		case 'a':
			generator.freq = 110.0;
			generator.stringNum = 1;
			cout << "\nset generator.freq:" << generator.freq;
			cout << "\nset generator.stringNum:" << generator.stringNum;
			generator.play();
			break;
		case 'S':
		case 's':
			generator.freq = 440.0;
			generator.stringNum = 4;
			cout << "\nset generator.freq:" << generator.freq;
			cout << "\nset generator.stringNum:" << generator.stringNum;
			generator.play();
			break;
		case 'B':
		case 'b':
//			generator.playSample(1, 2, 3); //detector.pluckParams
			generator.playSample(detector.pluckParams);
			break;
		case 'Q':
		case 'q':
			OF_EXIT_APP(0);
			break;
		default:
			break;
	}
}
