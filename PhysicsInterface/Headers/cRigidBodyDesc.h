#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

namespace gdpPhysics
{
	class cRigidBodyDesc
	{
	public:
		cRigidBodyDesc()
			: mass(1.f)
			, isStatic(false)
			, position(glm::vec3(0.f))
			, linearVelocity(glm::vec3(0.f))
			, restitution(0.8f)
			, friction(0.95f)
			, rotation(glm::quat(1.0f, 0.f, 0.f, 0.f)){}

		float mass;

		bool isStatic;

		glm::vec3 position;
		glm::vec3 linearVelocity;

		glm::quat rotation;
		glm::vec3 angularVelocity;

		float linearDamping;
		float angularDamping;
		float restitution;
		float friction;
	};
}