#pragma once

#include "iShape.h"
#include <glm/vec3.hpp>

namespace gdpPhysics
{
	class BoxShape : public iShape
	{
	public:
		BoxShape(const glm::vec3& halfExtents);
		virtual ~BoxShape();

		const glm::vec3& GetHalfExtents() const { return m_HalfExtents; }

		static BoxShape* Cast(iShape* shape);

	private:
		glm::vec3 m_HalfExtents;

		BoxShape(const BoxShape&) : iShape(ShapeTypeBox) { }
		BoxShape& operator=(const BoxShape&) { return *this; }
	};

	class CylinderShape : public iShape
	{
	public:
		CylinderShape(const glm::vec3& halfExtents);
		virtual ~CylinderShape();

		const glm::vec3& GetHalfExtents() const { return m_HalfExtents; }

		static CylinderShape* Cast(iShape* shape);

	private:
		glm::vec3 m_HalfExtents;

		CylinderShape(const CylinderShape&) : iShape(ShapeTypeCylinder) { }
		CylinderShape& operator=(const CylinderShape&) { return *this; }
	};

	class SphereShape : public iShape
	{
	public:
		SphereShape(float radius);
		virtual ~SphereShape();

		float GetRadius() const;
		static SphereShape* Cast(iShape* shape);

	protected:
		SphereShape(eShapeType shapeType)
			: iShape(shapeType) {}

	private:
		float m_Radius;
		SphereShape(const SphereShape&) : iShape(ShapeTypeSphere) {}
		SphereShape& operator=(const SphereShape&) { return *this; }
	};

	class PlaneShape : public iShape
	{
	public:
		PlaneShape(float dotProduct, const glm::vec3& normal);
		PlaneShape(const glm::vec3& a, const glm::vec3& b, const glm::vec3& c);

		float GetDotProduct() const { return m_DotProduct; }
		const glm::vec3& GetNormal() { return m_Normal; }

		static PlaneShape* Cast(iShape* shape);

	private:
		glm::vec3 m_Normal;
		float m_DotProduct;

		PlaneShape(const PlaneShape&) : iShape(ShapeTypePlane) { }
		PlaneShape& operator=(const PlaneShape&) { return *this; }
	};


	class ConeShape : public iShape
	{
	public:
		ConeShape(float height, float radius);

		float GetHeight() const { return m_Height; }
		float GetRadius() const { return m_Radius; }

		static ConeShape* Cast(iShape* shape);
	private:
		float m_Height;
		float m_Radius;

		ConeShape(const ConeShape&) : iShape(ShapeTypeCone){}
		ConeShape& operator=(const ConeShape&) { return *this; }
	};
}