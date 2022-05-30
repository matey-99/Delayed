#include "FoliageComponent.h"

#include "Assets/AssetManager.h"
#include "Importer/MaterialImporter.h"
#include "Camera/CameraManager.h"

#include "Scene/Actor.h"
#include "Scene/Scene.h"
#include "Light/PointLight.h"
#include "Light/SpotLight.h"
#include "Light/SkyLight.h"
#include "Math/Transform.h"

#include <glad/glad.h>

FoliageComponent::FoliageComponent(Actor* owner)
	: RenderComponent(owner)
{
	m_InstancesCount = 100;
	m_Radius = 10.0f;
	m_MinInstanceScale = 1.0f;
	m_MaxInstanceScale = 1.0f;
	
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution distribution(1, 999999999);

	m_Seed = distribution(gen);

	ChangeMesh("Models/defaults/default_cube.obj");
	Generate();
}

void FoliageComponent::SetInstancesCount(uint32_t instancesCount)
{
	m_InstancesCount = instancesCount;
	Generate();
}

std::vector<Ref<Mesh>> FoliageComponent::GetMeshes() const
{
	std::vector<Ref<Mesh>> result;
	for (auto mesh : m_Model->GetMeshes())
		result.push_back(mesh);

	return result;
}

uint32_t FoliageComponent::GetRenderedVerticesCount()
{
	uint32_t vertices = 0;
	for (auto mesh : m_Model->GetMeshes())
	{
		vertices += mesh->GetVertices().size();
	}

	return vertices;
}

void FoliageComponent::Start()
{
	m_Owner->GetTransform()->OnTransformChanged.Add(&FoliageComponent::Generate, this);
}

void FoliageComponent::Update(float deltaTime)
{
}

void FoliageComponent::PreRender()
{
}

void FoliageComponent::Render(Material::BlendMode blendMode)
{

}

void FoliageComponent::Destroy()
{
}

void FoliageComponent::Generate()
{
	m_InstancesTransformations.clear();
	m_InstancesWorldPositions.clear();
	m_InstancesBoundingBoxes.clear();

	glm::vec3 center = m_Owner->GetTransform()->GetWorldPosition();
	glm::vec3 rotation = m_Owner->GetTransform()->GetLocalRotation();

	srand(m_Seed);
	for (uint32_t i = 0; i < m_InstancesCount; i++)
	{

		float theta = 2 * glm::pi<float>() * ((float)rand() / RAND_MAX);
		float distance = glm::sqrt((float)rand() / RAND_MAX) * m_Radius;
		float x = center.x + distance * glm::cos(theta);
		float z = center.z + distance * glm::sin(theta);

		float scale = m_MinInstanceScale + ((float)rand() / (RAND_MAX / (m_MaxInstanceScale - m_MinInstanceScale)));
		float rotationY = (rand() % 360);

		Transform t;
		t.Position = glm::vec3(x, center.y, z);
		t.Scale = glm::vec3(scale);
		t.Rotation = glm::vec3(rotation.x, rotationY, rotation.z);

		glm::mat4 transformation = t.GetModelMatrix();
		m_InstancesTransformations.push_back(transformation);
		m_InstancesWorldPositions.push_back(t.Position);

		std::vector<glm::vec3> pointsFromAllMeshes;
		for (auto mesh : m_Model->GetMeshes())
		{
			auto points = mesh->GetBoundingBox().GetPoints();
			for (auto& point : points)
			{
				point = transformation * glm::vec4(point, 1.0f);
				pointsFromAllMeshes.push_back(point);
			}
		}

		m_InstancesBoundingBoxes.push_back(BoundingBox(pointsFromAllMeshes));
	}
}

void FoliageComponent::LoadMaterial(std::string path)
{
	m_MaterialPath = path;
	m_Material = AssetManager::LoadMaterial(path);
}

void FoliageComponent::ChangeMaterial(std::string path)
{
	LoadMaterial(path);
}

void FoliageComponent::ChangeMaterial(Ref<Material> material)
{
	m_Material = material;
}

void FoliageComponent::LoadMesh(std::string path)
{
	m_Path = path;
	m_Model = AssetManager::LoadModel(path);
}

void FoliageComponent::ChangeMesh(std::string path)
{
	LoadMesh(path);
	LoadMaterial("Materials/Default.mat");
}

void FoliageComponent::ChangeModel(Ref<Model> model)
{
	m_Model = model;
}
