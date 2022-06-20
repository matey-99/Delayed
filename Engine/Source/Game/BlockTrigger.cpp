#include <Assets/AssetManager.h>
#include "BlockTrigger.h"

#include "Scene/Actor.h"
#include "Game/Player.h"
#include "Game/Ghost.h"
#include "Scene/Scene.h"
#include "Scene/Component/StaticMeshComponent.h"

BlockTrigger::BlockTrigger(Actor* owner)
	: GameComponent(owner)
{
    m_GhostMaterial = AssetManager::LoadMaterial("Materials/M_GhostObject.mat");
    m_SolidMaterial = AssetManager::LoadMaterial("Materials/GhostBox.mat");
}

BlockTrigger::~BlockTrigger()
{
}

void BlockTrigger::Start()
{
	if (auto collider = m_Owner->GetComponent<ColliderComponent>())
	{
		collider->OnTriggerEnterDelegate.Add(&BlockTrigger::OnTriggerEnter, this);
		collider->OnTriggerExitDelegate.Add(&BlockTrigger::OnTriggerExit, this);
	}

	m_ConnectedBlockCollider = m_Owner->GetTransform()->GetParent()->GetOwner()->GetComponent<ColliderComponent>();
}

void BlockTrigger::Update(float deltaTime)
{

}

void BlockTrigger::Destroy()
{
}

void BlockTrigger::OnTriggerEnter(ColliderComponent* other)
{
	if (other->GetOwner()->GetComponent<Ghost>())
	{
		m_ConnectedBlockCollider->SetTrigger(false);
		m_ConnectedBlockCollider->GetOwner()->GetComponent<StaticMeshComponent>()->ChangeMaterial(0, m_SolidMaterial);
	}
}

void BlockTrigger::OnTriggerExit(ColliderComponent* other)
{
	if (other->GetOwner()->GetComponent<Ghost>())
	{
		m_ConnectedBlockCollider->SetTrigger(true);
		m_ConnectedBlockCollider->GetOwner()->GetComponent<StaticMeshComponent>()->ChangeMaterial(0, m_GhostMaterial);
	}
}
