#include "KinectStreamTransmitter.h"
#include "Compressor.h"

void KinectStreamTransmitter::setup(int port, int numBytePerPixel) {
	KinectStreamTransmitter::port = port;
	KinectStreamTransmitter::numBytePerPixel = numBytePerPixel;

	KinectStreamTransmitter::running = false;
}

void KinectStreamTransmitter::start() {
	running = true;

	startThread();
}

void KinectStreamTransmitter::stop() {
	stopThread();
	tcpServer.close();
}

void KinectStreamTransmitter::threadedFunction() {
	//TODO: retrive current local ip
	cout << ofGetTimestampString() + " -KinectStreamTransmitter: starting ip: TODO port: " << port << endl;

	tcpServer.setup(port, true);

	while (isThreadRunning()) {
		bitrate = 0;
		bytePerSecond = 0;

		//tcpServer.waitConnectedClient();
		// for each connected client lets get the data being sent and lets print it to the screen
		for (unsigned int i = 0; i < (unsigned int)tcpServer.getLastID(); i++) {

			if (!tcpServer.isClientConnected(i))continue;

			cout << "----- CONNECTED ------" << i << endl;

			clientId = i;

			// get the ip and port of the client
			string port = ofToString(tcpServer.getClientPort(i));
			string ip = tcpServer.getClientIP(i);
			cout <<ofGetTimestampString() + " - KinectStreamTransmitter: transmitting to " + ip + " on port: " + port << endl;

			bool sent = true;
			//long lastTime;

			while (sent) {
				if (imageToSend.isAllocated()) {
					sent = send();
				}

				std::unique_lock<std::mutex> lock(mtx);
				cv.wait(lock);

				//cout << ofGetTimestampString() << endl;
			}
		}
		
	}
}

void KinectStreamTransmitter::newData(ofShortPixels& imageToSend) {
	KinectStreamTransmitter::imageToSend = imageToSend;
	cv.notify_one();
}

bool KinectStreamTransmitter::send(bool sendRawBytesToAll) {
	bool result = false;

	//retrive data
	dataLengthStruct originalData = prepareData();

	if (originalData.length > 0) {
		dataLengthStruct sendData;
		sendData.length = originalData.length + 4;
		sendData.data = new char[sendData.length];

		//compress data and get size of compressed data, compress retrive the size of compressed data
		sendData.length = Compressor::compress(originalData.data, originalData.length, &sendData.data[4], sendData.length);
		
		//first 4 byte as int to rapresent the message size
		sendData.data[0] = (sendData.length >> 24) & 0xFF;
		sendData.data[1] = (sendData.length >> 16) & 0xFF;
		sendData.data[2] = (sendData.length >> 8) & 0xFF;
		sendData.data[3] = (sendData.length >> 0) & 0xFF;

		if (sendRawBytesToAll) {
			result = tcpServer.sendRawBytesToAll(sendData.data, sendData.length + 4);
		}
		else {
			result = tcpServer.sendRawBytes(clientId, sendData.data, sendData.length + 4);
		}

		bytePerSecond += sendData.length + 4;

		if (ofGetElapsedTimeMillis() - lastTime > 1000) {
			double diffTime = (ofGetElapsedTimeMillis() - lastTime) / 1000.0;
			bitrate = (bytePerSecond / diffTime) * 0.0000076294;
			lastTime = ofGetElapsedTimeMillis();

			bytePerSecond = 0;
		}

		//free memory
		delete[] sendData.data;
	}
	
	return result;
}