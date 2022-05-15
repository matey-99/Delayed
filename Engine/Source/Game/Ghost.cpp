#include "Ghost.h"

#include "Scene/Actor.h"
#include "Scene/Scene.h"
#include "Scene/Component/TransformComponent.h"

Ghost::Ghost(Actor* owner)
	: GameComponent(owner)
{
	for (int i = 0; i < GHOST_POSITIONS_COUNT; i++)
		m_Positions[i] = glm::vec3(0.0f);

	m_CurrentPositionIndex = 0;
	m_FollowPlayer = false;
}

void Ghost::Start()
{
	m_PlayerActor = m_Owner->GetScene()->FindActor(m_PlayerID);
	if (!m_PlayerActor)
	{
		WARN("PlayerActor is null!");
		return;
	}

	m_Positions[m_CurrentPositionIndex] = m_PlayerActor->GetTransform()->GetWorldPosition();
}

void Ghost::Update(float deltaTime)
{
	if (m_CurrentPositionIndex == GHOST_POSITIONS_COUNT)
	{
		m_CurrentPositionIndex = 0;
		m_FollowPlayer = true;
	}

	if (m_FollowPlayer)
		m_Owner->GetTransform()->SetWorldPosition(m_Positions[m_CurrentPositionIndex]);

	m_Positions[m_CurrentPositionIndex] = m_PlayerActor->GetTransform()->GetWorldPosition();
	m_CurrentPositionIndex++;
}
