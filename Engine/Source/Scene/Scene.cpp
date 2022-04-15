#include "Scene.h"
#include "Component/StaticMeshComponent.h"
#include "Component/Light/DirectionalLight.h"
#include "Component/Light/PointLight.h"
#include "Component/Light/SpotLight.h"

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glad/glad.h>
#include <Scene/Component/Collider/SphereColliderComponent.h>
#include <Scene/Component/Collider/BoxColliderComponent.h>
#include "Renderer/Renderer.h"
#include "Renderer/RenderPass/ShadowsPass.h"
#include "Component/TransformComponent.h"
#include "Component/Light/SkyLight.h"
#include "Component/UI/RectTransformComponent.h"
#include "Camera/CameraManager.h"
#include "Math/Math.h"

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
	AddUIRoot();
}

void Scene::Start()
{
	CameraManager::GetInstance()->SetMainCamera(m_CurrentCamera);

	m_Root->GetComponent<TransformComponent>()->CalculateWorldModelMatrix();
	m_UIRoot->GetComponent<RectTransformComponent>()->CalculateWorldModelMatrix();

	for (auto actor : m_Actors)
	{
		actor->Start();
	}

	m_SkyLight = FindComponent<SkyLight>();
}

void Scene::Update(float deltaTime)
{
	for (auto actor : m_Actors)
	{
		actor->Update(deltaTime);
	}

	for (auto actor : m_ActorsAddedRuntime)
		m_Actors.push_back(actor);
	m_ActorsAddedRuntime.clear();

	for (auto actor : m_ActorsDestroyedRuntime)
		RemoveActor(actor);
	m_ActorsDestroyedRuntime.clear();
}

void Scene::FixedUpdate()
{
	for (auto actor : m_Actors)
		actor->FixedUpdate();
}

void Scene::PreRender()
{
	m_ChangedSinceLastFrame = false;

	int pointLightsCount = GetComponentsCount<PointLight>();
	int spotLightsCount = GetComponentsCount<SpotLight>();

	m_LightsFragmentUniformBuffer->Bind();
	m_LightsFragmentUniformBuffer->SetUniform(0, GLSL_SCALAR_SIZE, &pointLightsCount);
	m_LightsFragmentUniformBuffer->SetUniform(GLSL_SCALAR_SIZE, GLSL_SCALAR_SIZE, &spotLightsCount);
	m_LightsFragmentUniformBuffer->Unbind();

	for (auto actor : m_Actors)
	{
		actor->PreRender();
	}

	UpdateMeshesRenderList();
}

void Scene::Render(Material::BlendMode blendMode)
{
	MeshesRenderList meshesList;
	for (auto& mesh : m_MeshesRenderList)
	{
		if (mesh.first->Material->GetBlendMode() == blendMode)
			meshesList.insert(mesh);
	}

	RenderMeshes(meshesList, blendMode);

	if (m_SkyLight)
		m_SkyLight->Render(blendMode);
}

void Scene::Render(Ref<Shader> shader)
{
	shader->Use();

	for (auto& renderMesh : m_MeshesRenderList)
	{
		auto mesh = renderMesh.first->Mesh;

		std::vector<glm::mat4> transformations;
		uint32_t instancesCount = 0;
		for (auto& transformation : renderMesh.second)
		{
			instancesCount++;
			transformations.push_back(transformation);
		}

		mesh->RenderInstanced(instancesCount, transformations);
	}
}

void Scene::Destroy()
{
}

void Scene::GetEnabledActors(Actor* actor, std::vector<Actor*>& output)
{
	if (!actor->IsEnabled())
	{
		if (auto light = actor->GetComponent<Light>())
			light->SwitchOff();

		return;
	}
	
	output.push_back(actor);

	if (!actor->GetTransform()->GetChildren().empty())
	{
		for (auto child : actor->GetTransform()->GetChildren())
		{
			GetEnabledActors(child->GetOwner(), output);
		}
	}
}

