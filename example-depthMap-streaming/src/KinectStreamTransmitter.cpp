#include "KinectStreamTransmitter.h"

#include <zlib.h>

void KinectStreamTransmitter::setup(int port, int numBytePerPixel) {
	KinectStreamTransmitter::port = port;
	KinectStreamTransmitter::numBytePerPixel = numBytePerPixel;

	KinectStreamTransmitter::running = false;
}

void KinectStreamTransmitter::start() {
	running = true;

	startThread();
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
			long lastTime;

			while (sent) {
				lastTime = ofGetElapsedTimeMillis();
				sent = send();
				/*long diff = ofGetElapsedTimeMillis() - lastTime;
				long sleepTime = MAX(0, 16 - diff);
				if (sleepTime > 0) {
					ofSleepMillis(sleepTime);
				}*/
				ofSleepMillis(30);
				/*
				if (input.available() > 0)
							{
								BufferedReader br = new BufferedReader(new InputStreamReader(input));
								String command = br.readLine();
								if (command.equals(KinectStreamMessages.Restart))
								{
									System.out.println("Restart command received!");
									kinect.restart();
								}
							}
				*/
			}

			//TODO: only first client
			//break;
		}
		
	}
}

void KinectStreamTransmitter::update(ofShortPixels& depthImage) {
	KinectStreamTransmitter::depthImage = depthImage;
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
		sendData.length = compress(originalData.data, originalData.length, &sendData.data[4], sendData.length);
		
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

int KinectStreamTransmitter::compress(char* uncompressedBytes, unsigned int lengthUncompressed, char* compressedByte, int lenghtCompressed) {
	if (uncompressedBytes == NULL) return -1;	//TODO: return NULL?

	z_stream defstream;
	defstream.zalloc = Z_NULL;
	defstream.zfree = Z_NULL;
	defstream.opaque = Z_NULL;
	defstream.avail_in = lengthUncompressed; // size of input
	defstream.avail_out = lenghtCompressed;
	defstream.next_in = (Bytef*) (uncompressedBytes);
	defstream.next_out = (Bytef*)compressedByte; // output char arra

	//source https://gist.github.com/arq5x/5315739#file-zlib-example-cpp-L35
	// the actual compression work.
	if (deflateInit(&defstream, Z_BEST_SPEED) != Z_OK) {
		cout << "deflateInit(&defstream, Z_BEST_SPEED) != Z_OK" << endl;
		free(uncompressedBytes);
		return -1;
	}

	int deflateResult = deflate(&defstream, Z_FINISH);
	deflateEnd(&defstream);

	if (deflateResult != Z_STREAM_END) {
		std::cout << "couldn't finish deflation" << std::endl;
		return -1;
	}

	return defstream.total_out;
}