#include "Entity.h"

#include "Scene.h"

Ref<Entity> Entity::Create(Scene* scene, std::string name)
{
	return CreateRef<Entity>(scene, name);
}

Ref<Entity> Entity::Create(Scene* scene, uint64_t id, std::string name)
{
	return CreateRef<Entity>(scene, id, name);
}

Entity::Entity(Scene* scene, std::string name)
	: m_Scene(scene), m_Name(name), m_Transform(Transform(this))
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution distribution(1, 999999999);

	m_ID = distribution(gen);

	m_Parent = nullptr;
}

Entity::Entity(Scene* scene, uint64_t id, std::string name)
	: m_Scene(scene), m_ID(id), m_Name(name), m_Transform(Transform(this))
{
	m_Parent = nullptr;
}

void Entity::Begin()
{
	for (auto component : m_Components)
	{
		component->Begin();
	}
}

void Entity::Update()
{
	for (auto component : m_Components)
	{
		component->Update();
	}
}

void Entity::PreRender()
{
	for (auto component : m_Components)
	{
		if (auto rc = Cast<RenderComponent>(component))
			rc->PreRender();
	}
}

void Entity::Render()
{
	for (auto component : m_Components)
	{
		if (auto rc = Cast<RenderComponent>(component))
			rc->Render();
	}
}

void Entity::Destroy()
{
	for (auto component : m_Components)
	{
		component->Destroy();
	}
}

void Entity::BeginPlay()
{
	for (auto component : m_Components)
	{
		if (auto igc = Cast<InGameComponent>(component))
			igc->BeginPlay();
	}
}

void Entity::Tick(float deltaTime)
{
	for (auto component : m_Components)
	{
		if (auto igc = Cast<InGameComponent>(component))
			igc->Tick(deltaTime);
	}
}

void Entity::EndPlay()
{
	for (auto component : m_Components)
	{
		if (auto igc = Cast<InGameComponent>(component))
			igc->EndPlay();
	}
}

void Entity::SetEnable(bool enable)
{
	m_Enable = enable;
}

void Entity::SetParent(Entity* parent)
{
	if (m_Parent)
		m_Parent->m_Children.erase(std::remove(m_Parent->m_Children.begin(), m_Parent->m_Children.end(), this));

	m_Parent = parent;
	m_Parent->m_Children.emplace_back(this);

	CalculateModelMatrix();
}

void Entity::SetLocalPosition(glm::vec3 position)
{
	m_Transform.LocalPosition = position;
	CalculateModelMatrix();

	m_Scene->SetChangedSinceLastFrame(true);
}

void Entity::SetLocalRotation(glm::vec3 rotation)
{
	m_Transform.LocalRotation = rotation;
	CalculateModelMatrix();

	m_Scene->SetChangedSinceLastFrame(true);
}

void Entity::SetLocalScale(glm::vec3 scale)
{
	m_Transform.LocalScale = scale;
	CalculateModelMatrix();

	m_Scene->SetChangedSinceLastFrame(true);
}

void Entity::SetID(uint64_t id)
{
	m_ID = id;
}

glm::vec3 Entity::GetWorldPosition()
{
	return m_Transform.LocalPosition + (m_Parent ? m_Parent->GetWorldPosition() : glm::vec3(0.0f));
}

glm::vec3 Entity::GetWorldRotation()
{
	return m_Transform.LocalRotation + (m_Parent ? m_Parent->GetWorldRotation() : glm::vec3(0.0f));
}

void Entity::SetWorldPosition(glm::vec3 position)
{
	SetLocalPosition(position - (m_Parent ? m_Parent->GetWorldPosition() : glm::vec3(0.0f)));
}

void Entity::CalculateModelMatrix()
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
