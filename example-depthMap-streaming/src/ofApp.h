#pragma once

#include "ofMain.h"

#include "ofxAzureKinect.h"
#include "KinectDepthMapTransmitter.h"
#include "ofxGui.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void exit();

		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

private:
	ofxAzureKinect::Device kinectDevice;
	KinectDepthMapTransmitter kinectDepthMapTransmitter;

	ofShortPixels depthPixels;		//depth image from kinect device
	ofTexture depthTexture;			//texture to draw depth image
		
	//gui
	ofxPanel gui;
	ofxFloatSlider  leftMarginFloatSlider;
	ofxFloatSlider  rightMarginFloatSlider;
	ofxFloatSlider  topMarginFloatSlider;
	ofxFloatSlider  bottomMarginFloatSlider;
};
