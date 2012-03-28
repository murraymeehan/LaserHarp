#include "arduinoSerial.h"

//--------------------------------------------------------------
void arduinoSerial::setup(){	 

	bSendSerialMessage = false;
	byte1 = 0x00;
	byte2 = 0x00;
	byte3 = 0x00;

	//----------------------------------- 
	serial.listDevices();
	vector <ofSerialDeviceInfo> deviceList = serial.getDeviceList();
	
	//set the comm rate before setting port
	serial.setup(0, 9600); 
	
	//----------------------------------- note:
	// < this should be set
	// to whatever com port
	// your serial device is
	// connected to.
	// (ie, COM4 on a pc, /dev/tty.... on linux, /dev/tty... on a mac)
	// arduino users check in arduino app....
	
	//serial.setup("COM4");  						  // windows example
//	serial.setup("/dev/tty.usbserial-A8004JtY",9600); // mac osx example
	serial.setup("/dev/ttyUSB1", 9600);			  //linux example

}

//--------------------------------------------------------------
void arduinoSerial::update(){
	
	if (bSendSerialMessage){
		
		serial.writeByte(byte1);
		serial.writeByte(byte2);
		serial.writeByte(byte3);
		serial.writeByte(0x00);
		
		bSendSerialMessage = false;
	}
}

void arduinoSerial::keyPressed  (int key){
       //generate a chromatic scale
       if (key == 'c'){
               byte1 = 0xFF;
               byte2 = 0xFF;
               byte3 = 0xFF;
               bSendSerialMessage = true;
       }
       //generate a major scale
       if (key == 'm'){
               byte1 = 0xAB;
               byte2 = 0x5A;
               byte3 = 0xB5;
               bSendSerialMessage = true;
       }
       //generate a minor scale
       if (key == 'n'){
               byte1 = 0x5A;
               byte2 = 0xD5;
               byte3 = 0xAD;
               bSendSerialMessage = true;
       }
       //generate a  major pentatonic scale
       if (key == 'p'){
               byte1 = 0x29;
               byte2 = 0x52;
               byte3 = 0x95;
               bSendSerialMessage = true;
       }
       //generate a minor pentatonic scale
       if (key == 'o'){
               byte1 = 0x4A;
               byte2 = 0x94;
               byte3 = 0xA9;
               bSendSerialMessage = true;
       }
       //use only lower octave
       if (key == 'l'){
               byte1 = 0x00;
               byte2 = 0x0F;
               byte3 = 0xFF;
               bSendSerialMessage = true;
       }
       //use only upper octave
       if (key == 'k'){
               byte1 = 0xFF;
               byte2 = 0xF0;
               byte3 = 0x00;
               bSendSerialMessage = true;
       }
       //use lower lights of low octave and upper lights of high octave
       if (key == 'b'){
               byte1 = 0xA8;
               byte2 = 0x00;
               byte3 = 0x15;
               bSendSerialMessage = true;
       }
}
