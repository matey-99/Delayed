#pragma once

#include <glm/glm.hpp>

#include "Camera/Frustum.h"
#include "Math/Ray.h"

class Camera
{
public:
	Camera() {}

	virtual glm::vec3 GetWorldPosition() = 0;
	virtual glm::vec3 GetFront() = 0;
	virtual glm::vec3 GetRight() = 0;
	virtual glm::vec3 GetUp() = 0;

	virtual glm::vec2 GetAspectRatio() = 0;
	virtual float GetFieldOfView() = 0;
	virtual float GetNearClipPlane() = 0;
	virtual float GetFarClipPlane() = 0;

	virtual glm::mat4 GetViewMatrix() = 0;
	virtual glm::mat4 GetProjectionMatrix() = 0;
	virtual glm::mat4 GetPreviousViewMatrix() = 0;
	virtual glm::mat4 GetViewProjectionMatrix() = 0;
	virtual glm::mat4 GetPreviousViewProjectionMatrix() = 0;


	virtual Ref<Frustum> GetFrustum() = 0;

	virtual void SetAspectRatio(glm::vec2) = 0;

	Ray ScreenPointToRay(glm::vec2 position);
};