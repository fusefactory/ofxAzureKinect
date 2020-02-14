#pragma once

#include "KinectStreamTransmitter.h"

class KinectDepthMapTransmitter : public KinectStreamTransmitter {

public:
	~KinectDepthMapTransmitter();
	void setup(int port, int numBytePerPixel = 2);

protected:
	dataLengthStruct prepareData() override;

private:
	dataLengthStruct byteBuffer;
};

