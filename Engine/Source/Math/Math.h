#pragma once

#include "Core.h"

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

	glm::vec3 Direction(const glm::vec3& a, const glm::vec3& b);

	float Lerp(float a, float b, float alpha);
	
	glm::vec3 Lerp(glm::vec3 a, glm::vec3 b, float alpha);

	glm::vec3 Lerp(glm::vec3 a, glm::vec3 b, float alpha);

	glm::vec4 Lerp(glm::vec4 a, glm::vec4 b, float alpha);

	float Magnitude(const glm::vec3& v);

	float Distance(const glm::vec3& a, const glm::vec3& b);

	bool IsNearlyEqual(float a, float b, float errorTolerance = 0.001f);

	bool IsNearlyEqual(const glm::vec3& a, const glm::vec3& b, float errorTolerance = 0.001f);

	glm::vec3 Normalize(const glm::vec3& v);

    int RandomRange(int from, int to);

	const float Infinity = std::numeric_limits<float>::infinity();

	const glm::vec3 ForwardVector = { 0.0f, 0.0f, -1.0f };
	const glm::vec3 UpVector = { 0.0f, 1.0f, 0.0f };
	const glm::vec3 RightVector = { 1.0f, 0.0f, 0.0f };
}

