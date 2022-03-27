#include "RectTransformComponent.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Scene/Scene.h"
#include "Math/Math.h"

RectTransformComponent::RectTransformComponent(Actor* owner)
	: TransformBaseComponent(owner)
{
	auto r = Renderer::GetInstance();

	m_LocalPosition = glm::vec3(0.0f + (r->GetWindowWidth() / 2), 0.0f + (r->GetWindowHeight() / 2), 0.0f);
}

void RectTransformComponent::CalculateLocalModelMatrix()
{
	auto r = Renderer::GetInstance();

	glm::vec3 position = m_LocalPosition;
	position.x -= (r->GetWindowWidth() / 2);
	position.y -= (r->GetWindowHeight() / 2);

	glm::mat4 rotation = glm::toMat4(glm::quat(glm::radians(m_LocalRotation)));
	m_LocalModelMatrix = glm::translate(glm::mat4(1.0f), position) * rotation * glm::scale(glm::mat4(1.0f), m_LocalScale);
}
