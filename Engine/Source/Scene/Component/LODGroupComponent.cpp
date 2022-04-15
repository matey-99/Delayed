#include "LODGroupComponent.h"

#include "Scene/Actor.h"
#include "StaticMeshComponent.h"
#include "Camera/CameraManager.h"
#include "Math/Math.h"

LODGroupComponent::LODGroupComponent(Actor* owner)
	: Component(owner)
{
	if (auto meshComp = m_Owner->GetComponent<MeshComponent>())
		m_MeshComponent = meshComp;
	else
		std::cout << "LODGroup error: There is no MeshComponent in actor with ID: " << m_Owner->GetID() << std::endl;
}

void LODGroupComponent::Start()
{

}

void LODGroupComponent::Update(float deltaTime)
{
	auto camera = CameraManager::GetInstance()->GetMainCamera();
	float distanceFromCamera = Math::Distance(m_Owner->GetTransform()->GetWorldPosition(), camera->GetWorldPosition());

	LOD newLOD = m_LODs[0];
	for (auto& lod : m_LODs)
	{
		if (distanceFromCamera > lod.MaxDistance)
			newLOD = lod;
	}
	
	if (newLOD != m_CurrentLOD)
	{
		m_MeshComponent->ChangeModel(newLOD.Model);
		m_MeshComponent->ChangeMaterials(newLOD.Materials);

		m_CurrentLOD = newLOD;
	}
}

void LODGroupComponent::Destroy()
{
}
