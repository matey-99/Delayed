#include "EditorCamera.h"

#include <glm/gtc/matrix_transform.hpp>

#include "Scene/Scene.h"
#include "Scene/Actor.h"

EditorCamera::EditorCamera(Scene* scene, glm::vec3 position, glm::vec3 front, glm::vec3 up, float yaw, float pitch, float movementSpeed)
	: m_Scene(scene), Position(position), Front(front), Up(up), Yaw(yaw), Pitch(pitch), m_MovementSpeed(movementSpeed)
{
	Right = CalculateRightVector();

	FieldOfView = 45.0f;
	AspectRactio = glm::vec2(16.0f, 9.0f);
	Near = 0.1f;
	Far = 500.0f;

	m_MovementSpeed = 50.0f;
	m_RotateSpeed = 50.0f;
}

void EditorCamera::Update()
{
	Front = CalculateFrontVector();
	Right = CalculateRightVector();
}

void EditorCamera::Move(CameraMovement movementDirection, float deltaTime)
{
	switch (movementDirection)
	{
	case CameraMovement::Forward:
		Position += Front * m_MovementSpeed * deltaTime;
		break;
	case CameraMovement::Backward:
		Position -= Front * m_MovementSpeed * deltaTime;
		break;
	case CameraMovement::Left:
		Position -= Right * m_MovementSpeed * deltaTime;
		break;
	case CameraMovement::Right:
		Position += Right * m_MovementSpeed * deltaTime;
		break;
	}
}

void EditorCamera::Move(float xoffset, float yoffset, float deltaTime)
{
	Position += xoffset * m_MovementSpeed * Right * deltaTime;
	Position += yoffset * m_MovementSpeed * Up * deltaTime;
}

void EditorCamera::Move(float yoffset, float deltaTime)
{
	Position += (float)yoffset * 3 * m_MovementSpeed * Front * deltaTime;
}

void EditorCamera::Rotate(float yaw, float pitch, float deltaTime)
{
	Yaw += yaw * 3 * m_RotateSpeed * deltaTime;
	Pitch += pitch * 3 * m_RotateSpeed * deltaTime;

	Yaw = glm::mod(Yaw, 360.0f);
	Pitch = glm::clamp(Pitch, -89.0f, 89.0f);
}

glm::vec3 EditorCamera::GetWorldPosition()
{
	return Position;
}

glm::vec3 EditorCamera::GetDirection()
{
	return Front;
}

glm::vec2 EditorCamera::GetAspectRatio()
{
	return AspectRactio;
}

float EditorCamera::GetFieldOfView()
{
	return FieldOfView;
}

float EditorCamera::GetNearClipPlane()
{
	return Near;
}

float EditorCamera::GetFarClipPlane()
{
	return Far;
}

glm::mat4 EditorCamera::GetViewMatrix()
{
	return glm::lookAt(Position, Position + Front, Up);
}

glm::mat4 EditorCamera::GetProjectionMatrix()
{
	return glm::perspective(glm::radians(FieldOfView), AspectRactio.x / AspectRactio.y, Near, Far);
}

glm::mat4 EditorCamera::GetViewProjectionMatrix()
{
	return GetProjectionMatrix() * GetViewMatrix();
}

glm::vec3 EditorCamera::CalculateFrontVector()
{
	glm::vec3 direction;
	direction.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
	direction.y = sin(glm::radians(Pitch));
	direction.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
	return glm::normalize(direction);
}

glm::vec3 EditorCamera::CalculateRightVector()
{
	return glm::normalize(glm::cross(Front, Up));
}
