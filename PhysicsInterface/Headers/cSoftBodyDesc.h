#pragma once

#include <vector>
#include <glm/vec3.hpp>

namespace gdpPhysics
{
	class cSoftBodyDesc
	{
	public:
		cSoftBodyDesc(void) { }
		std::vector<glm::vec3> nodePositions;
		std::vector<unsigned int> triangulatedIndices;
	};
}