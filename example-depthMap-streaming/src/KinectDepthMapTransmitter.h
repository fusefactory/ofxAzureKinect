#pragma once

#include "KinectStreamTransmitter.h"

class KinectDepthMapTransmitter : public KinectStreamTransmitter {

public:
	void setup(ofxAzureKinect::Device* kinectDevice, int port);

protected:
	dataLengthStruct prepareData() override;

private:
	dataLengthStruct byteBuffer;
};

