#include "BoundingBox.h"

BoundingBox::BoundingBox()
	: Center(glm::vec3(0.0f)), Extents(glm::vec3(0.0f)), Min(glm::vec3(0.0f)), Max(glm::vec3(0.0f)), Size(glm::vec3(0.0f))
{
}

BoundingBox::BoundingBox(glm::vec3 min, glm::vec3 max)
	: Center((min + max) * 0.5f), Extents(max - Center), Min(min), Max(max), Size(max - min)
{

}

BoundingBox::BoundingBox(std::vector<glm::vec3> points)
{
	if (points.size() < 1)
		return;

	Min = points.at(0);
	Max = points.at(0);

	for (auto point : points)
	{
		Min = glm::min(Min, point);
		Max = glm::max(Max, point);
	}

	Center = (Min + Max) * 0.5f;
	Extents = Max - Center;
	Size = Max - Min;
}

std::vector<glm::vec3> BoundingBox::GetPoints()
{
	std::vector<glm::vec3> points;
	points.push_back({ Min.x, Min.y, Min.z });
	points.push_back({ Max.x, Min.y, Min.z });
	points.push_back({ Max.x, Min.y, Max.z });
	points.push_back({ Min.x, Min.y, Max.z });
	points.push_back({ Min.x, Max.y, Min.z });
	points.push_back({ Max.x, Max.y, Min.z });
	points.push_back({ Max.x, Max.y, Max.z });
	points.push_back({ Min.x, Max.y, Max.z });

	return points;
}
