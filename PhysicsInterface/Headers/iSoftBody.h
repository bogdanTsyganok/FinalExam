#pragma once

#include <glm/vec3.hpp>

#include "iCollisionBody.h"

namespace gdpPhysics
{
	class iSoftBody : public iCollisionBody
	{
	public:
		virtual ~iSoftBody(void) { }

		virtual unsigned int GetNumNodes() = 0;
		virtual void GetNodePosition(unsigned int nodeIndex, glm::vec3& nodePositionOut) = 0;

	protected:
		iSoftBody(void) : iCollisionBody(SOFT_BODY) { }

	private:
		iSoftBody(const iSoftBody&) : iCollisionBody(SOFT_BODY) { }
		iSoftBody& operator=(const iSoftBody&) { return *this; }
	};
}