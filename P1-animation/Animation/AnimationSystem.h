#pragma once


#include "KeyFrame.h"
#include "Animation.h"
#include "../cMesh.h"

struct AnimetedMesh
{
	cMesh* pMesh;
	Animation* pAnim;
};

class AnimationSystem
{
public:
	virtual void Process(const std::vector<AnimetedMesh>& entities, float dt);
private:
	//const KeyFramePosition& FindCurrentKeyFramePosition(Animation* animation, float time);
	//const KeyFrameScale& FindCurrentKeyFrameScale(Animation* animation, float time);
	int FindKeyFramePositionIndex(Animation* animation, float time);
	int FindKeyFrameScaleIndex(Animation* animation, float time);
	int FindKeyFrameRotIndex(Animation* animation, float time);
};