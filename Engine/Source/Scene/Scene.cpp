#include "Scene.h"
#include "Component/StaticMeshComponent.h"
#include "Component/Light/DirectionalLight.h"
#include "Component/Light/PointLight.h"
#include "Component/Light/SpotLight.h"

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glad/glad.h>
#include "Renderer/Renderer.h"
#include "Component/TransformComponent.h"
#include "Component/PlayerComponent.h"

Scene::Scene()
{
	m_Root = Ref<Actor>();
	m_Actors = std::vector<Ref<Actor>>();

	m_BackgroundColor = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

	m_LightsVertexUniformBuffer = CreateRef<UniformBuffer>((GLSL_MAT4_SIZE * 16) + (MAX_SPOT_LIGHTS * GLSL_MAT4_SIZE), 1);
	m_LightsFragmentUniformBuffer = CreateRef<UniformBuffer>(GLSL_SCALAR_SIZE * 2
		+ GLSL_DIRECTIONAL_LIGHT_SIZE
		+ (GLSL_POINT_LIGHT_SIZE * MAX_POINT_LIGHTS)
		+ (GLSL_SPOT_LIGHT_SIZE * MAX_SPOT_LIGHTS), 3);

	AddRoot();
}

void Scene::Start()
{
	m_Root->GetComponent<TransformComponent>()->CalculateWorldModelMatrix();

	for (auto actor : m_Actors)
	{
		actor->Start();
	}
}

void Scene::Update(float deltaTime)
{
	for (auto actor : m_Actors)
	{
		actor->Update(deltaTime);
	}
}

void Scene::PreRender()
{
	for (auto actor : m_Actors)
	{
		actor->PreRender();
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

	RenderActor(GetRoot().get());
}

void Scene::Destroy()
{
}

void Scene::RenderActor(Actor* actor)
{
	if (!actor->IsEnable())
	{
		if (auto light = actor->GetComponent<Light>())
			light->SwitchOff();

		return;
	}

	actor->Render();

	if (!actor->GetTransform()->GetChildren().empty())
	{
		for (auto child : actor->GetTransform()->GetChildren())
		{
			RenderActor(child->GetOwner());
		}
	}

}

Ref<Actor> Scene::AddRoot()
{
	if (m_Root)
		return m_Root;

	Ref<Actor> root = Actor::Create(this, 0, "Root");
	m_Root = root;
	m_Actors.push_back(root);

	return root;
}

Ref<Actor> Scene::AddActor(std::string name)
{
	Ref<Actor> actor = Actor::Create(this, name);
	actor->GetComponent<TransformComponent>()->SetParent(m_Root->GetComponent<TransformComponent>().get());
	m_Actors.push_back(actor);

	return actor;
}

Ref<Actor> Scene::AddActor(uint64_t id, std::string name)
{
	Ref<Actor> actor = Actor::Create(this, id, name);
	actor->GetComponent<TransformComponent>()->SetParent(m_Root->GetComponent<TransformComponent>().get());
	m_Actors.push_back(actor);

	return actor;
}

Ref<Actor> Scene::AddActor(std::string path, std::string name)
{
	Ref<Actor> actor = Actor::Create(this, name);
	actor->GetComponent<TransformComponent>()->SetParent(m_Root->GetComponent<TransformComponent>().get());
	actor->AddComponent<StaticMeshComponent>(path.c_str());
	m_Actors.push_back(actor);

	return actor;
}

Ref<Actor> Scene::AddActor(std::string path, std::string name, Ref<Actor> parent)
{
	Ref<Actor> actor = Actor::Create(this, name);
	actor->GetComponent<TransformComponent>()->SetParent(m_Root->GetComponent<TransformComponent>().get());
	actor->AddComponent<StaticMeshComponent>(path.c_str());
	m_Actors.push_back(actor);

	return actor;
}

void Scene::RemoveActor(Ref<Actor> actor)
{
}

Ref<Actor> Scene::FindActor(std::string name)
{
	for (auto actor : m_Actors)
	{
		if (actor->GetName() == name)
			return actor;
	}

	return Ref<Actor>();
}

Ref<Actor> Scene::FindActor(uint64_t id)
{
	for (auto actor : m_Actors)
	{
		if (actor->GetID() == id)
			return actor;
	}

	return Ref<Actor>();
}
