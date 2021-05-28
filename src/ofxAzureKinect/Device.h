#pragma once

#include <k4a/k4a.hpp>
#include <turbojpeg.h>

#include "ofBufferObject.h"
#include "ofEvents.h"
#include "ofParameter.h"
#include "ofPixels.h"
#include "ofTexture.h"
#include "ofThread.h"
#include "ofVboMesh.h"
#include "ofVectorMath.h"

#include "Types.h"

namespace ofxAzureKinect
{
	struct DeviceSettings
	{
		int deviceIndex;

		DepthMode depthMode;
		ColorResolution colorResolution;
		ImageFormat colorFormat;
		FramesPerSecond cameraFps;
		
		bool updateColor;
		bool updateIr;
		bool updateWorld;
		bool updateVbo;

		bool synchronized;

		DeviceSettings(int idx = 0);
	};

	class Device 
		: ofThread
	{
	public:
		static int getInstalledCount();

	public:
		Device();
		~Device();

		bool open(int idx = 0);
		bool open(DeviceSettings settings);
		bool close();

		bool startCameras();
		bool stopCameras();

		bool isOpen() const;
		bool isStreaming() const;

		const ofShortPixels& getDepthPix() const;
		const ofTexture& getDepthTex() const;

		const ofPixels& getColorPix() const;
		const ofTexture& getColorTex() const;

		const ofShortPixels& getIrPix() const;
		const ofTexture& getIrTex() const;

		const ofFloatPixels& getDepthToWorldPix() const;
		const ofTexture& getDepthToWorldTex() const;

		const ofFloatPixels& getColorToWorldPix() const;
		const ofTexture& getColorToWorldTex() const;

		const ofShortPixels& getDepthInColorPix() const;
		const ofTexture& getDepthInColorTex() const;

		const ofPixels& getColorInDepthPix() const;
		const ofTexture& getColorInDepthTex() const;

		const ofVbo& getPointCloudVbo() const;
		ofEvent<void> onNewDepthData;


	public:
		ofParameter<float> jointSmoothing{ "Joint Smoothing", 0.0f, 0.0f, 1.0f };
		void setUpdateTextures(bool updateTextures) { this->bUpdateTextures = updateTextures; }

	protected:
		void threadedFunction() override;

	private:
		void updatePixels();
		void updateTextures();

		void update(ofEventArgs& args);

		bool setupDepthToWorldTable();
		bool setupColorToWorldTable();
		bool setupImageToWorldTable(k4a_calibration_type_t type, k4a::image& img);

		bool updatePointsCache(k4a::image& frameImg, k4a::image& tableImg);

		bool updateDepthInColorFrame(const k4a::image& depthImg, const k4a::image& colorImg);
		bool updateColorInDepthFrame(const k4a::image& depthImg, const k4a::image& colorImg);

	private:
		int index;
		bool bOpen;
		bool bStreaming;

		bool bUpdateIr;
		bool bUpdateWorld;
		bool bUpdateVbo;
		bool bUpdateTextures;

		std::condition_variable condition;
		uint64_t pixFrameNum;
		uint64_t texFrameNum;

		std::string serialNumber;

		k4a_device_configuration_t config;
		k4a::calibration calibration;
		k4a::transformation transformation;
		k4a::device device;
		k4a::capture capture;

		tjhandle jpegDecompressor;

		ofShortPixels depthPix;
		ofTexture depthTex;

		ofPixels colorPix;
		ofTexture colorTex;

		ofShortPixels irPix;
		ofTexture irTex;

		k4a::image depthToWorldImg;
		ofFloatPixels depthToWorldPix;
		ofTexture depthToWorldTex;

		k4a::image colorToWorldImg;
		ofFloatPixels colorToWorldPix;
		ofTexture colorToWorldTex;

		ofShortPixels depthInColorPix;
		ofTexture depthInColorTex;

		ofPixels colorInDepthPix;
		ofTexture colorInDepthTex;

		std::vector<glm::vec3> positionCache;
		std::vector<glm::vec2> uvCache;
		size_t numPoints;
		ofVbo pointCloudVbo;

		ofEventListeners eventListeners;
	};
}