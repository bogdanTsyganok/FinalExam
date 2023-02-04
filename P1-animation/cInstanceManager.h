#pragma once
#include "GLCommon.h"
#include <vector>
#include <map>
#include "cMesh.h"

struct sInstancedMesh
{
	std::vector<glm::vec3> position;
	std::vector<glm::vec3> rotation;
	std::vector<glm::vec3> scale;
	cMesh* pMesh;
};

class cInstanceManager
{
public:
	std::map<std::string, sInstancedMesh> instanceMeshes;
	
	cInstanceManager();

	void AddMesh(cMesh* mesh, std::string meshKey, int instanceCount);

	void SetPosition(std::string meshKey, unsigned int instanceIndex, glm::vec3 pos);

	void SetRotation(std::string meshKey, unsigned int instanceIndex, glm::vec3 rotation);

	void SetScale(std::string meshKey, unsigned int instanceIndex, glm::vec3 scale);

	cMesh* GetMesh(std::string meshKey);

	//next: create alt way to add/manage mesh
	//create destructor
	~cInstanceManager();
};