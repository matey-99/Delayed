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
#include "Renderer/RenderPass/ShadowsPass.h"

#include <glad/glad.h>

StaticMeshComponent::StaticMeshComponent(Actor* owner)
	: StaticMeshComponent(owner, "Models/defaults/default_cube.obj")
{
}

StaticMeshComponent::StaticMeshComponent(Actor* owner, std::string path)
	: MeshComponent(owner, path)
{
	LoadMesh(path);

	for (int i = 0; i < m_Meshes.size(); i++)
		LoadMaterial("Materials/Default.mat");
}

StaticMeshComponent::StaticMeshComponent(Actor* owner, std::string path, std::vector<std::string> materialsPaths)
	: MeshComponent(owner, path, materialsPaths)
{
	LoadMesh(path);

	for (auto path : m_MaterialsPaths)
		m_Materials.push_back(MaterialImporter::GetInstance()->ImportMaterial(path));
}

void StaticMeshComponent::Render(Material::BlendMode blendMode)
{
	for (int i = 0; i < m_Meshes.size(); i++)
	{
		if (m_Materials.size() > i)
		{
			if (m_Materials.at(i)->GetBlendMode() != blendMode)
				continue;

			m_Materials.at(i)->Use();
			m_Materials.at(i)->GetShader()->SetMat4("u_Model", m_Owner->GetTransform()->GetWorldModelMatrix());

			if (blendMode == Material::BlendMode::Transparent)
			{
				auto s = Renderer::GetInstance()->m_ShadowsPass;
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D_ARRAY, s->GetDirectionalLightRenderTarget()->GetTargets()[0]);

				m_Materials.at(i)->GetShader()->SetInt("u_DirectionalLightShadowMaps", 0);

				if (auto skyLight = m_Owner->GetScene()->FindComponent<SkyLight>())
				{
					m_Materials.at(i)->GetShader()->SetVec3("u_SkyLightColor", skyLight->GetColor());
					m_Materials.at(i)->GetShader()->SetFloat("u_SkyLightIntensity", skyLight->GetIntensity());
				}
				else
				{
					m_Materials.at(i)->GetShader()->SetVec3("u_SkyLightColor", glm::vec3(1.0f));
					m_Materials.at(i)->GetShader()->SetFloat("u_SkyLightIntensity", 0.03f);
				}
			}
		}

		m_Meshes.at(i)->Render();
	}
}

std::vector<Ref<Mesh>> StaticMeshComponent::GetMeshes() const
{
	std::vector<Ref<Mesh>> result;
	for (auto mesh : m_Meshes)
		result.push_back(mesh);

	return result;
}

uint32_t StaticMeshComponent::GetRenderedVerticesCount()
{
	uint32_t vertices = 0;
	for (auto mesh : m_Meshes)
	{
		vertices += mesh->GetVertices().size();
	}

	return vertices;
}

void StaticMeshComponent::LoadMesh(std::string path)
{
	m_Path = path;
	m_Meshes = MeshImporter::GetInstance()->ImportMesh(path);
}

void StaticMeshComponent::ChangeMesh(std::string path)
{
	LoadMesh(path);

	m_Materials.clear();
	m_MaterialsPaths.clear();

	for (int i = 0; i < m_Meshes.size(); i++)
		LoadMaterial("Materials/Default.mat");
}

void StaticMeshComponent::UpdateBoundingBox()
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

//TODO Potentially can be made more efficient
void StaticMeshComponent::UpdateBoundingSphere() {
	BoundingSphere s0, s1, s = m_Meshes[0]->GetBoundingSphere();
	glm::vec3 d;
	float dist2, dist, r;

	s.Center = m_Owner->GetTransform()->GetWorldModelMatrix() * glm::vec4(s.Center, 1.0f);
	s.Radius *= m_Owner->GetTransform()->GetWorldScale().x;

	for (auto mesh : m_Meshes)
	{
		s0 = s;
		s1 = mesh->GetBoundingSphere();
		s1.Center = m_Owner->GetTransform()->GetWorldModelMatrix() * glm::vec4(s1.Center, 1.0f);
		s1.Radius *= m_Owner->GetTransform()->GetWorldScale().x;

		d = s1.Center - s0.Center;
		dist2 = glm::dot(d, d);

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
