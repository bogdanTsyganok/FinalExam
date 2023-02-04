#pragma once

#include "../PhysicsInterface/Source/PhysicsFactory.h";
#include "../PhysicsInterface/Source/PhysicsWorld.h";
#include "../PhysicsInterface/Source/RigidBody.h";
#include "../PhysicsInterface/Headers/shapes.h";
#include "cMesh.h"
#include "AssimpAnimation/cAssimpModel.h"

struct sMeshRbConstruct
{
	cMesh* _mesh;
	gdpPhysics::iRigidBody* _rb;

	void updateMeshPosition();
};

struct sAssimpRbConstruct
{
	asModel* _model;
	gdpPhysics::iRigidBody* _rb;

	void updateModelPosition();
};

class cPhysicsManager
{
	std::vector<sMeshRbConstruct*> constructs;
	std::vector<sAssimpRbConstruct*> asConstructs;

public:

	cPhysicsManager()
	{
	}
	~cPhysicsManager()
	{
		asConstructs.clear();
		constructs.clear();
	}
	std::vector<sMeshRbConstruct*> getConstructs();

	void addConstruct(sMeshRbConstruct* con);

	void addConstruct(sAssimpRbConstruct* con);

	void updateConstructs();
};