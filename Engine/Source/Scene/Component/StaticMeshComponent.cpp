#include "StaticMeshComponent.h"

#include "Importer/MeshImporter.h"
#include "Importer/MaterialImporter.h"
#include "Content/ContentHelper.h"

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
	glm::vec3 min = m_Meshes.at(0).GetBoundingBox().Min;
	glm::vec3 max = m_Meshes.at(0).GetBoundingBox().Max;
	for (auto mesh : m_Meshes)
	{
		min = glm::min(min, mesh.GetBoundingBox().Min);
		max = glm::max(min, mesh.GetBoundingBox().Max);
	}

	m_BoundingBox = BoundingBox(m_Owner->GetTransform().ModelMatrix * glm::vec4(min, 1.0f), 
		m_Owner->GetTransform().ModelMatrix * glm::vec4(max, 1.0f));

	std::cout << m_Owner->GetName() << ": MIN = [" << m_BoundingBox.Min.x << ", " << m_BoundingBox.Min.y << ", " << m_BoundingBox.Min.z << "]" << std::endl;
	std::cout << m_Owner->GetName() << ": MAX = [" << m_BoundingBox.Max.x << ", " << m_BoundingBox.Max.y << ", " << m_BoundingBox.Max.z << "]" << std::endl;
	std::cout << m_Owner->GetName() << ": CENTER = [" << m_BoundingBox.Center.x << ", " << m_BoundingBox.Center.y << ", " << m_BoundingBox.Center.z << "]" << std::endl;
}

void StaticMeshComponent::Update(float deltaTime)
{
	//m_BoundingBox = BoundingBox(m_Owner->GetTransform().ModelMatrix * glm::vec4(m_BoundingBox.Min, 1.0f),
	//	m_Owner->GetTransform().ModelMatrix * glm::vec4(m_BoundingBox.Max, 1.0f));
}

void StaticMeshComponent::PreRender()
{
}

void StaticMeshComponent::Render()
{
	uint32_t irradianceMap;
	uint32_t prefilterMap;
	uint32_t BRDFLUT;
	float intensity;
	bool isSkyLight = false;

	auto components = m_Owner->GetScene()->GetComponents<SkyLight>();
	if (components.size() > 0)
	{
		if (auto skyLight = Cast<SkyLight>(components[0]))
		{
			isSkyLight = true;
			intensity = skyLight->GetIntensity();

			irradianceMap = skyLight->GetIrradianceMap();
			prefilterMap = skyLight->GetPrefilterMap();
			BRDFLUT = skyLight->GetBRDFLUT();
		}

	}

	for (auto material : GetMaterials())
	{
		material->Use();

		material->GetShader()->SetBool("u_IsSkyLight", isSkyLight);

		if (isSkyLight)
		{
			material->GetShader()->SetFloat("u_SkyLightIntensity", intensity);

			glActiveTexture(GL_TEXTURE0 + 20);
			glBindTexture(GL_TEXTURE_CUBE_MAP, irradianceMap);
			material->GetShader()->SetInt("u_IrradianceMap", 20);
			glActiveTexture(GL_TEXTURE0 + 21);
			glBindTexture(GL_TEXTURE_CUBE_MAP, prefilterMap);
			material->GetShader()->SetInt("u_PrefilterMap", 21);
			glActiveTexture(GL_TEXTURE0 + 22);
			glBindTexture(GL_TEXTURE_2D, BRDFLUT);
			material->GetShader()->SetInt("u_BRDFLUT", 22);
		}
		else
		{
			glActiveTexture(GL_TEXTURE0 + 20);
			glBindTexture(GL_TEXTURE_CUBE_MAP, Renderer::GetInstance()->GetPointLightShadowMapPlaceholder(0));
			material->GetShader()->SetInt("u_IrradianceMap", 20);
			glActiveTexture(GL_TEXTURE0 + 21);
			glBindTexture(GL_TEXTURE_CUBE_MAP, Renderer::GetInstance()->GetPointLightShadowMapPlaceholder(0));
			material->GetShader()->SetInt("u_PrefilterMap", 21);
			glActiveTexture(GL_TEXTURE0 + 22);
			glBindTexture(GL_TEXTURE_2D, Renderer::GetInstance()->GetSpotLightShadowMapPlaceholder(0));
			material->GetShader()->SetInt("u_BRDFLUT", 22);
		}

		glActiveTexture(GL_TEXTURE0 + 23);
		glBindTexture(GL_TEXTURE_2D, Renderer::GetInstance()->GetDirectionalLightShadowMapFramebuffer()->GetDepthAttachment());
		material->GetShader()->SetInt("u_DirectionalLightShadowMap", 23);

		auto pointLights = m_Owner->GetScene()->GetComponents<PointLight>();
		for (int i = 0; i < MAX_POINT_LIGHTS; i++)
		{
			if (i < pointLights.size())
			{
				auto shadowMap = Cast<PointLight>(pointLights[i])->GetShadowMap();

				glActiveTexture(GL_TEXTURE0 + 24 + i);
				glBindTexture(GL_TEXTURE_CUBE_MAP, shadowMap);
				material->GetShader()->SetInt("u_PointLightShadowMaps[" + std::to_string(i) + "]", 24 + i);
			}
			else
			{
				glActiveTexture(GL_TEXTURE0 + 24 + i);
				glBindTexture(GL_TEXTURE_CUBE_MAP, Renderer::GetInstance()->GetPointLightShadowMapPlaceholder(i));
				material->GetShader()->SetInt("u_PointLightShadowMaps[" + std::to_string(i) + "]", 24 + i);
			}
		}

		auto spotLights = m_Owner->GetScene()->GetComponents<SpotLight>();
		for (int i = 0; i < MAX_SPOT_LIGHTS; i++)
		{
			if (i < spotLights.size())
			{
				auto shadowMap = Cast<SpotLight>(spotLights[i])->GetShadowMap();

				glActiveTexture(GL_TEXTURE0 + 24 + MAX_POINT_LIGHTS + i);
				glBindTexture(GL_TEXTURE_2D, shadowMap);
				material->GetShader()->SetInt("u_SpotLightShadowMaps[" + std::to_string(i) + "]", 24 + MAX_POINT_LIGHTS + i);
			}
			else
			{
				glActiveTexture(GL_TEXTURE0 + 24 + MAX_POINT_LIGHTS + i);
				glBindTexture(GL_TEXTURE_2D, Renderer::GetInstance()->GetSpotLightShadowMapPlaceholder(i));
				material->GetShader()->SetInt("u_SpotLightShadowMaps[" + std::to_string(i) + "]", 24 + MAX_POINT_LIGHTS + i);
			}
		}


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