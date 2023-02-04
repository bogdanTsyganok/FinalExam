#pragma once
#include "../Headers/iConstraint.h"
#include <btBulletDynamicsCommon.h>


namespace gdpPhysics
{
	class Constraint : public iConstraint
	{
	public:
		virtual ~Constraint();
		Constraint(btTypedConstraint* constr);

		btTypedConstraint* GetConstraint();

	private:
		btTypedConstraint* mConstr;


		Constraint(const Constraint&) {}
		Constraint& operator=(const Constraint&) { return *this; }
	};
}