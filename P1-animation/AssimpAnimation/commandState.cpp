#include "commandState.h"

CommandState::CommandState(std::string exitCommand)
{
	m_exitCmd = exitCommand;
}

bool CommandState::updateAnimation(float deltaTime)
{	
	return animation->UpdateAnimation(deltaTime, this->shouldLoop);
}

bool CommandState::acceptString(std::string string)
{
	if (string == m_exitCmd)
	{
		return true;
	}
	return false;
}
