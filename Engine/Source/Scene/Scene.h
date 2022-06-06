#pragma once

#include "typedefs.h"
#include "Actor.h"
#include "Component/CameraComponent.h"
#include "Material/ShaderLibrary.h"
#include "Renderer/Renderer.h"
#include "Renderer/UniformBuffer.h"
#include "Patterns/Delegate.h"

class MeshBase;
class MeshComponent;
class FoliageComponent;
class SkyLight;
//class Animator;

struct MaterialMesh
{
	Ref<MeshBase> Mesh;
	Ref<Material> Material;
	bool CastShadow;
};

typedef std::unordered_map<Ref<MaterialMesh>, std::vector<glm::mat4>> MeshesRenderList;

class Scene
{
public:
	Scene();

	void Start();
	void Update(float deltaTime);
	//void UpdateAnimation(float deltaTime);
	void FixedUpdate();
	void PreRender();
	void Render(Material::BlendMode blendMode = Material::BlendMode::Opaque);
	void Render(Ref<Shader> shader);
	void Destroy();

	Ref<Actor> AddRoot();
	Ref<Actor> AddUIRoot();

	Ref<Actor> AddActor(std::string name);
	Ref<Actor> AddActor(uint64_t id, std::string name);
	Ref<Actor> AddActor(std::string path, std::string name);
	Ref<Actor> AddActor(std::string path, std::string name, Ref<Actor> parent);

	void AddActorToList(Ref<Actor> actor);

	Ref<Actor> AddUIActor(std::string name);
	Ref<Actor> AddUIActor(uint64_t id, std::string name);

	void RemoveActor(Actor* actor);
	Ref<Actor> FindActor(std::string name);
	Ref<Actor> FindActor(uint64_t id);

	Ref<Actor> SpawnActor(const glm::vec3& position = glm::vec3(0.0f), const glm::vec3& rotation = glm::vec3(0.0f), Actor* parent = nullptr);
	void DestroyActor(Actor* actor);

	void FindEnabledActors(Actor* actor, std::vector<Actor*>& output);

	template<typename T>
	Ref<T> FindComponent()
	{
		for (auto actor : m_Actors)
		{
			if (auto c = actor->GetComponent<T>())
				return c;
		}

		return nullptr;
	}

	template<typename T>
	Ref<T> GetComponent(uint64_t ownerID)
	{
		Ref<Actor> actor = FindActor(ownerID);
		if (!actor)
			return nullptr;

		return actor->GetComponent<T>();
	}

	template<typename T>
	std::vector<Ref<T>> GetComponents()
	{
		std::vector<Ref<T>> components;
		for (auto actor : m_Actors)
		{
			if (auto c = actor->GetComponent<T>())
				components.push_back(c);
		}

		return components;
	}

	template<typename T>
	std::vector<Ref<Actor>> GetActorsWithComponent()
	{
		std::vector<Ref<Actor>> actors;
		for (auto actor : m_Actors)
		{
			if (actor->GetComponent<T>())
				actors.push_back(actor);
		}

		return actors;
	}

	template<typename T>
	int GetComponentsCount()
	{
		int count = 0;
		for (auto actor : m_Actors)
		{
			if (actor->GetComponent<T>())
				count++;
		}

		return count;
	}

	inline Ref<CameraComponent> GetCurrentCamera() const { return m_CurrentCamera; }
	inline Ref<Actor> GetRoot() const { return m_Root; }
	inline Ref<Actor> GetUIRoot() const { return m_UIRoot; }
	inline std::vector<Ref<Actor>> GetActors() const { return m_Actors; }
	inline glm::vec4* GetBackgroundColor() { return &m_BackgroundColor; }
	inline bool IsChangedSinceLastFrame() const { return m_ChangedSinceLastFrame; }
	inline std::vector<Actor*> GetEnabledActors() const { return m_EnabledActors; }

	inline void SetName(std::string name) { m_Name = name; }
	inline void SetChangedSinceLastFrame(bool changed) { m_ChangedSinceLastFrame = changed; }
	inline void SetEnabledActors(std::vector<Actor*> actors) { m_EnabledActors = actors; }

private:
	void SortActorsByDistance(std::vector<Actor*>& actors, glm::vec3 point, bool ascending = true);
	void SortMeshes(std::vector<Ref<MeshComponent>>& meshComponents);
	void SortFoliages(std::vector<Ref<FoliageComponent>>& foliageComponents);
    std::vector<Actor*> CullActors(std::vector<Actor*>& actors);
	void UpdateMeshesRenderList(bool shouldCullActors = true);
	void RenderMeshes(MeshesRenderList meshes, Material::BlendMode blendMode);

private:
	std::string m_Name;

	Ref<Actor> m_Root;
	Ref<Actor> m_UIRoot;
	std::vector<Ref<Actor>> m_Actors;
	std::vector<Actor*> m_EnabledActors;
	Ref<CameraComponent> m_CurrentCamera;
	Ref<SkyLight> m_SkyLight;
	//std::vector<Ref<Animator>> m_Animators;

	std::vector<Ref<Actor>> m_ActorsAddedRuntime;
	std::vector<Actor*> m_ActorsDestroyedRuntime;

	MeshesRenderList m_MeshesRenderList;

	glm::vec4 m_BackgroundColor;

	Ref<UniformBuffer> m_LightsVertexUniformBuffer;
	Ref<UniformBuffer> m_LightsFragmentUniformBuffer;

	bool m_ChangedSinceLastFrame = false;


	friend class SceneSerializer;
	friend class WorldSettingsPanel;
	friend class ActorDetailsPanel;
	friend class SceneHierarchyPanel;
};