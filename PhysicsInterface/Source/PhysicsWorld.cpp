#include "PhysicsWorld.h"

#include "Conversion.h"

// BULLET

namespace gdpPhysics
{
	PhysicsWorld::PhysicsWorld()
	{
		btVector3 worldMin(-1000, -1000, -1000);
		btVector3 worldMax(1000, 1000, 1000);
		mBroadphase = new btAxisSweep3(worldMin, worldMax);

		mCollisionConfiguration = new btDefaultCollisionConfiguration();
		mDispatcher = new btCollisionDispatcher(mCollisionConfiguration);
		mSolver = new btSequentialImpulseConstraintSolver();

#ifdef USE_SOFT_BODY_WORLD
		mSoftBodySolver = new btDefaultSoftBodySolver();
		mSoftRigidWorld = new btSoftRigidDynamicsWorld(mDispatcher, mBroadphase, mSolver, mCollisionConfiguration, mSoftBodySolver);
		mVehicleRaycaster = new VehicleRaycaster(new btDefaultVehicleRaycaster(mSoftRigidWorld));
#else
		mDynamicsWorld = new btDiscreteDynamicsWorld(mDispatcher, mBroadphase, mSolver, mCollisionConfiguration);
#endif

		mVersion = "Bullet";
		printf("Bullet Physics World\n");

	}

	PhysicsWorld::~PhysicsWorld()
	{
		// TODO:
		// Remove rigid bodies from the dynamics world
		// remove soft bodies from the soft rigid world
		// delete soft bodies
		// delete motion states
		// delete rigid bodies
		// Delete shapes.

		mDynamicsWorld->clearForces();

#ifdef USE_SOFT_BODY_WORLD
		delete mSoftRigidWorld;
		delete mSoftBodySolver;
#else
		delete mDynamicsWorld;
#endif
		delete mSolver;
		delete mCollisionConfiguration;
		delete mDispatcher;
		delete mBroadphase;
 		delete mDebugDrawer;
	}

	void PhysicsWorld::SetGravity(const glm::vec3& gravity)
	{
		btVector3 btGravity;
		CastBulletVector3(gravity, &btGravity);
#ifdef USE_SOFT_BODY_WORLD
		mSoftRigidWorld->setGravity(btGravity);
#else
		mDynamicsWorld->setGravity(btGravity);
#endif
	}

	void PhysicsWorld::RegisterCollisionListener(iCollisionListener* listener)
	{
		// TODO:
	}
	void PhysicsWorld::RegisterDebugDrawer(iPhysicsWorldDrawDebug* debugDrawer)
	{
		mDynamicsWorld->setDebugDrawer(debugDrawer);
		this->mDebugDrawer = debugDrawer;
	}
	void PhysicsWorld::AddBody(iCollisionBody* body)
	{
		if (std::find(mBodies.begin(), mBodies.end(), body) != mBodies.end())
			return;

		if (body->GetBodyType() == gdpPhysics::eBodyType::SOFT_BODY)
		{
#ifdef USE_SOFT_BODY_WORLD
			printf("PhysicsWorld.AddBody: [Warning] Unimplemented adding soft body to mSoftRigidWorld!\n");
			mSoftRigidWorld->addSoftBody(CastBulletSoftBody(body));
#else
			printf("PhysicsWorld.AddBody: [Error] Unable to add a soft body to a dynamics world!\n");
			return;
#endif
		}

		// TODO:
		// May want to add 2 more vectors
		// vector<SoftBody*> mSoftBodies;
		// vector<RigidBody*> mRigidBodies;
		mBodies.push_back(body);

#ifdef USE_SOFT_BODY_WORLD
		mSoftRigidWorld->addRigidBody(CastBulletRigidBody(body));
#else
		mDynamicsWorld->addRigidBody(CastBulletRigidBody(body));
#endif
	}

	void PhysicsWorld::RemoveBody(iCollisionBody* body)
	{
		// TODO:
	}

	void PhysicsWorld::AddConstraint(iConstraint* con)
	{
		mDynamicsWorld->addConstraint(CastBulletConstraint(con));
	}

	void PhysicsWorld::TimeStep(float dt)
	{

#ifdef USE_SOFT_BODY_WORLD
		mSoftRigidWorld->stepSimulation(dt, 10);
#else
		mDynamicsWorld->stepSimulation(dt, 10);
		if (this->mDebugDrawer != NULL)
		{
			mDynamicsWorld->debugDrawWorld();
		}
#endif

	}

	const std::string& PhysicsWorld::GetVersion()
	{
		return mVersion;
	}

	void* PhysicsWorld::GetDynamicsWorld()
	{
		return this->mDynamicsWorld;
	}
}