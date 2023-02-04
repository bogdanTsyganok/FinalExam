#include "animation.h"

asAnimation::asAnimation(const std::string& animationPath, asModel* model, int animationIndex)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(animationPath, aiProcess_Triangulate);
	assert(scene && scene->mRootNode);
	auto animation = scene->mAnimations[animationIndex];
	m_Duration = animation->mDuration;
	m_TicksPerSecond = animation->mTicksPerSecond;
	aiMatrix4x4 globalTransformation = scene->mRootNode->mTransformation;
	globalTransformation = globalTransformation.Inverse();
	ReadHeirarchyData(m_RootNode, scene->mRootNode);
	ReadMissingBones(animation, *model);
}

asAnimation::asAnimation(const asAnimation& animation)
{
	this->m_BoneInfoMap = animation.m_BoneInfoMap;
	this->m_Bones = animation.m_Bones;
	this->m_Duration = animation.m_Duration;
	this->m_TicksPerSecond = animation.m_TicksPerSecond;
	this->m_RootNode = animation.m_RootNode;
}

asBone* asAnimation::FindBone(const std::string& name)
{
	auto iter = std::find_if(m_Bones.begin(), m_Bones.end(),
		[&](const asBone& Bone)
		{
			return Bone.GetBoneName() == name;
		}
	);
	if (iter == m_Bones.end()) return nullptr;
	else return &(*iter);
}

std::vector<asBone>& asAnimation::GetBones()
{
	// TODO: insert return statement here
	return m_Bones;
}

void asAnimation::ReadMissingBones(const aiAnimation* animation, asModel& model)
{
	int size = animation->mNumChannels;

	auto& boneInfoMap = model.GetBoneInfoMap();//getting m_BoneInfoMap from Model class
	int& boneCount = model.GetBoneCount(); //getting the m_BoneCounter from Model class

	//reading channels(bones engaged in an animation and their keyframes)
	for (int i = 0; i < size; i++)
	{
		auto channel = animation->mChannels[i];
		std::string boneName = channel->mNodeName.data;

		if (boneInfoMap.find(boneName) == boneInfoMap.end())
		{
			boneInfoMap[boneName].id = boneCount;
			boneCount++;
		}
		m_Bones.push_back(asBone(channel->mNodeName.data,
			boneInfoMap[channel->mNodeName.data].id, channel));
	}

	m_BoneInfoMap = boneInfoMap;
}

void asAnimation::ReadHeirarchyData(AssimpNodeData& dest, const aiNode* src)
{
	assert(src);

	dest.name = src->mName.data;
	dest.transformation = ConvertMatrixToGLMFormat(src->mTransformation);
	dest.childrenCount = src->mNumChildren;

	for (int i = 0; i < src->mNumChildren; i++)
	{
		AssimpNodeData newData;
		ReadHeirarchyData(newData, src->mChildren[i]);
		dest.children.push_back(newData);
	}
}