#include "Ghost.h"

#include "Scene/Actor.h"
#include "Scene/Scene.h"
#include "Scene/Component/TransformComponent.h"
#include "Player.h"
#include "SaveManager.h"
#include "Scene/Component/MeshComponent.h"
#include "Material/MaterialInstance.h"
#include "Trail.h"
#include "Math/Math.h"

#include <glm/gtx/matrix_decompose.hpp>

Ghost::Ghost(Actor* owner)
	: GameComponent(owner)
{
	for (int i = 0; i < GHOST_POSITIONS_COUNT; i++)
		m_Positions[i] = glm::vec3(0.0f);

	m_PositionOffset = glm::vec3(0.0f, 0.5f, 0.0f);
	m_NormalEmissiveColor = glm::vec3(1.0f);
	m_CorruptedEmissiveColor = glm::vec3(0.7f, 0.0f, 0.0f);

	m_CurrentPositionIndex = 0;
	m_FollowPlayer = false;
	m_IsCorrupted = false;
}

void Ghost::Start()
{
	m_PlayerActor = m_Owner->GetScene()->FindActor(m_PlayerID);
	if (!m_PlayerActor)
	{
		ENGINE_WARN("PlayerActor is null!");
		return;
	}

	if (auto collider = m_Owner->GetComponent<ColliderComponent>())
		collider->OnTriggerEnterDelegate.Add(&Ghost::OnTriggerEnter, this);

	m_Positions[m_CurrentPositionIndex] = m_PlayerActor->GetTransform()->GetWorldPosition() + m_PositionOffset;

	if (auto mesh = m_Owner->GetComponent<MeshComponent>())
	{
		Ref<Material> material = mesh->GetMaterials()[0];
		m_Material = MaterialInstance::Create(material);

		mesh->SetMaterial(0, m_Material);

		m_NormalEmissiveColor = m_Material->GetVec3Parameter("u_Material.emissive");
	}
}

void Ghost::Update(float deltaTime)
{
	if (m_CurrentPositionIndex == GHOST_POSITIONS_COUNT)
	{
		m_CurrentPositionIndex = 0;
		m_FollowPlayer = true;
	}

	if (m_FollowPlayer)
	{
		m_Owner->GetTransform()->SetWorldPosition(m_Positions[m_CurrentPositionIndex]);

		if (m_CurrentPositionIndex < GHOST_POSITIONS_COUNT - 1)
		{
			glm::vec3 currentPosition = m_Positions[m_CurrentPositionIndex];
			glm::vec3 nextPosition = m_Positions[m_CurrentPositionIndex + 1];
			if (currentPosition.x != nextPosition.x || currentPosition.z != nextPosition.z)
			{
				glm::mat4 rotationMatrix = glm::lookAt(currentPosition, nextPosition, glm::vec3(0.0f, 1.0f, 0.0f));

				glm::vec3 pos, scale, skew;
				glm::vec4 perspective;
				glm::quat rot;
				glm::decompose(rotationMatrix, scale, rot, pos, skew, perspective);

				glm::vec3 euler = glm::degrees(glm::eulerAngles(rot));

				auto rotation = m_Owner->GetTransform()->GetLocalRotation();
				rotation.y = euler.y;
				m_Owner->GetTransform()->SetLocalRotation(rotation);
			}
		}
	}

	m_Positions[m_CurrentPositionIndex] = m_PlayerActor->GetTransform()->GetWorldPosition() + m_PositionOffset;
	m_CurrentPositionIndex++;
}

void Ghost::OnTriggerEnter(ColliderComponent* other)
{
	if (other->GetOwner()->GetComponent<Player>() && m_IsCorrupted)
		SaveManager::GetInstance()->LoadGame();
}

void Ghost::Corrupt()
{
	m_IsCorrupted = true;

	m_Material->SetVec3Parameter("u_Material.emissive", m_CorruptedEmissiveColor);

	glm::vec4 startColor = glm::vec4(2.0f, 0.5f, 0.5f, 0.5f);
	glm::vec4 endColor = glm::vec4(2.0f, 0.5f, 0.5f, 0.0f);
	m_PlayerActor->GetComponent<Player>()->GetTrail()->ChangeTrailParticlesColor(startColor, endColor);
}

void Ghost::Heal()
{
	m_IsCorrupted = false;

	m_Material->SetVec3Parameter("u_Material.emissive", m_NormalEmissiveColor);
	m_PlayerActor->GetComponent<Player>()->GetTrail()->SetDefaultTrailParticlesColor();
}
