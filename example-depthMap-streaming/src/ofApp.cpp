#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofSetFrameRate(60);

	ofSetWindowTitle("example-depthMap-streaming");

	//setup gui
	gui.setup("kinect", "kinect.xml"); // most of the time you don't need a name
	gui.add(leftMarginFloatSlider.setup("LEFT MARGIN", 0, 0, 0.5));
	gui.add(rightMarginFloatSlider.setup("RIGHT MARGIN", 0, 0, 0.5));
	gui.add(topMarginFloatSlider.setup("TOP MARGIN", 0, 0, 0.5));
	gui.add(bottomMarginFloatSlider.setup("BOTTOM MARGIN", 0, 0, 0.5));
	
	gui.loadFromFile("kinect.xml");

	//setup kinect
	auto kinectSettings = ofxAzureKinect::DeviceSettings();
	kinectSettings.synchronized = false;
	kinectSettings.updateWorld = false;
	kinectSettings.depthMode = K4A_DEPTH_MODE_WFOV_2X2BINNED;
	kinectSettings.colorResolution = K4A_COLOR_RESOLUTION_OFF;
	kinectSettings.cameraFps = K4A_FRAMES_PER_SECOND_30;
	kinectSettings.updateColor = false;
	kinectSettings.updateIr = true;
	kinectSettings.updateVbo = false;
	kinectSettings.updateWorld = false;
	kinectSettings.synchronized = false;

	if (this->kinectDevice.open(kinectSettings)){
		this->kinectDevice.startCameras();
	}

	//setup transmitter
	kinectDepthMapTransmitter.setup(4444, 2);
	kinectDepthMapTransmitter.start();
}

//--------------------------------------------------------------
void ofApp::exit(){
	this->kinectDevice.close();
}

//--------------------------------------------------------------
void ofApp::update(){
	//apply crop
	if (this->kinectDevice.isStreaming()) {

		depthPixels = kinectDevice.getDepthPix();
		const float w = depthPixels.getWidth();
		const float h = depthPixels.getHeight();

		for (int y = 0; y < depthPixels.getHeight(); y++) {
			for (int x = 0; x < depthPixels.getWidth(); x++) {
				int index = depthPixels.getPixelIndex(x, y);

				if (x > leftMarginFloatSlider* w&& x < w - rightMarginFloatSlider * w &&
					y > topMarginFloatSlider* h&& y < h - bottomMarginFloatSlider * h) {
					//depthPixels[index] = depthPixels[index] * 6.5f;
				}
				else {
					depthPixels[index] = 0;
				}
			}
		}

		depthTexture.loadData(depthPixels);
	}

	kinectDepthMapTransmitter.update(depthPixels);
}

//--------------------------------------------------------------
void ofApp::draw(){
	ofBackground(128);

	if (this->kinectDevice.isStreaming()){
		depthTexture.draw(0, 0);
	}

	gui.draw();

	ofDrawBitmapStringHighlight(ofToString(ofGetFrameRate(), 2) + " FPS", 10, 20);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
