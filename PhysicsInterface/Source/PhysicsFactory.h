#pragma once

#include "../Headers/iPhysicsFactory.h"
#include "../Headers/cSoftBodyDesc.h"
#include "../Headers/iSoftBody.h"
#include "../Headers/iPhysicsWorldDrawDebug.h"

#include <btBulletCollisionCommon.h>
#include <btBulletDynamicsCommon.h>
#include <string>

namespace gdpPhysics
{
	class PhysicsFactory : public iPhysicsFactory
	{
	public:
		PhysicsFactory();
		virtual ~PhysicsFactory();

		virtual iPhysicsWorld* CreateWorld();

		virtual iRigidBody* CreateRigidBody(const cRigidBodyDesc& desc, iShape* shape);
		virtual iSoftBody* CreateSoftBody(const cSoftBodyDesc& desc);
		virtual iConstraint* CreateConstraint(const sConstraintDesc& desc);
	private:
		std::string mVersion;
	};
}
