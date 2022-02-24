#pragma once

#include "typedefs.h"
#include "Camera.h"
#include "Entity.h"
#include "Material/ShaderLibrary.h"
#include "Renderer/Renderer.h"
#include "Renderer/UniformBuffer.h"
#include "Renderer/Framebuffer.h"

class Scene
{
public:
	glm::mat4 m_LightSpace;

	unsigned int m_IrradianceMap;
	unsigned int m_PrefilterMap;
	unsigned int m_BRDFLUT;

private:
	Ref<Camera> m_Camera;
	Ref<Entity> m_Root;
	std::vector<Ref<Entity>> m_Entities;
	glm::vec4 m_BackgroundColor;

	Ref<UniformBuffer> m_CameraVertexUniformBuffer;
	Ref<UniformBuffer> m_LightsVertexUniformBuffer;
	Ref<UniformBuffer> m_CameraFragmentUniformBuffer;
	Ref<UniformBuffer> m_LightsFragmentUniformBuffer;

	bool m_ChangedSinceLastFrame = false;

public:
	Scene();

	void Begin();
	void Update();
	void PreRender();
	void Render();
	void Destroy();

	// In Game
	void BeginPlay();
	void Tick(float deltaTime);
	void EndPlay();

	void RenderEntity(Ref<Entity> entity);

	Ref<Entity> AddRoot();
	Ref<Entity> AddEntity(std::string name);
	Ref<Entity> AddEntity(uint64_t id, std::string name);
	Ref<Entity> AddEntity(std::string path, std::string name);
	Ref<Entity> AddEntity(std::string path, std::string name, Ref<Entity> parent);

	void RemoveEntity(Ref<Entity> entity);
	Ref<Entity> FindEntity(std::string name);
	Ref<Entity> FindEntity(uint64_t id);

	template<typename T>
	std::vector<Ref<Component>> GetComponents()
	{
		std::vector<Ref<Component>> components;
		for (auto entity : m_Entities)
		{
			if (auto c = entity->GetComponent<T>())
				components.push_back(c);
		}

		return components;
	}

	template<typename T>
	std::vector<Ref<Entity>> GetEntitiesWithComponent()
	{
		std::vector<Ref<Entity>> entities;
		for (auto entity : m_Entities)
		{
			if (entity->GetComponent<T>())
				entities.push_back(entity);
		}

		return entities;
	}

	template<typename T>
	int GetComponentsCount()
	{
		int count = 0;
		for (auto entity : m_Entities)
		{
			if (entity->GetComponent<T>())
				count++;
		}

		return count;
	}

	inline Ref<Camera> GetCamera() const { return m_Camera; }
	inline Ref<Entity> GetRoot() const { return m_Root; }
	inline std::vector<Ref<Entity>> GetEntities() const { return m_Entities; }
	inline glm::vec4* GetBackgroundColor() { return &m_BackgroundColor; }
	inline bool IsChangedSinceLastFrame() const { return m_ChangedSinceLastFrame; }

	inline void SetChangedSinceLastFrame(bool changed) { m_ChangedSinceLastFrame = changed; }

	friend class SceneSerializer;
	friend class WorldSettingsPanel;
	friend class EntityDetailsPanel;
};