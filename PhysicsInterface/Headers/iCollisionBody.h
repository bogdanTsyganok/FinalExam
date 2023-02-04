#pragma once

#include "eBodyType.h"

namespace gdpPhysics
{
	class iCollisionBody
	{
	public:
		virtual ~iCollisionBody(void) { }
		eBodyType GetBodyType(void) const { return mType; }

	protected:
		iCollisionBody(eBodyType type) : mType(type) { }

	private:
		eBodyType mType;

		iCollisionBody(const iCollisionBody&) { }
		iCollisionBody& operator=(const iCollisionBody&) { return *this; }
	};
}