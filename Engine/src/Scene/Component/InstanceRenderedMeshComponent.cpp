#include "InstanceRenderedMeshComponent.h"

#include "Importer/MeshImporter.h"
#include "Importer/MaterialImporter.h"

#include "Scene/Entity.h"
#include "Scene/Scene.h"
#include "Light/PointLight.h"
#include "Light/SpotLight.h"
#include "Light/SkyLight.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

InstanceRenderedMeshComponent::InstanceRenderedMeshComponent(Entity* owner)
	: InstanceRenderedMeshComponent(owner, "../../res/models/defaults/default_cube.obj")
{
}

InstanceRenderedMeshComponent::InstanceRenderedMeshComponent(Entity* owner, std::string path)
	: RenderComponent(owner), m_Path(path)
{
	LoadMesh(path);

	for (int i = 0; i < m_Meshes.size(); i++)
		LoadMaterial("../../res/materials/DefaultInstanced.mat");

	m_Radius = 1.0f;
	m_InstancesCount = 1;
	m_MinMeshScale = 1.0f;
	m_MaxMeshScale = 1.0f;

	Generate();
}

InstanceRenderedMeshComponent::InstanceRenderedMeshComponent(Entity* owner, std::string path, std::vector<std::string> materialsPaths)
	: RenderComponent(owner), m_Path(path), m_MaterialsPaths(materialsPaths)
{
	LoadMesh(path);

	for (auto path : m_MaterialsPaths)
		m_Materials.push_back(MaterialImporter::GetInstance()->ImportMaterial(path));

	m_Radius = 1.0f;
	m_InstancesCount = 1;
	m_MinMeshScale = 1.0f;
	m_MaxMeshScale = 1.0f;

	Generate();
}

void InstanceRenderedMeshComponent::Begin()
{
	
}

void InstanceRenderedMeshComponent::Update()
{
	
}

void InstanceRenderedMeshComponent::PreRender()
{
}

void InstanceRenderedMeshComponent::Render()
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

		material->GetShader()->SetMat4("u_Model", m_Owner->GetTransform().ModelMatrix);
	}
	if (!m_MultipleMaterials && m_Materials.at(0))
	{
		m_Materials.at(0)->Use();

		for (auto mesh : m_Meshes)
		{
			mesh.RenderInstanced(m_InstancesCount);
		}

		return;
	}

	for (int i = 0; i < m_Meshes.size(); i++)
	{
		if (m_Materials.size() > i)
			m_Materials.at(i)->Use();

		m_Meshes.at(i).RenderInstanced(m_InstancesCount);
	}

}

void InstanceRenderedMeshComponent::Destroy()
{
}

uint32_t InstanceRenderedMeshComponent::GetRenderedVerticesCount()
{
	uint32_t vertices = 0;
	for (auto mesh : m_Meshes)
	{
		vertices += mesh.vertices.size();
	}

	return vertices;
}

void InstanceRenderedMeshComponent::LoadMesh(std::string path)
{
	m_Path = path;
	m_Meshes = MeshImporter::GetInstance()->ImportMesh(path);
}

void InstanceRenderedMeshComponent::LoadMaterial(std::string path)
{
	m_Materials.push_back(MaterialImporter::GetInstance()->ImportMaterial(path));
	m_MaterialsPaths.push_back(path);
}

void InstanceRenderedMeshComponent::ChangeMesh(std::string path)
{
	LoadMesh(path);

	m_Materials.clear();
	m_MaterialsPaths.clear();

	for (int i = 0; i < m_Meshes.size(); i++)
		LoadMaterial("../../res/materials/DefaultInstanced.mat");
}

void InstanceRenderedMeshComponent::ChangeMaterial(int index, std::string path)
{
	m_MaterialsPaths.at(index) = path;
	m_Materials.at(index) = MaterialImporter::GetInstance()->ImportMaterial(path);
}

void InstanceRenderedMeshComponent::Generate()
{
	m_ModelMatrices.clear();

	glm::vec3 center = m_Owner->GetWorldPosition();

	srand(glfwGetTime());
	for (uint32_t i = 0; i < m_InstancesCount; i++)
	{
		glm::mat4 model = glm::mat4(1.0f);

		float theta = 2 * glm::pi<float>() * ((float)rand() / RAND_MAX);
		float distance = sqrt((float)rand() / RAND_MAX) * m_Radius;
		float x = center.x + distance * cos(theta);
		float z = center.z + distance * sin(theta);

		float scale = (rand() % (int)(m_MaxMeshScale * 100)) / 100.0f + m_MinMeshScale;
		float rotationY = (rand() % 360);

		Transform t = Transform(m_Owner);
		t.LocalPosition = glm::vec3(x, center.y, z);
		t.LocalScale = glm::vec3(scale);
		t.LocalRotation = glm::vec3(m_Owner->GetTransform().LocalRotation.x, rotationY, m_Owner->GetTransform().LocalRotation.z);
		t.CalculateModelMatrix();

		m_ModelMatrices.push_back(t.ModelMatrix);
	}

	if (m_ModelMatricesBuffer)
		glDeleteBuffers(1, &m_ModelMatricesBuffer);

	glGenBuffers(1, &m_ModelMatricesBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, m_ModelMatricesBuffer);
	glBufferData(GL_ARRAY_BUFFER, m_ModelMatrices.size() * sizeof(glm::mat4), &m_ModelMatrices[0], GL_STATIC_DRAW);

	for (int i = 0; i < m_Meshes.size(); i++)
	{
		glBindVertexArray(m_Meshes[i].GetVAO());

		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)0);

		glEnableVertexAttribArray(5);
		glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4)));

		glEnableVertexAttribArray(6);
		glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(2 * sizeof(glm::vec4)));

		glEnableVertexAttribArray(7);
		glVertexAttribPointer(7, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(3 * sizeof(glm::vec4)));

		glVertexAttribDivisor(4, 1);
		glVertexAttribDivisor(5, 1);
		glVertexAttribDivisor(6, 1);
		glVertexAttribDivisor(7, 1);

		glBindVertexArray(0);
	}
}
