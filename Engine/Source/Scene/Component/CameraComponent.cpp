#include "CameraComponent.h"

#include <glm/gtc/matrix_transform.hpp>

#include "Scene/Actor.h"

CameraComponent::CameraComponent(Actor* owner)
	: Component(owner)
{
	m_FieldOfView = 45.0f;
	m_AspectRatio = glm::vec2(16.0f, 9.0f);
	m_NearClipPlane = 0.1f;
	m_FarClipPlane = 1000.0f;

	m_Right = CalculateRightVector();
	m_Up = glm::vec3(0.0f, 1.0f, 0.0f);
}

void CameraComponent::Start()
{
}

void CameraComponent::Update(float deltaTime)
{
	m_Front = CalculateFrontVector();
	m_Right = CalculateRightVector();
}

void CameraComponent::Destroy()
{
}

glm::vec3 CameraComponent::GetWorldPosition()
{
	return m_Owner->GetTransform()->GetWorldPosition();
}

glm::vec3 CameraComponent::GetDirection()
{
	return m_Front;
}

glm::vec2 CameraComponent::GetAspectRatio()
{
	return m_AspectRatio;
}

float CameraComponent::GetFieldOfView()
{
	return m_FieldOfView;
}

float CameraComponent::GetNearClipPlane()
{
	return m_NearClipPlane;
}

float CameraComponent::GetFarClipPlane()
{
	return m_FarClipPlane;
}

glm::mat4 CameraComponent::GetViewMatrix()
{
	return glm::lookAt(m_Owner->GetTransform()->GetWorldPosition(), m_Owner->GetTransform()->GetWorldPosition() + m_Front, m_Up);
}

glm::mat4 CameraComponent::GetProjectionMatrix()
{
	return glm::perspective(glm::radians(m_FieldOfView), m_AspectRatio.x / m_AspectRatio.y, m_NearClipPlane, m_FarClipPlane);
}

glm::mat4 CameraComponent::GetViewProjectionMatrix()
{
	return GetProjectionMatrix() * GetViewMatrix();
}

glm::vec3 CameraComponent::CalculateFrontVector()
{
	glm::vec3 rotation = m_Owner->GetTransform()->GetWorldRotation();
	float pitch = rotation.x;
	float yaw = rotation.y;

	glm::vec3 direction;
	direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	direction.y = sin(glm::radians(pitch));
	direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	return glm::normalize(direction);
}

glm::vec3 CameraComponent::CalculateRightVector()
{
	return glm::normalize(glm::cross(m_Front, m_Up));
}