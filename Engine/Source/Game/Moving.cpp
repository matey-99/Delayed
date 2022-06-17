#include "Moving.h"

#include "Scene/Actor.h"
#include "Scene/Component/TransformComponent.h"
#include "Math/Math.h"
#include "Scene/Component/StaticMeshComponent.h"

Moving::Moving(Actor* owner)
	: GameComponent(owner)
{
	m_Direction = glm::vec3(0.0f, 1.0f, 0.0f);
	m_DefaultPosition = glm::vec3(0.0f);
	m_Distance = 5.0f;
	m_Smoothness = 0.95f;
	m_ErrorInReachingTarget = 0.01f;

	m_IsReturning = false;

	m_Velocity = glm::vec3(0.0f);
}

Moving::~Moving()
{
}

void Moving::Start()
{
	m_DefaultPosition = m_Owner->GetTransform()->GetWorldPosition();
	m_TargetPosition = m_DefaultPosition + (m_Direction * m_Distance);
}

void Moving::Update(float deltaTime)
{
	m_TargetPosition = m_DefaultPosition + (m_Direction * m_Distance);

	glm::vec3 currentPosition = m_Owner->GetTransform()->GetWorldPosition();

	if (!m_IsReturning)
	{
		if (!Math::IsNearlyEqual(currentPosition, m_TargetPosition, m_ErrorInReachingTarget))
		{
			glm::vec3 newPosition = Math::SmoothDamp(currentPosition, m_TargetPosition, m_Velocity, m_Smoothness, deltaTime);
			m_Owner->GetTransform()->SetWorldPosition(newPosition);
		}
		else
		{
			m_IsReturning = true;
		}
	}
	else
	{
		if (!Math::IsNearlyEqual(currentPosition, m_DefaultPosition, m_ErrorInReachingTarget))
		{
			glm::vec3 newPosition = Math::SmoothDamp(currentPosition, m_DefaultPosition, m_Velocity, m_Smoothness, deltaTime);
			m_Owner->GetTransform()->SetWorldPosition(newPosition);
		}
		else
		{
			m_IsReturning = false;
		}
	}
}