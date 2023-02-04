#include "cEnemyManager.h"



cEnemyManager::cEnemyManager(AnimationController* AnimationController)
{
	pAnimationController = AnimationController;
	enemySpeed = 0.5f;
}

void cEnemyManager::Update(float dt, glm::vec3 playerPos)
{
	for(sEnemy enemy : v_enemies)
	{
		if (enemy.state == "dead")
		{
			continue;
		}
		glm::vec3 curPos;
		enemy.construct->_rb->GetPosition(curPos);
		glm::vec3 direction = glm::normalize(playerPos - curPos);
		direction.y = 0;
		direction = glm::normalize(direction);
		enemy.construct->_rb->SetPosition(curPos + (direction * enemySpeed));

		float angle = glm::acos(glm::dot(glm::vec3(0.f, 0.f, 1.f), direction));
		if (direction.x < 0)
			angle = -angle;
		enemy.construct->_rb->SetOrientation(glm::quat(glm::vec3(0.f, angle, 0.f)));


	}
}

void cEnemyManager::AddEnemy(sEnemy enemy)
{
	v_enemies.push_back(enemy);
}

void cEnemyManager::AcceptCommand(sMessage cmd)
{
	if (cmd.cmd == "die")
	{
		for (sEnemy& enemy : v_enemies)
		{
			if (enemy.construct->_model == (asModel*)(cmd.pointerData))
			{
				pAnimationController->acceptCommand(enemy.dieCmd);
				enemy.state = "dead";
			}
		}
	}
}
