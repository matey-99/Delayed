#include "Transform.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

#include "Scene/Entity.h"

Transform::Transform(Entity* owner, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale)
	: Owner(owner), LocalPosition(position), LocalRotation(rotation), LocalScale(scale)
{
}

//glm::vec3 Transform::GetWorldPosition()
//{
//	return LocalPosition + (Parent ? Parent->GetWorldPosition() : glm::vec3(0.0f));
//}
//
//glm::vec3 Transform::GetWorldRotation()
//{
//	return LocalRotation + (Parent ? Parent->GetWorldRotation() : glm::vec3(0.0f));
//}
//
//glm::vec3 Transform::GetWorldScale()
//{
//	return LocalScale * (Parent ? Parent->GetWorldScale() : glm::vec3(1.0f));
//
//}
//
//void Transform::SetWorldPosition(glm::vec3 position)
//{
//	LocalPosition = position - (Parent ? Parent->GetWorldPosition() : glm::vec3(0.0f));
//}
//
//void Transform::SetWorldRotation(glm::vec3 rotation)
//{
//	LocalRotation = rotation - (Parent ? Parent->GetWorldRotation() : glm::vec3(0.0f));
//}
//
//void Transform::SetWorldScale(glm::vec3 scale)
//{
//	LocalScale = (Parent ? scale / Parent->GetWorldScale() : scale);
//}

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