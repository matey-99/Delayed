#pragma once

#include <glm/glm.hpp>

namespace Math
{
	bool DecomposeMatrix(const glm::mat4 matrix, glm::vec3& translation, glm::vec3& rotation, glm::vec3& scale);

	/// <summary>
	/// Transform given vector by quaternion.
	/// </summary>
	/// <param name="v">Source vector</param>
	/// <param name="q">Quaternion rotation</param>
	/// <returns>Trasformed vector</returns>
	glm::vec3 Transform(const glm::vec3& v, const glm::quat& q);

	float Lerp(float a, float b, float alpha);

	const glm::vec3 Forward = { 0.0f, 0.0f, 1.0f };
}

