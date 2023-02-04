#pragma once


namespace gdpPhysics
{
	class iConstraint
	{
	public:
		virtual ~iConstraint() {};
		iConstraint() {};

	private:
		iConstraint(const iConstraint&) {}
		iConstraint& operator=(const iConstraint&) { return *this; }
	};
}