#include "Transform.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

#include "Scene/Actor.h"

Transform::Transform(Actor* owner, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale)
	: Owner(owner), LocalPosition(position), LocalRotation(rotation), LocalScale(scale)
{
}

void Transform::CalculateModelMatrix()
{
	ModelMatrix = GetLocalModelMatrix();
}
void Transform::CalculateModelMatrix(const glm::mat4& parentGlobalModelMatrix)
{
	ModelMatrix = parentGlobalModelMatrix * GetLocalModelMatrix();
}

glm::mat4 Transform::GetLocalModelMatrix()
{
	glm::mat4 rotation = glm::toMat4(glm::quat(glm::radians(LocalRotation)));
	return glm::translate(glm::mat4(1.0f), LocalPosition) * rotation * glm::scale(glm::mat4(1.0f), LocalScale);
}