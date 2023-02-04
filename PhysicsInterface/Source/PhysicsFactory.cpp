#include "PhysicsFactory.h"

#include <cassert>

#include "Conversion.h"
#include "PhysicsWorld.h"
#include "RigidBody.h"
#include "SoftBody.h"
#include "Constraint.h"

// BULLET

namespace gdpPhysics
{
	PhysicsFactory::PhysicsFactory()
		: iPhysicsFactory()
		, mVersion("Bullet")
	{
		printf("Bullet Physics Factory\n");
	}

	PhysicsFactory::~PhysicsFactory()
	{
	}

	iPhysicsWorld* PhysicsFactory::CreateWorld()
	{
		return new PhysicsWorld();
	}

	iRigidBody* PhysicsFactory::CreateRigidBody(const cRigidBodyDesc& desc, iShape* shape)
	{
		return new RigidBody(desc, shape);
	}

	iSoftBody* PhysicsFactory::CreateSoftBody(const cSoftBodyDesc& desc)
	{
		return 0;// new SoftBody(desc);
	}
	iConstraint* PhysicsFactory::CreateConstraint(const sConstraintDesc& desc)
	{
		iConstraint* outConstr = nullptr;
		if (desc.type == HingeConstraint)
		{
			btVector3 pivot = btVector3(0, 0, 0), axis = btVector3(0, 0, 0);
			CastBulletVector3(desc.vecArray[0], &pivot);
			CastBulletVector3(desc.vecArray[1], &axis);
			btHingeConstraint* hinge = new btHingeConstraint(*CastBulletRigidBody(desc.bodyArray[0]), pivot, axis, true);
			outConstr = new Constraint(hinge);
		}else if(desc.type == RodConstraint)
		{
			btVector3 pivotA = btVector3(0, 0, 0), pivotB = btVector3(0, 0, 0);
			CastBulletVector3(desc.vecArray[0], &pivotA);
			CastBulletVector3(desc.vecArray[1], &pivotB);
			btPoint2PointConstraint* rod = new btPoint2PointConstraint(*CastBulletRigidBody(desc.bodyArray[0]), *CastBulletRigidBody(desc.bodyArray[1]), pivotA, pivotB);
			outConstr = new Constraint(rod);
		}
		return outConstr;
	}
}
