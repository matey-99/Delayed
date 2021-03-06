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

	//m_ViewMatrix = glm::mat4(1.0f);
	//m_PreviousViewMatrix = glm::mat4(1.0);

	m_MovementSpeed = 50.0f;
	m_RotateSpeed = 50.0f;

	m_Frustum = CreateRef<Frustum>();
}

void EditorCamera::Update()
{
	Front = CalculateFrontVector();
	Right = CalculateRightVector();

	m_PreviousViewMatrix = m_ViewMatrix;
	m_PreviousViewProjectionMatrix = m_ViewProjectionMatrix;

	m_Frustum->UpdateFrustum(GetViewProjectionMatrix());
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

glm::vec3 EditorCamera::GetFront()
{
	return Front;
}

glm::vec3 EditorCamera::GetRight() {
    return Right;
}

glm::vec3 EditorCamera::GetUp() {
    return Up;
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
	//m_PreviousViewMatrix = m_ViewMatrix;
	m_ViewMatrix = glm::lookAt(Position, Position + Front, Up);

	return m_ViewMatrix;
}

glm::mat4 EditorCamera::GetPreviousViewMatrix()
{
	return m_PreviousViewMatrix;
}

glm::mat4 EditorCamera::GetProjectionMatrix()
{
	return glm::perspective(glm::radians(FieldOfView), AspectRactio.x / AspectRactio.y, Near, Far);
}

glm::mat4 EditorCamera::GetViewProjectionMatrix()
{
	//m_PreviousViewProjectionMatrix = m_ViewProjectionMatrix;
	m_ViewProjectionMatrix = GetProjectionMatrix() * GetViewMatrix();

	return m_ViewProjectionMatrix;
}

glm::mat4 EditorCamera::GetPreviousViewProjectionMatrix()
{
	return m_PreviousViewProjectionMatrix;
}

Ref<Frustum> EditorCamera::GetFrustum()
{
	return m_Frustum;
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
