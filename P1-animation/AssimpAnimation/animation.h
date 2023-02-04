#pragma once

#include <vector>
#include <map>
#include <glm/glm.hpp>
#include <assimp/scene.h>
#include "bone.h"
#include <functional>
#include "boneInfo.h"
#include "cAssimpModel.h"

struct AssimpNodeData
{
	glm::mat4 transformation;
	std::string name;
	int childrenCount;
	std::vector<AssimpNodeData> children;
};

class asAnimation
{
public:
	asAnimation() = default;

	asAnimation(const std::string& animationPath, asModel* model, int animationIndex = 0);

	asAnimation(const asAnimation& animation);

	~asAnimation()
	{
	}

	asBone* FindBone(const std::string& name);


	inline float GetTicksPerSecond() { return m_TicksPerSecond; }
	inline float GetDuration() { return m_Duration; }
	inline const AssimpNodeData& GetRootNode() { return m_RootNode; }
	inline const std::map<std::string, BoneInfo>& GetBoneIDMap()
	{
		return m_BoneInfoMap;
	}

	std::vector<asBone>& GetBones();

private:
	void ReadMissingBones(const aiAnimation* animation, asModel& model);

	void ReadHeirarchyData(AssimpNodeData& dest, const aiNode* src);
	float m_Duration;
	int m_TicksPerSecond;
	std::vector<asBone> m_Bones;
	AssimpNodeData m_RootNode;
	std::map<std::string, BoneInfo> m_BoneInfoMap;
};
