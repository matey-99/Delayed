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

	m_LocalPositionUI = m_LocalPosition;
	m_LocalPositionUI.x += m_Anchor.x * r->GetWindowWidth();
	m_LocalPositionUI.y += m_Anchor.y * r->GetWindowHeight();

	m_LocalScaleUI = m_LocalScale;
	if (m_Owner->GetID() == 1)
	{
		m_LocalScaleUI.x /= r->GetWindowWidth();
		m_LocalScaleUI.y /= r->GetWindowHeight();
	}

	glm::mat4 rotation = glm::toMat4(glm::quat(glm::radians(m_LocalRotation)));
	m_LocalModelMatrix = glm::translate(glm::mat4(1.0f), m_LocalPositionUI) * rotation * glm::scale(glm::mat4(1.0f), m_LocalScaleUI);
}
