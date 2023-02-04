#include "blendState.h"

BlendState::BlendState(float time, asModelAnimator* previousAnimator)
{
	m_timer = time;
	p_previousAnimator = previousAnimator;
}

bool BlendState::updateAnimation(float deltaTime)
{
	if (!isTriggered)
	{
		asModelAnimator* blendAnimation = p_previousAnimator->CreateBlend(this->m_timer, animation->getCurrentAnimation());
		delete this->animation;
		this->animation = blendAnimation;
	}
	isTriggered = true;
	animation->UpdateAnimation(deltaTime);
	m_timer -= deltaTime;
	if (m_timer < 0.05)
	{
		//hardcode bad
		m_timer = 0.5f;
		//isTriggered = false;
		return true;
	}
	return false;
}

bool BlendState::acceptString(std::string command)
{
	return false;
}
