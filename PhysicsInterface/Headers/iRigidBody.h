#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

#include "iCollisionBody.h"

namespace gdpPhysics
{
	class iRigidBody : public iCollisionBody
	{
	public:
		virtual ~iRigidBody() {}

		virtual void GetPosition(glm::vec3& positionOut) = 0;
		virtual void SetPosition(const glm::vec3& positionIn) = 0;

		virtual void GetOrientation(glm::quat& orientationOut) = 0;
		virtual void SetOrientation(const glm::quat& orientationIn) = 0;

		virtual void ApplyForce(const glm::vec3& force) = 0;
		virtual void ApplyForceAtPoint(const glm::vec3& force, const glm::vec3& relativePoint) = 0;

		virtual void ApplyImpulse(const glm::vec3& impulse) = 0;
		virtual void ApplyImpulseAtPoint(const glm::vec3& impulse, const glm::vec3& relativePoint) = 0;

		virtual void ApplyTorque(const glm::vec3& torque) = 0;
		virtual void ApplyTorqueImpulse(const glm::vec3& torqueImpulse) = 0;

		virtual void SetUserPointer(void* ptr) = 0;
		virtual void* GetUserPointer() = 0;

	protected:
		iRigidBody() : iCollisionBody(RIGID_BODY) { }

	private:
		iRigidBody(const iRigidBody&) : iCollisionBody(RIGID_BODY) { }
		iRigidBody& operator=(const iRigidBody&) { return *this; }
	};
}