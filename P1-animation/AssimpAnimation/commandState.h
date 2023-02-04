#pragma once

#include "animatedState.h"

class CommandState : public AnimatedState
{
public:
	CommandState(std::string exitCommand);
	virtual bool updateAnimation(float deltaTime);
	virtual bool acceptString(std::string string);
private:
	std::string m_exitCmd;
};