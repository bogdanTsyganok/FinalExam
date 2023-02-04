#pragma once

#include <vector>
#include "AssimpAnimation/animationController.h"
#include "AssimpAnimation/cAssimpModel.h"
#include "cPhysicsManager.h"


struct sEnemy
{
	sAssimpRbConstruct* construct;
	std::string dieCmd;
	std::string attackCmd;
	std::string state;
};

struct sMessage
{
	std::string cmd;
	std::vector<std::string> stringData;
	glm::vec4 floatData;
	void* pointerData;
};

class cEnemyManager
{
	std::vector<sEnemy> v_enemies;
	AnimationController* pAnimationController;
	float enemySpeed;
public:

	cEnemyManager(AnimationController* AnimationController);

	void Update(float dt, glm::vec3 playerPos);

	void AddEnemy(sEnemy enemy);

	void AcceptCommand(sMessage cmd);
};