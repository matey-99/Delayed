#include "Platform.h"

#include "Scene/Actor.h"
#include "Scene/Component/TransformComponent.h"
#include "Math/Math.h"

Platform::Platform(Actor* owner)
	: GameComponent(owner)
{
	m_Active = false;
	m_Direction = glm::vec3(0.0f, 1.0f, 0.0f);
	m_DefaultPosition = glm::vec3(0.0f);
	m_Distance = 5.0f;
	m_Speed = 5.0f;
}

Platform::~Platform()
{
}

void Platform::Start()
{
	m_DefaultPosition = m_Owner->GetTransform()->GetWorldPosition();
}

void Platform::Update(float deltaTime)
{
	float step = m_Speed * deltaTime;
	glm::vec3 currentPosition = m_Owner->GetTransform()->GetWorldPosition();
	if (m_Active)
	{
		if (!Math::IsNearlyEqual(currentPosition, m_DefaultPosition + (m_Direction * m_Distance), step))
		{
			glm::vec3 newPosition = m_Owner->GetTransform()->GetLocalPosition();
			newPosition += m_Direction * step;
			m_Owner->GetTransform()->SetLocalPosition(newPosition);
		}
	}
	else
	{
		if (!Math::IsNearlyEqual(currentPosition, m_DefaultPosition, step))
		{
			glm::vec3 newPosition = m_Owner->GetTransform()->GetLocalPosition();
			newPosition -= m_Direction * step;
			m_Owner->GetTransform()->SetLocalPosition(newPosition);
		}
	}
}

void Platform::FixedUpdate()
{
}
