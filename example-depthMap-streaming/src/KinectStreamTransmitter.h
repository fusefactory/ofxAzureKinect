#pragma once

#include "ofxAzureKinect.h"
#include "ofxNetwork.h"
#include "ofMain.h"

using namespace std;

struct dataLengthStruct {
	char* data;
	unsigned int length;
};

static const int KINECT_WIDTH = 640;
static const int KINECT_HEIGHT = 576;


class KinectStreamTransmitter : public ofThread {

public:
	void setup(ofxAzureKinect::Device* kinectDevice, int port);
	void start();

protected:
	ofxAzureKinect::Device *kinectDevice;
	virtual dataLengthStruct prepareData() = 0;

private:
	int clientId;			//current clientID to 
	int port;				//port for tpc server
	bool running;
	ofxTCPServer tcpServer;
	void threadedFunction();
	bool send(bool sendRawBytesToAll=false);			//send function to send data throught tcp/ip
	//dataLengthStruct compress(char* data, unsigned int length);
	int compress(char* uncompressedBytes, unsigned int lengthUncompressed, char* compressedByte, int lenghtCompressed);


};
