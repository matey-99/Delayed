#include "Clouds.h"

#include "Scene/Actor.h"
#include "Scene/Scene.h"
#include "Scene/Component/StaticMeshComponent.h"
#include "Assets/AssetManager.h"
#include "Material/MaterialInstance.h"

Clouds::Clouds(Actor* owner)
	: GameComponent(owner)
{
	m_MaterialInstance = nullptr;

	m_MoveDirection = glm::vec3(0.0f, 0.0f, -1.0f);
	m_MoveSpeed = 1.0f;
}

Clouds::~Clouds()
{
}

void Clouds::Start()
{
	if (auto mesh = m_Owner->GetComponent<StaticMeshComponent>())
	{
		Ref<Material> material = mesh->GetMaterials()[0];
		m_MaterialInstance = MaterialInstance::Create(material);

		mesh->SetMaterial(0, m_MaterialInstance);
	}
}

void Clouds::Update(float deltaTime)
{
	auto transform = m_Owner->GetTransform();
	glm::vec3 boundsMin = transform->GetWorldPosition() - transform->GetLocalScale() / 2.0f;
	glm::vec3 boundsMax = transform->GetWorldPosition() + transform->GetLocalScale() / 2.0f;

	m_MaterialInstance->SetVec3Parameter("u_Material.BoundsMin", boundsMin);
	m_MaterialInstance->SetVec3Parameter("u_Material.BoundsMax", boundsMax);
	
	glm::vec3 offset = m_MaterialInstance->GetVec3Parameter("u_Material.CloudOffset");
	glm::vec3 delta = m_MoveDirection * m_MoveSpeed * deltaTime;
	offset += delta;

	m_MaterialInstance->SetVec3Parameter("u_Material.CloudOffset", offset);
}
