#pragma once

#include "Component.h"

#include <glm/glm.hpp>

class TransformComponent : public Component
{
public:
	TransformComponent(Actor* owner);

	virtual void Start() override;
	virtual void Update(float deltaTime) override;
	virtual void Destroy() override;

	// LOCAL
	inline glm::vec3 GetLocalPosition() const { return m_LocalPosition; }
	inline glm::vec3 GetLocalRotation() const { return m_LocalRotation; }
	inline glm::vec3 GetLocalScale() const { return m_LocalScale; }

	void SetLocalPosition(glm::vec3 position);
	void SetLocalRotation(glm::vec3 rotation);
	void SetLocalScale(glm::vec3 scale);

	// WORLD
	glm::vec3 GetWorldPosition();
	glm::vec3 GetWorldRotation();
	glm::vec3 GetWorldScale();

	void SetWorldPosition(glm::vec3 position);

	// SCENE HIERARCHY
	inline TransformComponent* GetParent() const { return m_Parent; }
	inline std::vector<TransformComponent*> GetChildren() const { return m_Children; }

	void SetParent(TransformComponent* parent);

	// MODEL MATRIX
	void CalculateLocalModelMatrix();
	void CalculateWorldModelMatrix();

	inline glm::mat4 GetLocalModelMatrix() const { return m_LocalModelMatrix; }
	inline glm::mat4 GetWorldModelMatrix() const { return m_WorldModelMatrix; }

private:
	glm::vec3 m_LocalPosition;
	glm::vec3 m_LocalRotation;
	glm::vec3 m_LocalScale;

	TransformComponent* m_Parent;
	std::vector<TransformComponent*> m_Children;

	glm::mat4 m_LocalModelMatrix;
	glm::mat4 m_WorldModelMatrix;

	friend class SceneSerializer;
};