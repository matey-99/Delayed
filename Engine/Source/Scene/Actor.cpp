#include "Actor.h"

#include "Scene.h"

Ref<Actor> Actor::Create(Scene* scene, std::string name)
{
	return CreateRef<Actor>(scene, name);
}

Ref<Actor> Actor::Create(Scene* scene, uint64_t id, std::string name)
{
	return CreateRef<Actor>(scene, id, name);
}

Actor::Actor(Scene* scene, std::string name)
	: m_Scene(scene), m_Name(name), m_Transform(Transform(this))
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution distribution(1, 999999999);

	m_ID = distribution(gen);

	m_Parent = nullptr;
}

Actor::Actor(Scene* scene, uint64_t id, std::string name)
	: m_Scene(scene), m_ID(id), m_Name(name), m_Transform(Transform(this))
{
	m_Parent = nullptr;
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

void Actor::SetEnable(bool enable)
{
	m_Enable = enable;
}

void Actor::SetParent(Actor* parent)
{
	if (m_Parent)
		m_Parent->m_Children.erase(std::remove(m_Parent->m_Children.begin(), m_Parent->m_Children.end(), this));

	m_Parent = parent;
	m_Parent->m_Children.emplace_back(this);

	CalculateModelMatrix();
}

void Actor::SetLocalPosition(glm::vec3 position)
{
	m_Transform.LocalPosition = position;
	CalculateModelMatrix();

	m_Scene->SetChangedSinceLastFrame(true);
}

void Actor::SetLocalRotation(glm::vec3 rotation)
{
	m_Transform.LocalRotation = rotation;
	CalculateModelMatrix();

	m_Scene->SetChangedSinceLastFrame(true);
}

void Actor::SetLocalScale(glm::vec3 scale)
{
	m_Transform.LocalScale = scale;
	CalculateModelMatrix();

	m_Scene->SetChangedSinceLastFrame(true);
}

void Actor::SetID(uint64_t id)
{
	m_ID = id;
}

glm::vec3 Actor::GetWorldPosition()
{
	return m_Transform.LocalPosition + (m_Parent ? m_Parent->GetWorldPosition() : glm::vec3(0.0f));
}

glm::vec3 Actor::GetWorldRotation()
{
	return m_Transform.LocalRotation + (m_Parent ? m_Parent->GetWorldRotation() : glm::vec3(0.0f));
}

void Actor::SetWorldPosition(glm::vec3 position)
{
	SetLocalPosition(position - (m_Parent ? m_Parent->GetWorldPosition() : glm::vec3(0.0f)));
}

void Actor::CalculateModelMatrix()
{
	if (m_Parent)
		m_Transform.CalculateModelMatrix(m_Parent->GetTransform().ModelMatrix);
	else
		m_Transform.CalculateModelMatrix();

	for (auto child : m_Children)
	{
		child->CalculateModelMatrix();
	}
}
