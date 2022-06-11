#include "MeshComponent.h"

#include "Assets/AssetManager.h"
#include "Importer/MaterialImporter.h"
#include "Camera/CameraManager.h"

#include "Scene/Actor.h"
#include "Scene/Scene.h"
#include "Light/PointLight.h"
#include "Light/SpotLight.h"
#include "Light/SkyLight.h"

#include <glad/glad.h>

MeshComponent::MeshComponent(Actor* owner)
	: MeshComponent(owner, "Models/defaults/default_cube.obj")
{
}

MeshComponent::MeshComponent(Actor* owner, std::string path)
	: RenderComponent(owner), m_Path(path)
{
	m_CastShadow = true;
}

MeshComponent::MeshComponent(Actor* owner, std::string path, std::vector<std::string> materialsPaths)
	: RenderComponent(owner), m_Path(path), m_MaterialsPaths(materialsPaths)
{
	m_CastShadow = true;
}

void MeshComponent::Start()
{
	m_Owner->GetTransform()->OnTransformChanged.Add(&MeshComponent::UpdateBoundingBox, this);
    m_Owner->GetTransform()->OnTransformChanged.Add(&MeshComponent::UpdateBoundingSphere, this);
	
	UpdateBoundingBox();
    UpdateBoundingSphere();
}

void MeshComponent::Update(float deltaTime)
{
}

void MeshComponent::PreRender()
{
}

void MeshComponent::Render(Material::BlendMode blendMode)
{

}

void MeshComponent::Destroy()
{
}

void MeshComponent::LoadMaterial(std::string path)
{
	m_Materials.push_back(AssetManager::LoadMaterial(path));
	m_MaterialsPaths.push_back(path);
}

void MeshComponent::ChangeMaterial(int index, std::string path)
{
	m_MaterialsPaths.at(index) = path;
	m_Materials.at(index) = AssetManager::LoadMaterial(path);
}

void MeshComponent::ChangeMaterial(int index, Ref<Material> material)
{
	m_Materials.at(index) = material;
}

void MeshComponent::ChangeMaterials(std::vector<Ref<Material>> materials)
{
	m_Materials.clear();
	m_MaterialsPaths.clear();

	m_Materials = materials;
	for (auto material : m_Materials)
		m_MaterialsPaths.push_back(material->GetPath());
}
