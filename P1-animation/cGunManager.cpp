#include "cGunManager.h"

cGunManager::cGunManager(cMesh* gunMesh, cInstanceManager* instanceManager, int maxBulletHoles, cEnemyManager* enemyManager)
{
    p_gunMesh = gunMesh;
    p_instanceManager = instanceManager;
    this->maxBulletHoles = maxBulletHoles;
    pEnemyManager = enemyManager;
}

void cGunManager::Fire()
{
    glm::vec3 from = ::g_pFlyCamera->eye;
    glm::vec3 at = g_pFlyCamera->getAtInWorldSpace();
    glm::vec3 to = from + ((at - ::g_pFlyCamera->eye) * 1000.f);
    btVector3 btFrom(from.x, from.y, from.z);
    btVector3 btTo(to.x, to.y, to.z);
    glm::vec3 offset = glm::normalize(glm::cross(g_pFlyCamera->getAt(), g_pFlyCamera->getUp())) * 3.f + glm::normalize(g_pFlyCamera->getAt()) * 20.f - glm::normalize(g_pFlyCamera->getUp()) * 2.f;
    btVector3 btOffset(offset.x, offset.y, offset.z);
    btCollisionWorld::ClosestRayResultCallback callback(btFrom, btTo);
    btDiscreteDynamicsWorld* worldPointer = (btDiscreteDynamicsWorld*)::physicsWorld->GetDynamicsWorld();
    worldPointer->rayTest(btFrom, btTo, callback);
    worldPointer->getDebugDrawer()->drawLine(btFrom + btOffset, btTo, btVector3(1.f, 0.f, 0.f));//+ btOffset
    if (callback.hasHit())
    {
        btVector3 p = btFrom.lerp(btTo, callback.m_closestHitFraction);
        glm::vec3 worldNormal(callback.m_hitNormalWorld.x(), callback.m_hitNormalWorld.y(), callback.m_hitNormalWorld.z());
        glm::vec3 rotation = DirectionToRotation(worldNormal);
        p_instanceManager->SetPosition("bulletHoles", bulletHoleCounter, glm::vec3(p.x(), p.y(), p.z()) + worldNormal * 0.01f);
        p_instanceManager->SetRotation("bulletHoles", bulletHoleCounter, rotation);
        btRigidBody* pointer = ((btRigidBody*)callback.m_collisionObject);
        pointer->setMassProps(0, btVector3(0.f, 0.f, 0.f));
        if ((++bulletHoleCounter) >= maxBulletHoles)
        {
            bulletHoleCounter = 0;
        }

        asModel* model = static_cast<asModel*>((pointer->getUserPointer()));
        if (model != NULL)
        {
            //SetAnimation
            sMessage msg;
            msg.cmd = "die";
            msg.pointerData = model;
            pEnemyManager->AcceptCommand(msg);
            //p_animationController->acceptCommand("stopIdlying");
        }
    }
    //Gun sway?
}

void cGunManager::Update(float deltaTime)
{
}
