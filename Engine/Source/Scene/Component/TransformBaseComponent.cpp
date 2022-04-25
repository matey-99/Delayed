#include "TransformBaseComponent.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/matrix_decompose.hpp>

#include "Scene/Scene.h"
#include "Math/Math.h"

TransformBaseComponent::TransformBaseComponent(Actor* owner)
	: Component(owner)
{
	m_LocalPosition = glm::vec3(0.0f);
	m_LocalRotation = glm::vec3(0.0f);
	m_LocalScale = glm::vec3(1.0f);

	m_WorldPosition = glm::vec3(0.0f);
	m_WorldRotation = glm::vec3(0.0f);
	m_WorldScale = glm::vec3(1.0f);

	m_Parent = nullptr;
	m_Children = std::vector<TransformBaseComponent*>();

	m_LocalModelMatrix = glm::mat4(1.0f);
	m_WorldModelMatrix = glm::mat4(1.0f);

	m_Forward = Math::Transform(Math::ForwardVector, m_WorldRotation);
	m_Right = Math::Transform(Math::RightVector, m_WorldRotation);
}

void TransformBaseComponent::Start()
{
}

void TransformBaseComponent::Update(float deltaTime)
{
}

void TransformBaseComponent::Destroy()
{
}

void TransformBaseComponent::SetLocalPosition(glm::vec3 position)
{
	m_LocalPosition = position;
	CalculateWorldModelMatrix();

	m_Owner->GetScene()->SetChangedSinceLastFrame(true);
}

void TransformBaseComponent::SetLocalRotation(glm::vec3 rotation)
{
	m_LocalRotation = rotation;
	CalculateWorldModelMatrix();

	m_Owner->GetScene()->SetChangedSinceLastFrame(true);
}

void TransformBaseComponent::SetLocalScale(glm::vec3 scale)
{
	m_LocalScale = scale;
	CalculateWorldModelMatrix();

	m_Owner->GetScene()->SetChangedSinceLastFrame(true);
}

glm::vec3 TransformBaseComponent::GetWorldPosition()
{
	return m_WorldPosition;
	//return m_LocalPosition + (m_Parent ? m_Parent->GetWorldPosition() : glm::vec3(0.0f));
}

glm::quat TransformBaseComponent::GetWorldRotation()
{
	return m_WorldRotation;
	//return m_LocalRotation + (m_Parent ? m_Parent->GetWorldRotation() : glm::vec3(0.0f));
}

glm::vec3 TransformBaseComponent::GetWorldScale()
{
	return m_WorldScale;
	//return m_LocalScale * (m_Parent ? m_Parent->GetWorldScale() : glm::vec3(1.0f));
}

void TransformBaseComponent::SetWorldPosition(glm::vec3 position)
{
	SetLocalPosition(position - (m_Parent ? m_Parent->GetWorldPosition() : glm::vec3(0.0f)));
}

void TransformBaseComponent::SetParent(TransformBaseComponent* parent)
{
	if (m_Parent)
		m_Parent->m_Children.erase(std::remove(m_Parent->m_Children.begin(), m_Parent->m_Children.end(), this));

	m_Parent = parent;
	m_Parent->m_Children.emplace_back(this);

	CalculateWorldModelMatrix();
}

void TransformBaseComponent::RemoveChild(TransformBaseComponent* child)
{
	m_Children.erase(std::remove(m_Children.begin(), m_Children.end(), child));
}

void TransformBaseComponent::CalculateWorldModelMatrix()
{
	CalculateLocalModelMatrix();
	OnTransformChanged.Broadcast();

	if (m_Parent)
		m_WorldModelMatrix = m_Parent->GetWorldModelMatrix() * m_LocalModelMatrix;
	else
		m_WorldModelMatrix = m_LocalModelMatrix;

	glm::vec3 skew;
	glm::vec4 perspective;
	glm::decompose(m_WorldModelMatrix, m_WorldScale, m_WorldRotation, m_WorldPosition, skew, perspective);

	m_Forward = Math::Transform(Math::ForwardVector, m_WorldRotation);
	m_Right = Math::Transform(Math::RightVector, m_WorldRotation);

	for (auto child : m_Children)
	{
		child->CalculateWorldModelMatrix();
	}
}
