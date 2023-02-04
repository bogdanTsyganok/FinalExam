#pragma once

#include "iCollisionBody.h"

namespace gdpPhysics
{
	class iCollisionListener
	{
	public:
		virtual ~iCollisionListener(void) { }

		virtual void Collision(iCollisionBody* bodyA, iCollisionBody* bodyB) = 0;

	protected:
		iCollisionListener(void) { }

	private:
		iCollisionListener(const iCollisionListener&) { }
		iCollisionListener& operator=(const iCollisionListener&) { return *this; }
	};
}