#pragma once

#include "glm/glm.hpp"
#include "Scene/Component/Component.h"
#include "Scene/Component/TransformBaseComponent.h"
#include "Scene/Component/TransformComponent.h"
#include "Scene/Component/RenderComponent.h"

class Scene;

class Actor
{
public:
	static Ref<Actor> Create(Scene* scene, std::string name);
	static Ref<Actor> Create(Scene* scene, uint64_t id, std::string name);

	Actor(Scene* scene, std::string name);
	Actor(Scene* scene, uint64_t id, std::string name);

	void Start();
	void Update(float deltaTime);
	void FixedUpdate();
	void PreRender();
	void Render();
	void Destroy();

	template<typename T, typename ... Args>
	Ref<T> AddComponent(Args&& ... args)
	{
		Ref<T> comp = CreateRef<T>(this, std::forward<Args>(args)...);
		m_Components.push_back(comp);

		return comp;
	}

	template<typename T>
	Ref<T> GetComponent() const
	{
		for (auto component : m_Components)
		{
			if (Ref<T> comp = Cast<T>(component))
				return comp;
		}

		return Ref<T>();
	}

	inline std::vector<Ref<Component>> GetComponents() const { return m_Components; }

	template<typename T>
	void RemoveComponent()
	{
		for (auto component : m_Components)
		{
			if (Cast<T>(component))
			{
				m_Components.erase(std::remove(m_Components.begin(), m_Components.end(), component), m_Components.end());
			}
		}
	}

	inline Scene* GetScene() const { return m_Scene; }
	inline std::string GetName() const { return m_Name; }
	inline bool IsEnabled() const { return m_Enabled; }
	inline uint64_t GetID() const { return m_ID; }
	inline bool IsDynamic() const { return m_Dynamic; }

	Ref<TransformBaseComponent> GetTransform();
	inline void SetTransform(Ref<TransformBaseComponent> transform) { m_Transform = transform; }

	void SetDynamic(bool dynamic);
	void SetEnabled(bool enabled);
	void SetID(uint64_t id);

private:
	Scene* m_Scene;

	uint64_t m_ID;
	std::string m_Name;
	bool m_Enabled = true;
	bool m_Dynamic = false;
	std::vector<Ref<Component>> m_Components;

	Ref<TransformBaseComponent> m_Transform;


	friend class SceneHierarchyPanel;
	friend class ActorDetailsPanel;
};
