#include "Ghost.h"

#include "Scene/Actor.h"
#include "Scene/Scene.h"
#include "Scene/Component/TransformComponent.h"
#include "Game/CharacterController.h"
#include "Player.h"
#include "SaveManager.h"
#include "Scene/Component/MeshComponent.h"
#include "Material/MaterialInstance.h"
#include "Scene/Component/Animation/Animator.h"
#include "Trail.h"
#include "Math/Math.h"
#include "Renderer/RenderPass/PostProcessingPass.h"
#include "TPPPlayer.h"
#include "TPPCharacterController.h"

#include <glm/gtx/matrix_decompose.hpp>

Ghost::Ghost(Actor* owner)
	: GameComponent(owner)
{
	for (int i = 0; i < GHOST_POSITIONS_COUNT; i++)
	{
		m_Positions[i] = glm::vec3(0.0f);
		m_RotationsY[i] = 0.0f;
		m_MovementSpeed[i] = 0.f;
		m_IsGrounded[i] = false;
		m_IsLanding[i] = false;
		m_IsJumping[i] = false;
		m_IsDashing[i] = false;
	}
	
	m_PositionOffset = glm::vec3(0.0f, -2.0f, 0.0f);
	m_NormalEmissiveColor = glm::vec3(1.0f);
	m_CorruptedEmissiveColor = glm::vec3(0.7f, 0.0f, 0.0f);
	m_DefaultVignetteColor = glm::vec3(0.0f);
	m_CorruptedVignetteColor = glm::vec3(1.0f, 0.0f, 0.0f);

	m_CurrentPositionIndex = 0;
	m_FollowPlayer = false;
	m_IsCorrupted = false;
}

void Ghost::Start()
{
	m_PlayerActor = m_Owner->GetScene()->FindActor(m_PlayerID);
	if (auto collider = m_Owner->GetComponent<ColliderComponent>())
		collider->OnTriggerEnterDelegate.Add(&Ghost::OnTriggerEnter, this);

	m_Animator = m_Owner->GetComponent<Animator>();

	m_Positions[m_CurrentPositionIndex] = m_PlayerActor->GetTransform()->GetWorldPosition() + m_PositionOffset;

	if (auto mesh = m_Owner->GetComponent<MeshComponent>())
	{
		Ref<Material> material = mesh->GetMaterials()[0];
		m_Material = MaterialInstance::Create(material);

		mesh->SetMaterial(0, m_Material);

		m_NormalEmissiveColor = m_Material->GetVec3Parameter("u_Material.emissive");
	}

	m_DefaultVignetteColor = Renderer::GetInstance()->m_PostProcessingPass->GetSettings().VignetteColor;
}

void Ghost::Update(float deltaTime)
{
	if (m_FollowPlayer)
	{
		m_Owner->GetTransform()->SetWorldPosition(m_Positions[m_CurrentPositionIndex]);
		
		auto rot = m_Owner->GetTransform()->GetLocalRotation();
		rot.y = m_RotationsY[m_CurrentPositionIndex];
		m_Owner->GetTransform()->SetLocalRotation(rot);

		HandleAnimator();
	}

	m_RotationsY[m_CurrentPositionIndex] = m_PlayerActor->GetTransform()->GetLocalRotation().y - 180.0f;
	m_Positions[m_CurrentPositionIndex] = m_PlayerActor->GetTransform()->GetWorldPosition() + m_PositionOffset;

	auto player = m_PlayerActor->GetComponent<Player>();
	m_MovementSpeed[m_CurrentPositionIndex] = player->GetMovementSpeed();

	player->UpdateGhostAnimatorParams();

	if (++m_CurrentPositionIndex == GHOST_POSITIONS_COUNT)
	{
		m_CurrentPositionIndex = 0;
		m_FollowPlayer = true;
	}

	if (auto tppPlayer = Cast<TPPPlayer>(player))
		m_Material->SetFloatParameter("u_Material.disappearDistance", -1.0f);
	else
		m_Material->SetFloatParameter("u_Material.disappearDistance", 1.0f);

	m_Material->SetVec3Parameter("u_Material.playerPosition", m_PlayerActor->GetTransform()->GetWorldPosition());
}

void Ghost::OnTriggerEnter(ColliderComponent* other)
{
	if (other->GetOwner()->GetComponent<Player>() && m_IsCorrupted)
		SaveManager::GetInstance()->LoadGame();
}

const SaveData Ghost::Save()
{
	SaveData data;
	data.ActorID = m_Owner->GetID();
	data.BoolFields.insert({ "IsCorrupted", m_IsCorrupted });

	return data;
}

void Ghost::Load(const SaveData& data)
{
	m_IsCorrupted = data.BoolFields.find("IsCorrupted")->second;

	if (m_IsCorrupted)
		Corrupt();
	else
		Heal();
}

void Ghost::Corrupt()
{
	m_IsCorrupted = true;

	m_Material->SetVec3Parameter("u_Material.emissive", m_CorruptedEmissiveColor);

	glm::vec4 startColor = glm::vec4(2.0f, 0.5f, 0.5f, 0.5f);
	glm::vec4 endColor = glm::vec4(2.0f, 0.5f, 0.5f, 0.0f);
	m_PlayerActor->GetComponent<Player>()->GetTrail()->ChangeTrailParticlesColor(startColor, endColor);

	auto renderer = Renderer::GetInstance();
	auto temp = renderer->m_PostProcessingPass->GetSettings();
	temp.VignetteColor = m_CorruptedVignetteColor;

	renderer->m_PostProcessingPass->SetSettings(temp);
}

void Ghost::Heal()
{
	m_IsCorrupted = false;

	m_Material->SetVec3Parameter("u_Material.emissive", m_NormalEmissiveColor);
	m_PlayerActor->GetComponent<Player>()->GetTrail()->SetDefaultTrailParticlesColor();

	auto renderer = Renderer::GetInstance();
	auto temp = renderer->m_PostProcessingPass->GetSettings();
	temp.VignetteColor = m_DefaultVignetteColor;

	renderer->m_PostProcessingPass->SetSettings(temp);
}

void Ghost::UpdateAnimatorParams(bool grounded, bool landing, bool jumping, bool dashing, bool sprinting)
{
	m_IsGrounded[m_CurrentPositionIndex] = grounded;
	m_IsLanding[m_CurrentPositionIndex] = landing;
	m_IsJumping[m_CurrentPositionIndex] = jumping;
	m_IsDashing[m_CurrentPositionIndex] = dashing;
	m_IsSprinting[m_CurrentPositionIndex] = sprinting;
}

void Ghost::HandleAnimator()
{
	float speed = m_MovementSpeed[m_CurrentPositionIndex] * 7.0f;
	speed = glm::clamp(speed, 0.0f, 1.0f);

	m_Animator->SetFloatParameter("Speed", speed);
	m_Animator->SetBoolParameter("IsGrounded", m_IsGrounded[m_CurrentPositionIndex]);
	m_Animator->SetBoolParameter("IsLanding", m_IsLanding[m_CurrentPositionIndex]);
	m_Animator->SetBoolParameter("IsDashing", m_IsDashing[m_CurrentPositionIndex]);
	m_Animator->SetBoolParameter("IsJumping", m_IsJumping[m_CurrentPositionIndex]);
	m_Animator->SetBoolParameter("IsSprinting", m_IsSprinting[m_CurrentPositionIndex]);
}
