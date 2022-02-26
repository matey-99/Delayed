#include "Scene.h"
#include "Component/StaticMeshComponent.h"
#include "Component/Light/DirectionalLight.h"
#include "Component/Light/PointLight.h"
#include "Component/Light/SpotLight.h"

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glad/glad.h>
#include "Renderer/Renderer.h"

Scene::Scene()
{
	m_Root = Ref<Entity>();
	m_Entities = std::vector<Ref<Entity>>();

	m_BackgroundColor = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

	m_LightsVertexUniformBuffer = CreateRef<UniformBuffer>(GLSL_MAT4_SIZE + (MAX_SPOT_LIGHTS * GLSL_MAT4_SIZE), 1);
	m_LightsFragmentUniformBuffer = CreateRef<UniformBuffer>(GLSL_SCALAR_SIZE * 2
		+ GLSL_DIRECTIONAL_LIGHT_SIZE
		+ (GLSL_POINT_LIGHT_SIZE * MAX_POINT_LIGHTS)
		+ (GLSL_SPOT_LIGHT_SIZE * MAX_SPOT_LIGHTS), 3);

	AddRoot();
}

void Scene::Begin()
{
	m_Root->CalculateModelMatrix();

	for (auto entity : m_Entities)
	{
		entity->Begin();
	}
}

void Scene::Update()
{
	for (auto entity : m_Entities)
	{
		entity->Update();
	}
}

void Scene::PreRender()
{
	for (auto e : m_Entities)
	{
		e->PreRender();
	}
}

void Scene::Render()
{
	m_ChangedSinceLastFrame = false;

	int pointLightsCount = GetComponentsCount<PointLight>();
	int spotLightsCount = GetComponentsCount<SpotLight>();

	m_LightsFragmentUniformBuffer->Bind();
	m_LightsFragmentUniformBuffer->SetUniform(0, GLSL_SCALAR_SIZE, &pointLightsCount);
	m_LightsFragmentUniformBuffer->SetUniform(GLSL_SCALAR_SIZE, GLSL_SCALAR_SIZE, &spotLightsCount);
	m_LightsFragmentUniformBuffer->Unbind();

	RenderEntity(GetRoot());
}

void Scene::Destroy()
{
}

void Scene::BeginPlay()
{
	for (auto entity : m_Entities)
	{
		entity->BeginPlay();
	}
}

void Scene::Tick(float deltaTime)
{
	for (auto entity : m_Entities)
	{
		entity->Tick(deltaTime);
	}
}

void Scene::EndPlay()
{
	for (auto entity : m_Entities)
	{
		entity->EndPlay();
	}
}

void Scene::RenderEntity(Ref<Entity> entity)
{
	if (!entity->IsEnable())
	{
		if (auto light = entity->GetComponent<Light>())
			light->SwitchOff();

		return;
	}

	entity->Render();

	if (!entity->GetChildren().empty())
	{
		for (auto child : entity->GetChildren())
		{
			RenderEntity(CreateRef<Entity>(*child));
		}
	}
}

Ref<Entity> Scene::AddRoot()
{
	if (m_Root)
		return m_Root;

	Ref<Entity> root = Entity::Create(this, "Root");
	m_Root = root;
	m_Entities.push_back(root);

	return root;
}

Ref<Entity> Scene::AddEntity(std::string name)
{
	Ref<Entity> entity = Entity::Create(this, name);
	entity->SetParent(m_Root.get());
	m_Entities.push_back(entity);

	return entity;
}

Ref<Entity> Scene::AddEntity(uint64_t id, std::string name)
{
	Ref<Entity> entity = Entity::Create(this, id, name);
	Entity* root = m_Root.get();
	entity->SetParent(root);
	m_Entities.push_back(entity);

	return entity;
}

Ref<Entity> Scene::AddEntity(std::string path, std::string name)
{
	Ref<Entity> entity = Entity::Create(this, name);
	entity->SetParent(m_Root.get());
	entity->AddComponent<StaticMeshComponent>(path.c_str());
	m_Entities.push_back(entity);

	return entity;
}

Ref<Entity> Scene::AddEntity(std::string path, std::string name, Ref<Entity> parent)
{
	Ref<Entity> entity = Entity::Create(this, name);
	entity->SetParent(parent.get());
	entity->AddComponent<StaticMeshComponent>(path.c_str());
	m_Entities.push_back(entity);

	return entity;
}

void Scene::RemoveEntity(Ref<Entity> entity)
{
}

Ref<Entity> Scene::FindEntity(std::string name)
{
	for (auto entity : m_Entities)
	{
		if (entity->GetName() == name)
			return entity;
	}

	return Ref<Entity>();
}

Ref<Entity> Scene::FindEntity(uint64_t id)
{
	for (auto entity : m_Entities)
	{
		if (entity->GetID() == id)
			return entity;
	}

	return Ref<Entity>();
}
