#include "KinectDepthMapTransmitter.h"

void KinectDepthMapTransmitter::setup(ofxAzureKinect::Device* kinectDevice, int port, int numBytePerPixel) {
	KinectStreamTransmitter::setup(kinectDevice, port, numBytePerPixel);

	byteBuffer.length = KINECT_WIDTH * KINECT_HEIGHT * numBytePerPixel;
	byteBuffer.data = new char[byteBuffer.length];
}

dataLengthStruct KinectDepthMapTransmitter::prepareData() {
	ofShortPixels tempTempMap = kinectDevice->getDepthPix();
	byteBuffer.length = 0;
	
	if (tempTempMap.size() > 0) {
		for (int i = 0; i < tempTempMap.size(); i++) {
			
			//1 byte
			if (numBytePerPixel == 1) {
				float value = tempTempMap[i];
				int div = 39;		//ipotizzo che il numero massimo in mm sia 10000 e lo divido per 256
				int r = (int)(value / div) & 255;

				byteBuffer.data[i] = r & 0xff;
			}
			//2 bytes
			else if (numBytePerPixel == 2){
				// sending only 2 most valuable bytes
				byteBuffer.data[i * 2] = (tempTempMap[i] >> 8 ) & 0xff;
				byteBuffer.data[i * 2 + 1] = (tempTempMap[i] >> 0) & 0xff;
			}
			else {
				cout << "KinectDephtMapTransmitter::prepareData - Error: value of numBytePerPixel not avaiable.";
			}
		}

		byteBuffer.length = tempTempMap.getWidth() * tempTempMap.getHeight() * numBytePerPixel;
	}

	return byteBuffer;
}