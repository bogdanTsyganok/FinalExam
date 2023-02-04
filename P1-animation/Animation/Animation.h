#pragma once

#include <vector>

#include "KeyFrame.h"

struct Animation 
{
	bool playing;
	float duration;
	float currentTime;
	float speed;
	bool repeat;

	std::vector<KeyFramePosition> keyFramePositions;
	std::vector<KeyFrameScale> keyFrameScales;
	std::vector<KeyFrameRotation> keyFrameRotations;

	std::vector<KeyFrameBonePosition> keyFrameBonePosition;
	std::vector<KeyFrameBoneRotation> keyFrameBoneRotations;
	std::vector<KeyFrameBoneScale> keyFrameBoneScale;
};