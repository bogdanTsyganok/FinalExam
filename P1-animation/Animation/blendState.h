#pragma once

#include "animatedState.h"

class BlendState : public AnimatedState
{
public:
	BlendState(float time, asModelAnimator* previousAnimator);
	virtual bool updateAnimation(float deltaTime);
	virtual bool acceptString(std::string command);
private:
	float m_timer;
	asModelAnimator* p_previousAnimator;
	bool isTriggered = false;
};