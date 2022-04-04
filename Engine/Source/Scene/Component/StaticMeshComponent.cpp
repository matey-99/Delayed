#include "StaticMeshComponent.h"

#include "Importer/MeshImporter.h"
#include "Importer/MaterialImporter.h"
#include "Content/ContentHelper.h"
#include "Camera/CameraManager.h"

#include "Scene/Actor.h"
#include "Scene/Scene.h"
#include "Light/PointLight.h"
#include "Light/SpotLight.h"
#include "Light/SkyLight.h"

#include <glad/glad.h>

StaticMeshComponent::StaticMeshComponent(Actor* owner)
	: StaticMeshComponent(owner, "Models/defaults/default_cube.obj")
{
}

StaticMeshComponent::StaticMeshComponent(Actor* owner, std::string path)
	: RenderComponent(owner), m_Path(path)
{
	LoadMesh(path);

	for (int i = 0; i < m_Meshes.size(); i++)
		LoadMaterial("Materials/Default.mat");
}

StaticMeshComponent::StaticMeshComponent(Actor* owner, std::string path, std::vector<std::string> materialsPaths)
	: RenderComponent(owner), m_Path(path), m_MaterialsPaths(materialsPaths)
{
	LoadMesh(path);

	for (auto path : m_MaterialsPaths)
		m_Materials.push_back(MaterialImporter::GetInstance()->ImportMaterial(path));

}

void StaticMeshComponent::Start()
{
	m_Owner->GetTransform()->OnTransformChanged.Add(&StaticMeshComponent::UpdateBoundingBox, this);
    m_Owner->GetTransform()->OnTransformChanged.Add(&StaticMeshComponent::UpdateBoundingSphere, this);
	
	UpdateBoundingBox();
    UpdateBoundingSphere();
}

void StaticMeshComponent::Update(float deltaTime)
{
}

void StaticMeshComponent::PreRender()
{
}

void StaticMeshComponent::Render()
{
	for (auto material : GetMaterials())
	{
		material->Use();
		material->GetShader()->SetMat4("u_Model", m_Owner->GetTransform()->GetWorldModelMatrix());
	}
	if (!m_MultipleMaterials && m_Materials.at(0))
	{
		m_Materials.at(0)->Use();

		for (auto mesh : m_Meshes)
		{
			mesh.Render();
		}

		return;
	}

	for (int i = 0; i < m_Meshes.size(); i++)
	{
		if (m_Materials.size() > i)
			m_Materials.at(i)->Use();

		m_Meshes.at(i).Render();
	}

}

void StaticMeshComponent::Destroy()
{
}

uint32_t StaticMeshComponent::GetRenderedVerticesCount()
{
	uint32_t vertices = 0;
	for (auto mesh : m_Meshes)
	{
		vertices += mesh.GetVertices().size();
	}

	return vertices;
}

void StaticMeshComponent::LoadMesh(std::string path)
{
	m_Path = path;
	m_Meshes = MeshImporter::GetInstance()->ImportMesh(path);
}

void StaticMeshComponent::LoadMaterial(std::string path)
{
	m_Materials.push_back(MaterialImporter::GetInstance()->ImportMaterial(path));
	m_MaterialsPaths.push_back(path);
}

void StaticMeshComponent::ChangeMesh(std::string path)
{
	LoadMesh(path);

	m_Materials.clear();
	m_MaterialsPaths.clear();

	for (int i = 0; i < m_Meshes.size(); i++)
		LoadMaterial("Materials/Default.mat");
}

void StaticMeshComponent::ChangeMaterial(int index, std::string path)
{
	m_MaterialsPaths.at(index) = path;
	m_Materials.at(index) = MaterialImporter::GetInstance()->ImportMaterial(path);
}

void StaticMeshComponent::UpdateBoundingBox()
{
	std::vector<glm::vec3> pointsFromAllMeshes;
	for (auto mesh : m_Meshes)
	{
		auto points = mesh.GetBoundingBox().GetPoints();
		for (auto& point : points)
		{
			point = m_Owner->GetTransform()->GetWorldModelMatrix() * glm::vec4(point, 1.0f);
			pointsFromAllMeshes.push_back(point);
		}
	}

	m_BoundingBox = BoundingBox(pointsFromAllMeshes);
}

void StaticMeshComponent::UpdateBoundingSphere() {
    BoundingSphere s0, s1, s = m_Meshes[0].GetBoundingSphere();
    glm::vec3 d;
    float dist2, dist, r;

    s.Center = m_Owner->GetTransform()->GetWorldModelMatrix() * glm::vec4(s.Center, 1.0f);
    s.Radius *= m_Owner->GetTransform()->GetWorldModelMatrix()[0][0];

    for (auto mesh : m_Meshes)
    {
        s0 = s;
        s1 = mesh.GetBoundingSphere();
        s1.Center = m_Owner->GetTransform()->GetWorldModelMatrix() * glm::vec4(s1.Center, 1.0f);
        s1.Radius *= m_Owner->GetTransform()->GetWorldModelMatrix()[0][0];

        d = s1.Center - s0.Center;
        dist2 = glm::dot(d,d);

        r = s1.Radius - s0.Radius;

        if ((r * r) >= dist2) {
            if (s1.Radius >= s0.Radius)
                s = s1;
        } else {
            dist = glm::sqrt(dist2);
            s.Radius = (dist + s0.Radius + s1.Radius) * 0.5f;
            s.Center = s0.Center;
            if (dist > (s0.Radius + s1.Radius))
                s.Center += ((s.Radius - s0.Center) / dist) * d;
        }

    }

    m_BoundingSphere = s;
}
