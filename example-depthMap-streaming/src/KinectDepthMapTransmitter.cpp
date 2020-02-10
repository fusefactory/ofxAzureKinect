#include "KinectDepthMapTransmitter.h"

void KinectDepthMapTransmitter::setup(ofxAzureKinect::Device* kinectDevice, int port) {
	KinectStreamTransmitter::setup(kinectDevice, port);

	byteBuffer.data = new char[KINECT_WIDTH * KINECT_HEIGHT * 2];
	byteBuffer.length = KINECT_WIDTH * KINECT_HEIGHT * 2;
}

dataLengthStruct KinectDepthMapTransmitter::prepareData() {
	ofShortPixels tempTempMap = kinectDevice->getDepthPix();
	byteBuffer.length = 0;
	
	if (tempTempMap.size() > 0) {
		for (int i = 0; i < tempTempMap.size(); i++) {
			// sending only 2 most valuable bytes
			byteBuffer.data[i * 2] = (tempTempMap[i] >> 8 ) & 0xff;
			byteBuffer.data[i * 2 + 1] = (tempTempMap[i] >> 0) & 0xff;
		}

		byteBuffer.length = tempTempMap.getWidth() * tempTempMap.getHeight() * 2;
	}

	return byteBuffer;
}