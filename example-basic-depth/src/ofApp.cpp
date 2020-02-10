#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofSetFrameRate(60);

	auto kinectSettings = ofxAzureKinect::DeviceSettings();
	kinectSettings.synchronized = false;
	kinectSettings.updateWorld = false;
	kinectSettings.depthMode = K4A_DEPTH_MODE_NFOV_UNBINNED;
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
}


//--------------------------------------------------------------
void ofApp::exit(){
	this->kinectDevice.close();
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
	ofBackground(128);

	if (this->kinectDevice.isStreaming()){
		this->kinectDevice.getDepthTex().draw(0, 0);
	}

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
