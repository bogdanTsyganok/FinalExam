#include "Constraint.h"

namespace gdpPhysics
{
	Constraint::~Constraint()
	{
		if (mConstr != nullptr)
		{
			delete mConstr;
		}
	}

	Constraint::Constraint(btTypedConstraint* constr)
	{
		mConstr = constr;
	}

	btTypedConstraint* Constraint::GetConstraint()
	{
		return mConstr;
	}
}