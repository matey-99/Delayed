#include "Actor.h"

#include "Scene.h"
#include "Component/TransformBaseComponent.h"
#include "Component/TransformComponent.h"
#include "Component/UI/RectTransformComponent.h"

Ref<Actor> Actor::Create(Scene* scene, std::string name)
{
	return CreateRef<Actor>(scene, name);
}

Ref<Actor> Actor::Create(Scene* scene, uint64_t id, std::string name)
{
	return CreateRef<Actor>(scene, id, name);
}

Actor::Actor(Scene* scene, std::string name)
	: m_Scene(scene), m_Name(name)
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution distribution(1, 999999999);

	m_ID = distribution(gen);
}

Actor::Actor(Scene* scene, uint64_t id, std::string name)
	: m_Scene(scene), m_ID(id), m_Name(name)
{
}

void Actor::Start()
{
	for (auto component : m_Components)
	{
		component->Start();
	}
}

void Actor::Update(float deltaTime)
{
	for (auto component : m_Components)
	{
		component->Update(deltaTime);
	}
}

void Actor::PreRender()
{
	for (auto component : m_Components)
	{
		if (auto rc = Cast<RenderComponent>(component))
			rc->PreRender();
	}
}

void Actor::Render()
{
	for (auto component : m_Components)
	{
		if (auto rc = Cast<RenderComponent>(component))
			rc->Render();
	}
}

void Actor::Destroy()
{
	for (auto component : m_Components)
	{
		component->Destroy();
	}
}

Ref<TransformBaseComponent> Actor::GetTransform()
{
	return GetComponent<TransformBaseComponent>();
}

void Actor::SetEnable(bool enable)
{
	m_Enable = enable;
}

void Actor::SetID(uint64_t id)
{
	m_ID = id;
}
