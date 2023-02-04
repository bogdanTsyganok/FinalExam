#include "animator.h"

asModelAnimator::asModelAnimator(asAnimation* animation)
{
	m_CurrentTime = 0.0;
	m_CurrentAnimation = animation;

	m_FinalBoneMatrices.reserve(100);

	for (int i = 0; i < 100; i++)
		m_FinalBoneMatrices.push_back(glm::mat4(1.0f));
}

void asModelAnimator::UpdateAnimation(float dt)
{
	m_DeltaTime = dt;
	if (m_CurrentAnimation)
	{
		m_CurrentTime += m_CurrentAnimation->GetTicksPerSecond() * dt;
		m_CurrentTime = fmod(m_CurrentTime, m_CurrentAnimation->GetDuration());
		CalculateBoneTransform(&m_CurrentAnimation->GetRootNode(), glm::mat4(1.0f));
	}
}

void asModelAnimator::PlayAnimation(asAnimation* pAnimation)
{
	m_CurrentAnimation = pAnimation;
	m_CurrentTime = 0.0f;
}

void asModelAnimator::CalculateBoneTransform(const AssimpNodeData* node, glm::mat4 parentTransform)
{
	std::string nodeName = node->name;
	glm::mat4 nodeTransform = node->transformation;

	asBone* Bone = m_CurrentAnimation->FindBone(nodeName);

	if (Bone)
	{
		Bone->Update(m_CurrentTime);
		nodeTransform = Bone->GetLocalTransform();
	}

	glm::mat4 globalTransformation = parentTransform * nodeTransform;

	std::map<std::string, BoneInfo> boneInfoMap = m_CurrentAnimation->GetBoneIDMap();
	if (boneInfoMap.find(nodeName) != boneInfoMap.end())
	{
		int index = boneInfoMap[nodeName].id;
		glm::mat4 offset = boneInfoMap[nodeName].offset;
		m_FinalBoneMatrices[index] = globalTransformation * offset;
	}

	for (int i = 0; i < node->childrenCount; i++)
		CalculateBoneTransform(&node->children[i], globalTransformation);
}

asModelAnimator* asModelAnimator::CreateBlend(float time, asAnimation* animBlendTo)
{
	asAnimation* blendAnimation = new asAnimation(*this->m_CurrentAnimation);

	std::vector<asBone>& previousBones = blendAnimation->GetBones();
	for (int i = 0; i < previousBones.size(); i++)
	{
		KeyPosition positionStart, positionEnd;
		KeyRotation rotationStart, rotationEnd;
		KeyScale scaleStart, scaleEnd;
		positionStart.position = previousBones[i].getLastPosition();
		rotationStart.orientation = previousBones[i].getLastRotation();
		scaleStart.scale = previousBones[i].getLastScale();
		positionStart.timeStamp = 0.f;
		rotationStart.timeStamp = 0.f;
		scaleStart.timeStamp = 0.f;

		positionEnd = animBlendTo->GetBones()[i].GetFirstPosition();
		rotationEnd = animBlendTo->GetBones()[i].GetFirstRotation();
		scaleEnd = animBlendTo->GetBones()[i].GetFirstScale();
		positionEnd.timeStamp = time * blendAnimation->GetTicksPerSecond();
		rotationEnd.timeStamp = time * blendAnimation->GetTicksPerSecond();
		scaleEnd.timeStamp = time * blendAnimation->GetTicksPerSecond();
		previousBones[i].ClearKeyFrames();
		previousBones[i].AddPositionKeyFrame(positionStart);
		previousBones[i].AddPositionKeyFrame(positionEnd);
		previousBones[i].AddRotationKeyFrame(rotationStart);
		previousBones[i].AddRotationKeyFrame(rotationEnd);
		previousBones[i].AddScaleKeyFrame(scaleStart);
		previousBones[i].AddScaleKeyFrame(scaleEnd);
	}

	asModelAnimator* animator = new asModelAnimator(blendAnimation);
	return animator;
}

std::vector<glm::mat4> asModelAnimator::GetFinalBoneMatrices()
{
	return m_FinalBoneMatrices;
}