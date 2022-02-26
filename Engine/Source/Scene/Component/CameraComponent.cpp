#include "CameraComponent.h"

#include <glm/gtc/matrix_transform.hpp>

#include "Scene/Entity.h"

CameraComponent::CameraComponent(Entity* owner)
	: InGameComponent(owner)
{
	m_FieldOfView = 45.0f;
	m_AspectRatio = glm::vec2(16.0f, 9.0f);
	m_NearClipPlane = 0.1f;
	m_FarClipPlane = 1000.0f;

	m_Right = CalculateRightVector();
	m_Up = glm::vec3(0.0f, 1.0f, 0.0f);
}

void CameraComponent::Begin()
{
}

void CameraComponent::Update()
{
}

void CameraComponent::Destroy()
{
}

void CameraComponent::BeginPlay()
{


}

void CameraComponent::Tick(float deltaTime)
{
	m_Front = CalculateFrontVector();
	m_Right = CalculateRightVector();
}

void CameraComponent::EndPlay()
{

}

glm::mat4 CameraComponent::GetViewMatrix()
{
	return glm::lookAt(m_Owner->GetWorldPosition(), m_Owner->GetWorldPosition() + m_Front, m_Up);
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
	glm::vec3 rotation = m_Owner->GetWorldRotation();
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