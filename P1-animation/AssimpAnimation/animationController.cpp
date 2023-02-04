#include "animationController.h"

AnimatedState* AnimationController::createState(sAnimationControllerInfo creationInfo)
{
	AnimatedState* state = nullptr;
	asModelAnimator* animator = nullptr;
	switch (creationInfo.type)
	{
	case(sAnimationControllerInfo::eStateType::command):		
		state = new CommandState(creationInfo.stringArray[0]);
		animator = new asModelAnimator(m_animations[creationInfo.animationName]);
		state->animation = animator;
		if (creationInfo.floatData[0] == 0.f)
			state->shouldLoop = false;
		break;		

	case(sAnimationControllerInfo::eStateType::blend):		
		state = new BlendState(creationInfo.length, (asModelAnimator*)(creationInfo.voidPointerArray[0]));
		state->animation = new asModelAnimator(m_animations[creationInfo.animationName]);
		break;		
	}
	return state;
}

void AnimationController::acceptCommand(std::string command)
{
	for (std::map<int, AnimatedState*>::iterator it = m_currentStates.begin(); it != m_currentStates.end(); it++)
	{
		if (it->second->acceptString(command))
		{
			AnimatedState* next = it->second->getNextState();
			//delete it->second;
			it->second = next;
			it->second->animation->Reset();
		}
	}
}

void AnimationController::update(float dt, GLuint program)
{	
	for (std::map<int, AnimatedState*>::iterator it = m_currentStates.begin(); it != m_currentStates.end(); it++)
	{
		if (it->second == nullptr)
			continue;

		if (it->second->updateAnimation(dt))
		{
			if (it->second->shouldLoop)
				continue;
			it->second->done = true;
			
			AnimatedState* next = it->second->getNextState();
			if (next == NULL)
				continue;
			//delete it->second;
			it->second = next;			
			it->second->animation->Reset();
		}
		
	}
}

void AnimationController::loadAnimation(std::string fileName, std::string animationName, asModel* model, int animationIndex)
{
	asAnimation* animation = new asAnimation(fileName, model, animationIndex);
	m_animations.insert(std::make_pair(animationName, animation));
}

void AnimationController::addState(int id, AnimatedState* state)
{
	if (m_currentStates.find(id) == m_currentStates.end())
	{
		m_currentStates.insert(std::make_pair(id, state));
	}
	else
	{
		delete m_currentStates[id];
		m_currentStates[id] = state;
	}
}

void AnimationController::loadMatricies(int id, GLuint program)
{
	std::vector<glm::mat4> boneMatricies = m_currentStates[id]->animation->GetFinalBoneMatrices();
	for (int boneIndex = 0; boneIndex < m_currentStates[id]->animation->GetFinalBoneMatrices().size(); boneIndex++)
	{
		std::string boneLocationName = "boneMatrices[" + std::to_string(boneIndex) + "]";
		glUniformMatrix4fv(glGetUniformLocation(program, boneLocationName.c_str()), 1, GL_FALSE, &boneMatricies[boneIndex][0][0]);
	}
}
