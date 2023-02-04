#pragma once

#include "../Headers/iPhysicsWorld.h"
#include <vector>
#include <string>

// Bullet Physics
#include <btBulletDynamicsCommon.h>
#include <btBulletCollisionCommon.h>
#include <BulletSoftBody/btSoftRigidDynamicsWorld.h>
#include <BulletSoftBody/btDefaultSoftBodySolver.h>




namespace gdpPhysics
{

	class PhysicsWorld : public iPhysicsWorld
	{
	public:
		PhysicsWorld();
		virtual ~PhysicsWorld();

		virtual void TimeStep(float dt);

		virtual void SetGravity(const glm::vec3& gravity);
		virtual void RegisterCollisionListener(iCollisionListener* listener);
		virtual void RegisterDebugDrawer(iPhysicsWorldDrawDebug* debugDrawer);

		virtual void AddBody(iCollisionBody* body);
		virtual void RemoveBody(iCollisionBody* body);
		virtual void AddConstraint(iConstraint* con);
		virtual void* GetDynamicsWorld();

		virtual const std::string& GetVersion();

	private:
		btBroadphaseInterface* mBroadphase;
		btDefaultCollisionConfiguration* mCollisionConfiguration;
		btCollisionDispatcher* mDispatcher;
		btSequentialImpulseConstraintSolver* mSolver;
		btIDebugDraw* mDebugDrawer;

		btSoftBodySolver* mSoftBodySolver;

#ifdef USE_SOFT_BODY_WORLD
		btSoftRigidDynamicsWorld* mSoftRigidWorld;
#else
		btDiscreteDynamicsWorld* mDynamicsWorld;
#endif

		std::vector<iCollisionBody*> mBodies;

		std::string mVersion;

		PhysicsWorld(const PhysicsWorld&) {}
		PhysicsWorld& operator=(const PhysicsWorld&) { return *this; }
	};
}