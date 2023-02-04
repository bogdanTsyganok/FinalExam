#pragma once
#include "animator.h"

//class AnimationController;

class AnimatedState
{
public:
	asModelAnimator* animation;
	
	//returns true if animation is complete
	virtual bool updateAnimation(float deltaTime) = 0;
	virtual bool acceptString(std::string string) = 0;
	virtual void setNextState(AnimatedState* next)
	{
		m_nextState = next;
	}
	virtual AnimatedState* getNextState()
	{
		return m_nextState;
	}

	std::vector<glm::mat4> GetFinalBoneMatricies()
	{
		return animation->GetFinalBoneMatrices();
	}
private:
	//AnimationController* m_parent;
	AnimatedState* m_nextState = nullptr;
};