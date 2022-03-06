#pragma once

#include "glm/glm.hpp"

struct BoundingBox
{
	glm::vec3 Center;
	glm::vec3 Extents;
	glm::vec3 Min;
	glm::vec3 Max;
	glm::vec3 Size;

	glm::mat3 Rotation;

	BoundingBox();
	BoundingBox(glm::vec3 min, glm::vec3 max);
	BoundingBox(std::vector<glm::vec3> points);

	std::vector<glm::vec3> GetPoints();
};