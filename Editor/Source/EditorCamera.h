#pragma once

#include "Camera/Camera.h"

#include <glm/glm.hpp>

class Scene;

enum class CameraMovement
{
	Forward, Backward, Left, Right
};

class EditorCamera : public Camera
{
public:
	glm::vec3 Position;

	glm::vec3 Front;
	glm::vec3 Up;
	glm::vec3 Right;

	float FieldOfView;
	glm::vec2 AspectRactio;
	float Near;
	float Far;

	float Yaw;
	float Pitch;


	EditorCamera(Scene* scene, glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 front = glm::vec3(0.0f, 0.0f, -1.0f),
		   glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = -90.0f, float pitch = 0.0f, float movementSpeed = 10.0f);

	void Update();

	void Move(CameraMovement movementDirection, float deltaTime);
	void Move(float xoffset, float yoffset, float deltaTime);
	void Move(float yoffset, float deltaTime);

	void Rotate(float yaw, float pitch, float deltaTime);

	virtual glm::vec3 GetWorldPosition() override;

	virtual glm::vec2 GetAspectRatio() override;
	virtual float GetFieldOfView() override;
	virtual float GetNearClipPlane() override;
	virtual float GetFarClipPlane() override;

	virtual glm::mat4 GetViewMatrix() override;
	virtual glm::mat4 GetProjectionMatrix() override;
	virtual glm::mat4 GetViewProjectionMatrix() override;

private:
	glm::vec3 CalculateFrontVector();
	glm::vec3 CalculateRightVector();

private:
	float m_MovementSpeed;
	float m_RotateSpeed;
	Scene* m_Scene;

	friend class ImGuiRenderer;
};
