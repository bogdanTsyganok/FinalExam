#include "shapes.h"

#include <glm/glm.hpp>

namespace gdpPhysics
{
	/* BOX SHAPE */
	BoxShape::BoxShape(const glm::vec3& halfExtents)
		: iShape(ShapeTypeBox)
		, m_HalfExtents(halfExtents)
	{
	}

	BoxShape::~BoxShape()
	{}

	BoxShape* BoxShape::Cast(iShape* shape)
	{
		return dynamic_cast<BoxShape*>(shape);
	}

	/* CYLINDER SHAPE */
	CylinderShape::CylinderShape(const glm::vec3& halfExtents)
		: iShape(ShapeTypeCylinder)
		, m_HalfExtents(halfExtents)
	{
	}

	CylinderShape::~CylinderShape()
	{}

	CylinderShape* CylinderShape::Cast(iShape* shape)
	{
		return dynamic_cast<CylinderShape*>(shape);
	}

/* SPHERE SHAPE */
	SphereShape::SphereShape(float radius)
		: iShape(ShapeTypeSphere)
		, m_Radius(radius)
	{}

	SphereShape::~SphereShape() 
	{}

	float SphereShape::GetRadius() const
	{
		return m_Radius;
	}

	SphereShape* SphereShape::Cast(iShape* shape)
	{
		return dynamic_cast<SphereShape*>(shape);
	}

/* PLANE SHAPE */
	PlaneShape::PlaneShape(float dotProduct, const glm::vec3& normal)
		: iShape(ShapeTypePlane)
		, m_DotProduct(dotProduct)
		, m_Normal(normal)
	{
	}

	PlaneShape::PlaneShape(const glm::vec3& a, const glm::vec3& b, const glm::vec3& c)
		: iShape(ShapeTypePlane)
	{
		m_Normal = glm::cross(b - a, c - a);
		m_Normal = glm::normalize(m_Normal);
		m_DotProduct = glm::dot(m_Normal, a);
		if (m_DotProduct == 0.f)
		{
			m_DotProduct = glm::dot(m_Normal, b);
		}
	}

	PlaneShape* PlaneShape::Cast(iShape* shape)
	{
		return dynamic_cast<PlaneShape*>(shape);
	}

	ConeShape::ConeShape(float height, float radius) : iShape(ShapeTypeCone), m_Height(height), m_Radius(radius)
	{}

	ConeShape* ConeShape::Cast(iShape* shape)
	{
		return dynamic_cast<ConeShape*>(shape);
	}
}