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
	m_Transform = GetComponent<TransformBaseComponent>();

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

void Actor::FixedUpdate()
{
	for (auto component : m_Components)
		component->FixedUpdate();
}

void Actor::PreRender()
{
	for (auto component : m_Components)
	{
		if (auto rc = Cast<RenderComponent>(component))
			rc->PreRender();
	}
}

void Actor::Render(Material::BlendMode blendMode)
{
	for (auto component : m_Components)
	{
		if (auto rc = Cast<RenderComponent>(component))
			rc->Render(blendMode);
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
	return m_Transform;
}

void Actor::SetDynamic(bool dynamic)
{
	m_Dynamic = dynamic;
}

void Actor::SetEnabled(bool enabled)
{
	m_Enabled = enabled;
}

void Actor::SetID(uint64_t id)
{
	m_ID = id;
}