void Scene::SortActorsByDistance(std::vector<Actor*>& actors, glm::vec3 point, bool ascending)
{
	std::sort(actors.begin(), actors.end(), [point, ascending](Actor* a1, Actor* a2) {
		float a1Distance = Math::Distance(a1->GetTransform()->GetWorldPosition(), point);
		float a2Distance = Math::Distance(a2->GetTransform()->GetWorldPosition(), point);

		return ascending ? a1Distance < a2Distance : a1Distance > a2Distance;
		});
}

std::vector<Actor*> Scene::CullActors(std::vector<Actor*>& actors) {
    std::vector<Actor*> output;
    for (auto actor : actors) {
        if (auto a = actor->GetComponent<StaticMeshComponent>()) {
            if (m_CurrentCamera->GetFrustum()->BoxInFrustum(a->GetBoundingBox())) {
                output.push_back(actor);
            }
        }
//        if (auto a = actor->GetComponent<SphereColliderComponent>()) {
//            if (m_CurrentCamera->GetFrustum()->SphereInFrustum(a->GetBoundingSphere())) {
//                output.push_back(actor);
//            }
//        }
//        if (auto a = actor->GetComponent<BoxColliderComponent>()) {
//            if (m_CurrentCamera->GetFrustum()->BoxInFrustum(a->GetBoundingBox()) ) {
//                output.push_back(actor);
//            }
//        }
    }
    return output;
}

void Scene::SortMeshes(std::vector<Ref<MeshComponent>>& meshComponents)
{
	m_MeshesRenderList.clear();

	for (auto meshComponent : meshComponents)
	{
		for (int i = 0; i < meshComponent->GetMeshes().size(); i++)
		{
			if (meshComponent->GetMaterials().size() > i)
			{
				Ref<Mesh> mesh = meshComponent->GetMeshes()[i];
				Ref<Material> material = meshComponent->GetMaterials()[i];

				glm::mat4 transformation = meshComponent->GetOwner()->GetTransform()->GetWorldModelMatrix();

				bool found = false;
				for (auto& renderMesh : m_MeshesRenderList)
				{
					if (renderMesh.first->Mesh == mesh && renderMesh.first->Material == material)
					{
						found = true;
						renderMesh.second.push_back(transformation);

						break;
					}
				}
				if (!found)
				{
					Ref<MaterialMesh> materialMesh = CreateRef<MaterialMesh>();
					materialMesh->Mesh = mesh;
					materialMesh->Material = material;

					std::vector<glm::mat4> transformations;
					transformations.push_back(transformation);

					m_MeshesRenderList.insert({ materialMesh, transformations });
				}
			}
		}
	}
}

void Scene::UpdateMeshesRenderList()
{
	std::vector<Actor*> actors;
	GetEnabledActors(m_Root.get(), actors);

	actors = CullActors(actors);

	// Set order of rendering actors
	auto cameraPosition = CameraManager::GetInstance()->GetMainCamera()->GetWorldPosition();
	SortActorsByDistance(actors, cameraPosition, false);

	std::vector<Ref<MeshComponent>> meshComponents;
	for (auto actor : actors)
	{
		if (auto m = actor->GetComponent<MeshComponent>())
			meshComponents.push_back(m);
	}
	SortMeshes(meshComponents);
}

void Scene::RenderMeshes(MeshesRenderList meshes, Material::BlendMode blendMode)
{
	for (auto& renderMesh : meshes)
	{
		auto mesh = renderMesh.first->Mesh;
		auto material = renderMesh.first->Material;

		material->Use();

		if (blendMode == Material::BlendMode::Transparent)
		{
			auto s = Renderer::GetInstance()->m_ShadowsPass;
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D_ARRAY, s->GetDirectionalLightRenderTarget()->GetTargets()[0]);

			material->GetShader()->SetInt("u_DirectionalLightShadowMaps", 0);

			if (auto skyLight = FindComponent<SkyLight>())
			{
				material->GetShader()->SetVec3("u_SkyLightColor", skyLight->GetColor());
				material->GetShader()->SetFloat("u_SkyLightIntensity", skyLight->GetIntensity());
			}
			else
			{
				material->GetShader()->SetVec3("u_SkyLightColor", glm::vec3(1.0f));
				material->GetShader()->SetFloat("u_SkyLightIntensity", 0.03f);
			}
		}

		std::vector<glm::mat4> transformations;
		uint32_t instancesCount = 0;
		for (auto& transformation : renderMesh.second)
		{
			instancesCount++;
			transformations.push_back(transformation);
		}

		mesh->RenderInstanced(instancesCount, transformations);
	}
}

