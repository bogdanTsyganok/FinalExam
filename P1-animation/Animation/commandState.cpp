#include "commandState.h"

CommandState::CommandState(std::string exitCommand)
{
	m_exitCmd = exitCommand;
}

bool CommandState::updateAnimation(float deltaTime)
{
	animation->UpdateAnimation(deltaTime);
	return false;
}

bool CommandState::acceptString(std::string string)
{
	if (string == m_exitCmd)
	{
		return true;
	}
	return false;
}
