#pragma once

#include "KinectStreamTransmitter.h"

class KinectDepthMapTransmitter : public KinectStreamTransmitter {

public:
	void setup(ofxAzureKinect::Device* kinectDevice, int port, int numBytePerPixel = 2);

protected:
	dataLengthStruct prepareData() override;

private:
	dataLengthStruct byteBuffer;
};

