#pragma once

#include <glm/glm.hpp>

class Camera
{
public:
	Camera() {}

	virtual glm::vec3 GetWorldPosition() = 0;

	virtual glm::vec2 GetAspectRatio() = 0;
	virtual float GetFieldOfView() = 0;
	virtual float GetNearClipPlane() = 0;
	virtual float GetFarClipPlane() = 0;

	virtual glm::mat4 GetViewMatrix() = 0;
	virtual glm::mat4 GetProjectionMatrix() = 0;
	virtual glm::mat4 GetViewProjectionMatrix() = 0;
};