#include "cPhysicsManager.h"

std::vector<sMeshRbConstruct*> cPhysicsManager::getConstructs()
{
	return constructs;
}

void cPhysicsManager::addConstruct(sMeshRbConstruct* con)
{
	constructs.push_back(con);
}

void cPhysicsManager::addConstruct(sAssimpRbConstruct* con)
{
	asConstructs.push_back(con);
}

void cPhysicsManager::updateConstructs()
{
	for (sMeshRbConstruct* construct : constructs)
	{
		construct->updateMeshPosition();
	}

	for (sAssimpRbConstruct* construct : asConstructs)
	{
		construct->updateModelPosition();
	}
}

void sMeshRbConstruct::updateMeshPosition()
{
	_rb->GetPosition(_mesh->positionXYZ);
	glm::quat rbOrientation;
	_rb->GetOrientation(rbOrientation);
	_mesh->orientationXYZ = glm::eulerAngles(rbOrientation);
}

void sAssimpRbConstruct::updateModelPosition()
{
	glm::vec3 pos;
	_rb->GetPosition(pos);
	pos.y -= 15.f;
	_model->mPosition = pos;
	glm::quat rbOrientation;
	_rb->GetOrientation(rbOrientation);
	_model->mOrientation = glm::eulerAngles(rbOrientation);
}
