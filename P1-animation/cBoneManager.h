#pragma once

#include <vector>
#include <glm/vec3.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/mat4x4.hpp>
#include "cBone.h"

//class Animation;
//class BoneHierarchy;
//class Bone;

class BoneSystem 
{
public:
	virtual void Process(const std::vector<BoneHierarchy*>& entities, float dt);
private:
	void UpdateHierarchy(BoneHierarchy* boneHierarchy);
	void UpdateBone(BoneHierarchy* boneHierarchy, cBone* bone,
		const glm::mat4 parentModelMatrix);

	//int FindKeyFramePositionIndex(Animation* animation, float time);
	//int FindKeyFrameScaleIndex(Animation* animation, float time);
	//int FindKeyFrameRotIndex(Animation* animation, float time);
};

void BoneSystem::Process(const std::vector<BoneHierarchy*>& entities, float dt)
{
	BoneHierarchy* boneHierarchy;

	for (int i = 0; i < entities.size(); ++i)
	{
		boneHierarchy = entities[i];

		if (boneHierarchy == 0)
			continue;

		boneHierarchy->time += dt;
		UpdateHierarchy(boneHierarchy);
	}
}

void BoneSystem::UpdateHierarchy(BoneHierarchy* boneHierarchy /*, Animation* animation */)
{
	// 0 -> 90 over 5 seconds
	// quat(1, 0, 0, 0) -> quat(0.7071, 0.0f, 0.0f, 0.7071) over 5 seconds


	glm::quat start = glm::quat(1.f, 0.f, 0.f, 0.f);
	glm::quat end = glm::quat(.07071f, 0.f, 0.f, 0.7071f);
	float rotationFraction = boneHierarchy->time / 5.f;
	glm::quat currentRotation = glm::slerp(start, end, rotationFraction);

	for (int i = 0; i < boneHierarchy->currentTransforms.size(); ++i)
	{
		boneHierarchy->currentTransforms[i].rotation = currentRotation;
	}

	UpdateBone(boneHierarchy, boneHierarchy->rootBone, glm::mat4(1.f));
}

void BoneSystem::UpdateBone(BoneHierarchy* boneHierarchy, cBone* bone,
	const glm::mat4 parentModelMatrix)
{
	/*Transform& transform = boneHierarchy->currentTransforms[bone->index];

	bone->SetPosition(transform.position);
	bone->SetScale(transform.scale);
	bone->SetRotation(transform.rotation);*/

	glm::mat4 TranslationMatrix = glm::translate(parentModelMatrix, bone->GetPosition());
	glm::mat4 RotationMatrix = glm::mat4_cast(bone->GetRotation());
	glm::mat4 ScaleMatrix = glm::scale(glm::mat4(1.0f), bone->GetScale());

	glm::mat4 ModelMatrix = TranslationMatrix * RotationMatrix * ScaleMatrix;

	bone->SetModelMatrix(ModelMatrix);

	std::vector<cBone*>& children = bone->GetChildren();
	for (int i = 0; i < children.size(); ++i)
	{
		UpdateBone(boneHierarchy, children[i], ModelMatrix);
	}
}