Ref<Actor> Scene::AddRoot()
{
	if (m_Root)
		return m_Root;

	Ref<Actor> root = Actor::Create(this, 0, "Root");
	root->AddComponent<TransformComponent>();

	m_Root = root;
	m_Actors.push_back(root);

	return root;
}

Ref<Actor> Scene::AddUIRoot()
{
	if (m_UIRoot)
		return m_UIRoot;

	Ref<Actor> uiRoot = Actor::Create(this, 1, "UI Root");
	uiRoot->AddComponent<RectTransformComponent>();

	m_UIRoot = uiRoot;
	m_Actors.push_back(uiRoot);

	return uiRoot;
}

Ref<Actor> Scene::AddActor(std::string name)
{
	Ref<Actor> actor = Actor::Create(this, name);

	auto transform = actor->AddComponent<TransformComponent>();
	transform->SetParent(m_Root->GetComponent<TransformComponent>().get());
	actor->SetTransform(transform);

	m_Actors.push_back(actor);

	return actor;
}

Ref<Actor> Scene::AddActor(uint64_t id, std::string name)
{
	Ref<Actor> actor = Actor::Create(this, id, name);

	auto transform = actor->AddComponent<TransformComponent>();
	transform->SetParent(m_Root->GetComponent<TransformComponent>().get());
	actor->SetTransform(transform);

	m_Actors.push_back(actor);

	return actor;
}

Ref<Actor> Scene::AddActor(std::string path, std::string name)
{
	Ref<Actor> actor = Actor::Create(this, name);

	auto transform = actor->AddComponent<TransformComponent>();
	transform->SetParent(m_Root->GetComponent<TransformComponent>().get());
	actor->SetTransform(transform);

	actor->AddComponent<StaticMeshComponent>(path.c_str());

	m_Actors.push_back(actor);

	return actor;
}

Ref<Actor> Scene::AddActor(std::string path, std::string name, Ref<Actor> parent)
{
	Ref<Actor> actor = Actor::Create(this, name);

	auto transform = actor->AddComponent<TransformComponent>();
	transform->SetParent(m_Root->GetComponent<TransformComponent>().get());
	actor->SetTransform(transform);

	actor->AddComponent<StaticMeshComponent>(path.c_str());

	m_Actors.push_back(actor);

	return actor;
}

void Scene::AddActorToList(Ref<Actor> actor)
{
	m_Actors.push_back(actor);
}

Ref<Actor> Scene::AddUIActor(std::string name)
{
	Ref<Actor> actor = Actor::Create(this, name);

	auto transform = actor->AddComponent<RectTransformComponent>();
	transform->SetParent(m_UIRoot->GetComponent<RectTransformComponent>().get());
	actor->SetTransform(transform);

	m_Actors.push_back(actor);

	return actor;
}

Ref<Actor> Scene::AddUIActor(uint64_t id, std::string name)
{
	Ref<Actor> actor = Actor::Create(this, id, name);

	auto transform = actor->AddComponent<RectTransformComponent>();
	transform->SetParent(m_UIRoot->GetComponent<RectTransformComponent>().get());
	actor->SetTransform(transform);

	m_Actors.push_back(actor);

	return actor;
}

void Scene::RemoveActor(Actor* actor)
{
	if (actor->GetTransform()->GetChildren().empty())
	{
		auto parent = actor->GetTransform()->GetParent();
		if (parent)
			parent->RemoveChild(actor->GetTransform().get());

		m_Actors.erase(std::remove_if(m_Actors.begin(), m_Actors.end(), [actor](Ref<Actor> a) { return a->GetID() == actor->GetID(); }), m_Actors.end());
	}
	else
	{
		for (auto child : actor->GetTransform()->GetChildren())
		{
			RemoveActor(child->GetOwner());
		}
	}

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

void Scene::DestroyActor(Actor* actor)
{
	m_ActorsDestroyedRuntime.push_back(actor);
}
