#include "TransformComponent.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Scene/Scene.h"
#include "Math/Math.h"

TransformComponent::TransformComponent(Actor* owner)
	: TransformBaseComponent(owner)
{
}

void TransformComponent::CalculateLocalModelMatrix()
{
	glm::mat4 rotation = glm::toMat4(glm::quat(glm::radians(m_LocalRotation)));
	m_LocalModelMatrix = glm::translate(glm::mat4(1.0f), m_LocalPosition) * rotation * glm::scale(glm::mat4(1.0f), m_LocalScale);
}


