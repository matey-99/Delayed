#include "BlockTrigger.h"

#include "Scene/Actor.h"
#include "Game/Player.h"
#include "Game/Ghost.h"
#include "Scene/Scene.h"
#include "Scene/Component/StaticMeshComponent.h"

BlockTrigger::BlockTrigger(Actor* owner)
	: GameComponent(owner)
{
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
		m_ConnectedBlockCollider->GetOwner()->GetComponent<StaticMeshComponent>()->ChangeMaterial(0, "Materials/White.mat");
	}
}

void BlockTrigger::OnTriggerExit(ColliderComponent* other)
{
	if (other->GetOwner()->GetComponent<Ghost>())
	{
		m_ConnectedBlockCollider->SetTrigger(true);
		m_ConnectedBlockCollider->GetOwner()->GetComponent<StaticMeshComponent>()->ChangeMaterial(0, "Materials/M_Ghost.mat");
	}
}
