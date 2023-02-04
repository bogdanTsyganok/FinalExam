#pragma once
#include "animator.h"
#include "concreteStates.h"

struct sAnimationControllerInfo
{
	enum eStateType {blend, command };
	eStateType type;
	std::string animationName;
	float length;
	std::vector<void*> voidPointerArray;
	std::vector<std::string> stringArray;
	glm::vec4 floatData;
};

class AnimationController
{
public:
	AnimatedState* createState(sAnimationControllerInfo creationInfo);
	void acceptCommand(std::string command);
	void update(float dt, GLuint program);
	void loadAnimation(std::string fileName, std::string animationName, asModel* model, int animationIndex);
	void addState(int id, AnimatedState* state);
	void loadMatricies(int id, GLuint program);

	~AnimationController()
	{
		//delete m_currentState;
	}
private:
	//std::map<std::string, AnimatedState*> m_controlledObjects;
	std::map<int, AnimatedState*> m_currentStates;
	std::map<std::string, asAnimation*> m_animations;
	
};