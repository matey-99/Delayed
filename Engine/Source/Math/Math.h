#pragma once

#include "Core.h"

#include "Time/Time.h"

namespace Math
{
	const float Infinity = std::numeric_limits<float>::infinity();

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

	float Smoothstep(float a, float b, float alpha);

	glm::vec3 Smoothstep(glm::vec3 a, glm::vec3 b, float alpha);

	float SmoothDamp(float current, float target, float& velocity, float smoothTime, float deltaTime, float maxSpeed = Infinity);

	glm::vec3 SmoothDamp(glm::vec3 current, glm::vec3 target, glm::vec3& velocity, float smoothTime, float deltaTime, float maxSpeed = Infinity);

	float Magnitude(const glm::vec3& v);

	float Distance(const glm::vec3& a, const glm::vec3& b);

	bool IsNearlyEqual(float a, float b, float errorTolerance = 0.001f);

	bool IsNearlyEqual(const glm::vec3& a, const glm::vec3& b, float errorTolerance = 0.001f);

	glm::vec3 Normalize(const glm::vec3& v);

	glm::vec2 RandomGradient(int ix, int iy);

	float DotGridGradient(int ix, int iy, float x, float y);

	float PerlinNoise(int x, int y);

	std::vector<glm::vec3> WorleyPoints(int cellsPerAxis, uint64_t seed);

	float WorleyNoise(std::vector<glm::vec3> points, glm::vec3 position);
	
    int RandomRange(int from, int to);

	const glm::vec3 ForwardVector = { 0.0f, 0.0f, -1.0f };
	const glm::vec3 UpVector = { 0.0f, 1.0f, 0.0f };
	const glm::vec3 RightVector = { 1.0f, 0.0f, 0.0f };
}

