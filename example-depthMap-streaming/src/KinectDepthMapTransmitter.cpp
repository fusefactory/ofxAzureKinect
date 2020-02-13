#include "KinectDepthMapTransmitter.h"

void KinectDepthMapTransmitter::setup(int port, int numBytePerPixel) {
	KinectStreamTransmitter::setup(port, numBytePerPixel);

	const int maxSize = 1024 * 1024 * numBytePerPixel;
	byteBuffer.length = 0;
	byteBuffer.data = new char[maxSize];	//maximum resolution
}

dataLengthStruct KinectDepthMapTransmitter::prepareData() {
	byteBuffer.length = 0;
	
	if (depthImage.size() > 0) {
		for (int i = 0; i < depthImage.size(); i++) {
			
			//1 byte
			if (numBytePerPixel == 1) {
				float value = depthImage[i];
				int div = 39;		//ipotizzo che il numero massimo in mm sia 10000 e lo divido per 256
				int r = (int)(value / div) & 255;

				byteBuffer.data[i] = r & 0xff;
			}
			//2 bytes
			else if (numBytePerPixel == 2){
				// sending only 2 most valuable bytes
				byteBuffer.data[i * 2] = (depthImage[i] >> 8 ) & 0xff;
				byteBuffer.data[i * 2 + 1] = (depthImage[i] >> 0) & 0xff;
			}
			else {
				cout << "KinectDephtMapTransmitter::prepareData - Error: value of numBytePerPixel not avaiable.";
			}
		}

		byteBuffer.length = depthImage.getWidth() * depthImage.getHeight() * numBytePerPixel;
	}

	return byteBuffer;
}