#pragma once

#include "cSoftBodyDesc.h"
#include "iSoftBody.h"


#include "cRigidBodyDesc.h"
#include "iRigidBody.h"

#include "iShape.h"
#include "iPhysicsWorld.h"

namespace gdpPhysics
{
	enum eConstraintType {RodConstraint, HingeConstraint};

	struct sConstraintDesc
	{
		eConstraintType type;
		std::vector<iRigidBody*> bodyArray;
		std::vector<glm::vec3> vecArray;
	};

	class iPhysicsFactory
	{
	public:
		virtual ~iPhysicsFactory() {}

		virtual iPhysicsWorld* CreateWorld() = 0;

		virtual iRigidBody* CreateRigidBody(const cRigidBodyDesc& desc, iShape* shape) = 0;
		virtual iSoftBody* CreateSoftBody(const cSoftBodyDesc& desc) = 0;
		virtual iConstraint* CreateConstraint(const sConstraintDesc& dexc) = 0;

	protected:
		iPhysicsFactory() {}

	private:
		iPhysicsFactory(const iPhysicsFactory&) {}
		iPhysicsFactory& operator=(const iPhysicsFactory&) { return *this; }
	};
}