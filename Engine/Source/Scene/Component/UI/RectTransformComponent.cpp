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

	m_LocalPosition = glm::vec3(0.0f, 0.0f, 0.0f);

	m_AnchorType = AnchorType::Center;
	m_Anchor = glm::vec2(0.0f);
}

void RectTransformComponent::SetAnchor(AnchorType type)
{
	m_AnchorType = type;

	switch (type)
	{
	case AnchorType::Center:
		m_Anchor = glm::vec2(0.0f);
		break;
	case AnchorType::Top:
		m_Anchor = glm::vec2(0.0f, 1.0f);
		break;
	case AnchorType::Bottom:
		m_Anchor = glm::vec2(0.0f, -1.0f);
		break;
	case AnchorType::Left:
		m_Anchor = glm::vec2(-1.0f, 0.0f);
		break;
	case AnchorType::Right:
		m_Anchor = glm::vec2(1.0f, 0.0f);
		break;
	case AnchorType::TopLeft:
		m_Anchor = glm::vec2(-1.0f, 1.0f);
		break;
	case AnchorType::TopRight:
		m_Anchor = glm::vec2(1.0f, 1.0f);
		break;
	case AnchorType::BottomLeft:
		m_Anchor = glm::vec2(-1.0f, -1.0f);
		break;
	case AnchorType::BottomRight:
		m_Anchor = glm::vec2(1.0f, -1.0f);
		break;
	}

	CalculateWorldModelMatrix();
}

void RectTransformComponent::CalculateLocalModelMatrix()
{
	auto r = Renderer::GetInstance();

	glm::vec3 position = m_LocalPosition;
	position.x /= r->GetWindowWidth();
	position.y /= r->GetWindowHeight();
	position.x += m_Anchor.x;
	position.y += m_Anchor.y;

	glm::vec3 scale = m_LocalScale;

	// If owner isn't UI Root
	if (m_Owner->GetID() != 1)
	{
		scale.x /= r->GetWindowWidth();
		scale.y /= r->GetWindowHeight();
		scale.x *= 100.0f;
		scale.y *= 100.0f;
	}


	glm::mat4 rotation = glm::toMat4(glm::quat(glm::radians(m_LocalRotation)));
	m_LocalModelMatrix = glm::translate(glm::mat4(1.0f), position) * rotation * glm::scale(glm::mat4(1.0f), scale);
}
