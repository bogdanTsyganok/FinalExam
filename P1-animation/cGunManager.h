#pragma once

#include "cMesh.h"
#include "AssimpAnimation/cAssimpModel.h"
#include "cInstanceManager.h"
#include "globalThings.h"
#include "AssimpAnimation/animationController.h"
#include "cEnemyManager.h"

class cGunManager
{
	cMesh* p_gunMesh;

	cInstanceManager* p_instanceManager;

	cEnemyManager* pEnemyManager;

	float animationTimer = 0.f;

	int bulletHoleCounter = 0;

	int maxBulletHoles = 0;
public:
	cGunManager() = delete;
	
	cGunManager(cMesh* gunMesh, cInstanceManager* instanceManager, int maxBulletHoles, cEnemyManager* enemyManager);

	void Fire();

	void Update(float deltaTime);
};