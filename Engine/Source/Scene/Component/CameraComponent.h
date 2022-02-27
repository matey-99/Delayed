#pragma once

#include "InGameComponent.h"

class CameraComponent : public InGameComponent
{
private:

public:
	CameraComponent(Entity* owner);

	virtual void Begin() override;
	virtual void Update() override;
	virtual void Destroy() override;

	virtual void BeginPlay() override;
	virtual void Tick(float deltaTime) override;
	virtual void EndPlay() override;

	glm::mat4 GetViewMatrix();
	glm::mat4 GetProjectionMatrix();
	glm::mat4 GetViewProjectionMatrix();

	inline void SetAspectRatio(glm::vec2 aspectRatio) { m_AspectRatio = aspectRatio; }

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
	friend class EntityDetailsPanel;
};