#pragma once

#include "Camera/Camera.h"
#include "Component.h"

#include <glm/glm.hpp>

class CameraComponent : public Camera, public Component
{
public:
	CameraComponent(Actor* owner);

	virtual void Start() override;
	virtual void Update(float deltaTime) override;
	virtual void Destroy() override;

	virtual glm::vec3 GetWorldPosition() override;
	virtual glm::vec3 GetDirection() override;

	virtual glm::vec2 GetAspectRatio() override;
	virtual float GetFieldOfView() override;
	virtual float GetNearClipPlane() override;
	virtual float GetFarClipPlane() override;

	virtual glm::mat4 GetViewMatrix() override;
	virtual glm::mat4 GetProjectionMatrix() override;
	virtual glm::mat4 GetViewProjectionMatrix() override;

	inline void SetAspectRatio(glm::vec2 aspectRatio) override { m_AspectRatio = aspectRatio; }

private:
	glm::vec3 CalculateFrontVector();
	glm::vec3 CalculateRightVector();

private:
	glm::vec2 m_AspectRatio;
	float m_FieldOfView;
	float m_NearClipPlane;
	float m_FarClipPlane;

	glm::vec3 m_Front;
	glm::vec3 m_Up;
	glm::vec3 m_Right;

	friend class SceneSerializer;
	friend class ActorDetailsPanel;
};