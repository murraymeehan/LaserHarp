#ifndef _ARRAYV_ARDUINO_SERIAL
#define _ARRAYV_ARDUINO_SERIAL

#include "ofMain.h"

class arduinoSerial : public ofBaseApp{
	
	public:
		void setup();
		void update();
		
		void keyPressed  (int key);
		
		bool		bSendSerialMessage;			// a flag for sending serial
// 		bool		keyIsDown[255];				// store pressed keys
// 		char		bytesRead[3];				// data from serial, we will be trying to read 3
// 		char		bytesReadString[4];			// a string needs a null terminator, so we need 3 + 1 bytes
// 		int			nBytesRead;					// how much did we read?
// 		int			nTimesRead;					// how many times did we read?
// 		float		readTime;					// when did we last read?
 		int			byte1;
		int			byte2;
		int			byte3;
	
		
		ofSerial	serial;
};

#endif