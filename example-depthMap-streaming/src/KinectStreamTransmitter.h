#pragma once

#include "ofxNetwork.h"
#include "ofMain.h"

using namespace std;

struct dataLengthStruct {
	char* data;
	unsigned int length;
};

class KinectStreamTransmitter : public ofThread {

public:
	void setup(int port, int numBytePerPixel = 2);
	void start();
	void stop();
	void update(ofShortPixels& depthImage);

	float& getBitrate() { return bitrate; }		//return the current bitrate in Mbit/s
	int getPort() { return port; }

protected:
	virtual dataLengthStruct prepareData() = 0;
	void threadedFunction();
	bool send(bool sendRawBytesToAll = false);			//send function to send data throught tcp/ip
	int compress(char* uncompressedBytes, unsigned int lengthUncompressed, char* compressedByte, int lenghtCompressed);

	int numBytePerPixel;			//number of byte for each pixels used for the transmission
	ofShortPixels depthImage;		//depth image to sed

private:
	ofxTCPServer tcpServer;

	int clientId;			//current clientID to 
	int port;				//port for tpc server
	bool running;
	long lastTime = 0;
	float bitrate = 0;	//Mbit
	long bytePerSecond = 0;
};

