#include "Block.h"

#include "Scene/Actor.h"
#include "Game/Player.h"
#include "Game/Ghost.h"
#include "Scene/Scene.h"

Block::Block(Actor* owner)
	: GameComponent(owner)
{
	m_IsIn = false;
}

Block::~Block()
{
}

void Block::Start()
{
	if (auto collider = m_Owner->GetComponent<ColliderComponent>())
	{
		collider->OnTriggerEnterDelegate.Add(&Block::OnTriggerEnter, this);
		collider->OnTriggerExitDelegate.Add(&Block::OnTriggerExit, this);
	}


}

void Block::Update(float deltaTime)
{
	if (m_IsIn == true) {

	}
	else {
		//m_Owner->GetComponent<ColliderComponent>()->IsTrigger = false;
		//m_Owner->GetComponent<Mater>()->IsTrigger = false;
	}
	
}

void Block::Destroy()
{
}

void Block::OnTriggerEnter(Ref<ColliderComponent> other)
{
	if (other->GetOwner()->GetComponent<Ghost>())
	{
		m_IsIn = true;
	}
}

void Block::OnTriggerExit(Ref<ColliderComponent> other)
{
	if ( other->GetOwner()->GetComponent<Ghost>())
	{
		m_IsIn = false;
	}
}
