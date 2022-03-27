#pragma once

#include "typedefs.h"
#include "Actor.h"
#include "Component/CameraComponent.h"
#include "Material/ShaderLibrary.h"
#include "Renderer/Renderer.h"
#include "Renderer/UniformBuffer.h"

class Scene
{
public:
	glm::mat4 m_LightSpace;

	unsigned int m_IrradianceMap;
	unsigned int m_PrefilterMap;
	unsigned int m_BRDFLUT;

private:
	std::string m_Name;

	Ref<Actor> m_Root;
	Ref<Actor> m_UIRoot;
	std::vector<Ref<Actor>> m_Actors;
	Ref<CameraComponent> m_CurrentCamera;

	glm::vec4 m_BackgroundColor;

	Ref<UniformBuffer> m_LightsVertexUniformBuffer;
	Ref<UniformBuffer> m_LightsFragmentUniformBuffer;

	bool m_ChangedSinceLastFrame = false;

public:
	Scene();

	void Start();
	void Update(float deltaTime);
	void PreRender();
	void Render();
	void Destroy();

	void RenderActor(Actor* actor);

	Ref<Actor> AddRoot();
	Ref<Actor> AddUIRoot();

	Ref<Actor> AddActor(std::string name);
	Ref<Actor> AddActor(uint64_t id, std::string name);
	Ref<Actor> AddActor(std::string path, std::string name);
	Ref<Actor> AddActor(std::string path, std::string name, Ref<Actor> parent);

	Ref<Actor> AddUIActor(std::string name);
	Ref<Actor> AddUIActor(uint64_t id, std::string name);

	void RemoveActor(Ref<Actor> actor);
	Ref<Actor> FindActor(std::string name);
	Ref<Actor> FindActor(uint64_t id);

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

	inline void SetName(std::string name) { m_Name = name; }
	inline void SetChangedSinceLastFrame(bool changed) { m_ChangedSinceLastFrame = changed; }

	friend class SceneSerializer;
	friend class WorldSettingsPanel;
	friend class ActorDetailsPanel;
	friend class SceneHierarchyPanel;
};