#include "SkeletalMeshComponent.h"

#include "Importer/SkeletalMeshImporter.h"
#include "Importer/MaterialImporter.h"
#include "Content/ContentHelper.h"
#include "Camera/CameraManager.h"

#include "Scene/Actor.h"
#include "Scene/Scene.h"

#include <glad/glad.h>

SkeletalMeshComponent::SkeletalMeshComponent(Actor* owner)
	: SkeletalMeshComponent(owner, "Models/defaults/Skeletal/SK_Walking.fbx")
{
}

SkeletalMeshComponent::SkeletalMeshComponent(Actor* owner, std::string path)
	: MeshComponent(owner, path)
{
	LoadMesh(path);

	for (int i = 0; i < m_Meshes.size(); i++)
		LoadMaterial("Materials/Default.mat");
}

SkeletalMeshComponent::SkeletalMeshComponent(Actor* owner, std::string path, std::vector<std::string> materialsPaths)
	: MeshComponent(owner, path, materialsPaths)
{
	LoadMesh(path);

	for (auto path : m_MaterialsPaths)
		m_Materials.push_back(MaterialImporter::GetInstance()->ImportMaterial(path));
}

void SkeletalMeshComponent::Render(Material::BlendMode blendMode)
{
	for (int i = 0; i < m_Meshes.size(); i++)
	{
		if (m_Materials.size() > i)
		{
			if (m_Materials.at(i)->GetBlendMode() != blendMode)
				continue;

			m_Materials.at(i)->Use();
			m_Materials.at(i)->GetShader()->SetMat4("u_Model", m_Owner->GetTransform()->GetWorldModelMatrix());

		}

		m_Meshes.at(i)->Render();
	}
}

std::vector<Ref<Mesh>> SkeletalMeshComponent::GetMeshes() const
{
	std::vector<Ref<Mesh>> result;
	for (auto mesh : m_Meshes)
		result.push_back(mesh);

	return result;
}

uint32_t SkeletalMeshComponent::GetRenderedVerticesCount()
{
	uint32_t vertices = 0;
	for (auto mesh : m_Meshes)
	{
		vertices += mesh->GetVertices().size();
	}

	return vertices;
}

uint32_t SkeletalMeshComponent::GetBoneCount()
{
	uint32_t bones = 0;
	for (auto mesh : m_Meshes)
	{
		bones += mesh->GetBoneCount();
	}

	return bones;
}

void SkeletalMeshComponent::LoadMesh(std::string path)
{
	m_Path = path;
	m_Meshes = SkeletalMeshImporter::GetInstance()->ImportMesh(path);
}

void SkeletalMeshComponent::ChangeMesh(std::string path)
{
	LoadMesh(path);

	m_Materials.clear();
	m_MaterialsPaths.clear();

	for (int i = 0; i < m_Meshes.size(); i++)
		LoadMaterial("Materials/Default.mat");
}

void SkeletalMeshComponent::UpdateBoundingBox()
{
	std::vector<glm::vec3> pointsFromAllMeshes;
	for (auto mesh : m_Meshes)
	{
		auto points = mesh->GetBoundingBox().GetPoints();
		for (auto& point : points)
		{
			point = m_Owner->GetTransform()->GetWorldModelMatrix() * glm::vec4(point, 1.0f);
			pointsFromAllMeshes.push_back(point);
		}
	}

	m_BoundingBox = BoundingBox(pointsFromAllMeshes);
}

void SkeletalMeshComponent::UpdateBoundingSphere() {
	BoundingSphere s0, s1, s = m_Meshes[0]->GetBoundingSphere();
	glm::vec3 d;
	float dist2, dist, r;

	s.Center = m_Owner->GetTransform()->GetWorldModelMatrix() * glm::vec4(s.Center, 1.0f);
	s.Radius *= m_Owner->GetTransform()->GetWorldModelMatrix()[0][0];

	for (auto mesh : m_Meshes)
	{
		s0 = s;
		s1 = mesh->GetBoundingSphere();
		s1.Center = m_Owner->GetTransform()->GetWorldModelMatrix() * glm::vec4(s1.Center, 1.0f);
		s1.Radius *= m_Owner->GetTransform()->GetWorldModelMatrix()[0][0];

		d = s1.Center - s0.Center;
		dist2 = glm::dot(d, d);

		r = s1.Radius - s0.Radius;

		if ((r * r) >= dist2) {
			if (s1.Radius >= s0.Radius)
				s = s1;
		}
		else {
			dist = glm::sqrt(dist2);
			s.Radius = (dist + s0.Radius + s1.Radius) * 0.5f;
			s.Center = s0.Center;
			if (dist > (s0.Radius + s1.Radius))
				s.Center += ((s.Radius - s0.Center) / dist) * d;
		}

	}

	m_BoundingSphere = s;
}
