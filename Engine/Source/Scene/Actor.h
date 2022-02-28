#pragma once

#include "glm/glm.hpp"
#include "Scene/Component/Component.h"
#include "Scene/Component/RenderComponent.h"
#include "Scene/Component/Transform.h"

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
	inline Transform GetTransform() const { return m_Transform; }
	inline bool IsEnable() const { return m_Enable; }
	inline Actor* GetParent() const { return m_Parent; }
	inline std::vector<Actor*> GetChildren() const { return m_Children; }
	inline uint64_t GetID() const { return m_ID; }

	void SetEnable(bool enable);
	void SetParent(Actor* parent);
	void SetLocalPosition(glm::vec3 position);
	void SetLocalRotation(glm::vec3 rotation);
	void SetLocalScale(glm::vec3 scale);
	void SetID(uint64_t id);

	glm::vec3 GetWorldPosition();
	glm::vec3 GetWorldRotation();

	void SetWorldPosition(glm::vec3 position);

	void CalculateModelMatrix();

private:
	Scene* m_Scene;

	uint64_t m_ID;
	std::string m_Name;
	std::vector<Ref<Component>> m_Components;

	Transform m_Transform;
	Actor* m_Parent;
	std::vector<Actor*> m_Children;

	bool m_Enable = true;

	friend class SceneHierarchyPanel;
	friend class ActorDetailsPanel;
};
