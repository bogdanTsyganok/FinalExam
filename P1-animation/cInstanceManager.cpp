#include "globalThings.h"
#include "cInstanceManager.h"

cInstanceManager::cInstanceManager()
{
}

void cInstanceManager::AddMesh(cMesh* mesh, std::string meshKey, int instanceCount)
{
    sInstancedMesh instancedMesh;
    instancedMesh.pMesh = mesh;
    if (instanceCount > 100)
        instanceCount = 100;
    mesh->mNumberOfInstances = instanceCount;
    mesh->bInstanced = true;
    instancedMesh.position.resize(instanceCount);
    instancedMesh.rotation.resize(instanceCount);
    instancedMesh.scale.resize(instanceCount);
    instancedMesh.pMesh->v_InstancedMatricies.resize(instanceCount);
    for (int i = 0; i < instanceCount; i++)
    {
        instancedMesh.position[i] = instancedMesh.scale[i] = instancedMesh.rotation[i] = glm::vec3(0.f);
        instancedMesh.pMesh->v_InstancedMatricies[i] = CalculateMatrix(instancedMesh.position[i], instancedMesh.scale[i], instancedMesh.rotation[i]);
    }
    this->instanceMeshes[meshKey] = instancedMesh;
}

cMesh* cInstanceManager::GetMesh(std::string meshKey)
{
    sInstancedMesh instanceMesh = this->instanceMeshes[meshKey];
    /*for (int i = 0; i < instanceMesh.pMesh->mNumberOfInstances; i++)
    {
        instanceMesh.pMesh->v_InstancedMatricies[i] = CalculateMatrix(instanceMesh.position[i], instanceMesh.scale[i], instanceMesh.rotation[i]);
    }*/
    return instanceMesh.pMesh;
}

cInstanceManager::~cInstanceManager()
{
    for (auto pair : instanceMeshes)
        delete pair.second.pMesh;
}

void cInstanceManager::SetPosition(std::string meshKey, unsigned int instanceIndex, glm::vec3 pos)
{
    sInstancedMesh* instanceMesh = &(this->instanceMeshes[meshKey]);
    instanceMesh->position[instanceIndex] = pos;
    instanceMesh->pMesh->v_InstancedMatricies[instanceIndex] = CalculateMatrix(instanceMesh->position[instanceIndex], instanceMesh->scale[instanceIndex], instanceMesh->rotation[instanceIndex]);
}

void cInstanceManager::SetRotation(std::string meshKey, unsigned int instanceIndex, glm::vec3 rotation)
{
    sInstancedMesh* instanceMesh = &(this->instanceMeshes[meshKey]);
    instanceMesh->rotation[instanceIndex] = rotation;
    instanceMesh->pMesh->v_InstancedMatricies[instanceIndex] = CalculateMatrix(instanceMesh->position[instanceIndex], instanceMesh->scale[instanceIndex], instanceMesh->rotation[instanceIndex]);
}

void cInstanceManager::SetScale(std::string meshKey, unsigned int instanceIndex, glm::vec3 scale)
{
    sInstancedMesh* instanceMesh = &(this->instanceMeshes[meshKey]);
    instanceMesh->scale[instanceIndex] = scale;
    instanceMesh->pMesh->v_InstancedMatricies[instanceIndex] = CalculateMatrix(instanceMesh->position[instanceIndex], instanceMesh->scale[instanceIndex], instanceMesh->rotation[instanceIndex]);
}
