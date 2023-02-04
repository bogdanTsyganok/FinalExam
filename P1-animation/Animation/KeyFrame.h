#pragma once

#include <glm/vec3.hpp>
#include <glm/gtx/quaternion.hpp>

enum EasingType
{
	EaseIn,
	EaseOut,
	EaseInOut,
	None
};

#define DIRECT_ROTATION 0
#define LERP_ROTATION 1
#define SLERP_ROTATION 2

struct KeyFramePosition 
{
	KeyFramePosition(float time, const glm::vec3& position, EasingType easingType = EasingType::None)
		: time(time), position(position), easingType(easingType) {}
	float time;
	glm::vec3 position;
	EasingType easingType;
};

struct KeyFrameScale 
{
	KeyFrameScale(float time, const glm::vec3& scale, EasingType easingType = EasingType::None)
		: time(time), scale(scale), easingType(easingType) {}
	float time;
	glm::vec3 scale;
	EasingType easingType;
};

struct KeyFrameRotation 
{
	KeyFrameRotation(float time, const glm::quat& rotation, int interpolationType = 0, EasingType easingType = EasingType::None)
		: time(time), rotation(rotation), InterpolationType(interpolationType), easingType(easingType) {}
	float time;
	glm::quat rotation;
	EasingType easingType;
	int InterpolationType;
};

struct KeyFrameBonePosition
{
	KeyFrameBonePosition(float time, EasingType easingType = EasingType::None)
		: time(time), easingType(easingType)
	{
	}
	float time;
	glm::vec3 positions[4];
	EasingType easingType;
};

struct KeyFrameBoneRotation
{
	KeyFrameBoneRotation(float time, int interpolationType = 0, EasingType easingType = EasingType::None)
		: time(time), InterpolationType(interpolationType), easingType(easingType)
	{
	}
	float time;
	glm::quat rotations[4];
	EasingType easingType;
	int InterpolationType;
};

struct KeyFrameBoneScale
{
	KeyFrameBoneScale(float time, EasingType easingType = EasingType::None)
		: time(time), easingType(easingType)
	{
	}
	float time;
	glm::vec3 scales[4];
	EasingType easingType;
	int boneId;
};