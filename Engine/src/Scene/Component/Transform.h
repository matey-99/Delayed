#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "typedefs.h"

class Entity;

struct Transform
{
public:
	Entity* Owner;

	glm::vec3 LocalPosition;
	glm::vec3 LocalRotation;
	glm::vec3 LocalScale;

	glm::mat4 ModelMatrix = glm::mat4(1.0f);

public:
	Transform(Entity* owner, glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 rotation = glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f));

	//glm::vec3 GetWorldPosition();
	//glm::vec3 GetWorldRotation();
	//glm::vec3 GetWorldScale();

	//void SetWorldPosition(glm::vec3 position);
	//void SetWorldRotation(glm::vec3 rotation);
	//void SetWorldScale(glm::vec3 scale);

	glm::mat4 GetLocalModelMatrix();
	void CalculateModelMatrix();
	void CalculateModelMatrix(const glm::mat4& parentGlobalModelMatrix);
};