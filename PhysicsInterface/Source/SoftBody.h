#pragma once

#include "../Headers/iSoftBody.h"
#include "../Headers/cSoftBodyDesc.h"

#include <btBulletDynamicsCommon.h>
#include <BulletSoftBody/btSoftBody.h>

namespace gdpPhysics
{
	class SoftBody : public iSoftBody
	{
	public:
		SoftBody(const cSoftBodyDesc& desc);
		~SoftBody();

		virtual unsigned int GetNumNodes();
		virtual void GetNodePosition(unsigned int nodeIndex, glm::vec3& nodePositionOut);

		btSoftBody* GetBulletBody() { return mBulletBody; }
	private:
		btSoftBody* mBulletBody;
	};
}