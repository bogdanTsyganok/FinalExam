#pragma once

#include <glm/glm.hpp>
#include <map>
#include <vector>
#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include "animation.h"
#include "bone.h"

class asModelAnimator
{
public:
	asModelAnimator(asAnimation* animation);

	void UpdateAnimation(float dt);

	void PlayAnimation(asAnimation* pAnimation);

	void CalculateBoneTransform(const AssimpNodeData* node, glm::mat4 parentTransform);

	void Reset()
	{
		m_CurrentTime = 0.f;
	}

	//two keyframeblend, use same model animation
	asModelAnimator* CreateBlend(float time, asAnimation* animBlendTo);

	std::vector<glm::mat4> GetFinalBoneMatrices();

	asAnimation* getCurrentAnimation()
	{
		return m_CurrentAnimation;
	}
private:
	std::vector<glm::mat4> m_FinalBoneMatrices;
	asAnimation* m_CurrentAnimation;
	float m_CurrentTime;
	float m_DeltaTime;

};