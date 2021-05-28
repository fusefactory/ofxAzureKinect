#pragma once

#include <k4a/k4atypes.h>
#include "ofVectorMath.h"

inline const glm::vec2 & toGlm(const k4a_float2_t & v) 
{
	return *reinterpret_cast<const glm::vec2*>(&v);
}

inline const glm::vec3 & toGlm(const k4a_float3_t & v)
{
	return *reinterpret_cast<const glm::vec3*>(&v);
}

namespace ofxAzureKinect
{
	typedef k4a_depth_mode_t DepthMode;
	typedef k4a_color_resolution_t ColorResolution;
	typedef k4a_image_format_t ImageFormat;
	typedef k4a_fps_t FramesPerSecond;
}
