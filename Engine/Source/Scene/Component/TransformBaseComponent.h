#pragma once

#include "Component.h"

#include <glm/glm.hpp>

class TransformBaseComponent : public Component
{
public:
	TransformBaseComponent(Actor* owner);

	virtual void Start() override;
	virtual void Update(float deltaTime) override;
	virtual void Destroy() override;

	// LOCAL
	virtual inline glm::vec3 GetLocalPosition() const { return m_LocalPosition; }
	virtual inline glm::vec3 GetLocalRotation() const { return m_LocalRotation; }
	virtual inline glm::vec3 GetLocalScale() const { return m_LocalScale; }

	virtual void SetLocalPosition(glm::vec3 position);
	virtual void SetLocalRotation(glm::vec3 rotation);
	virtual void SetLocalScale(glm::vec3 scale);

	// WORLD
	virtual glm::vec3 GetWorldPosition();
	virtual glm::vec3 GetWorldRotation();
	virtual glm::vec3 GetWorldScale();

	virtual void SetWorldPosition(glm::vec3 position);

	// SCENE HIERARCHY
	virtual inline TransformBaseComponent* GetParent() const { return m_Parent; }
	virtual inline std::vector<TransformBaseComponent*> GetChildren() const { return m_Children; }

	virtual void SetParent(TransformBaseComponent* parent);

	// MODEL MATRIX
	virtual void CalculateLocalModelMatrix();
	virtual void CalculateWorldModelMatrix();

	virtual inline glm::mat4 GetLocalModelMatrix() const { return m_LocalModelMatrix; }
	virtual inline glm::mat4 GetWorldModelMatrix() const { return m_WorldModelMatrix; }

	virtual inline glm::vec3 GetForward() const { return m_Forward; }

protected:
	glm::vec3 m_LocalPosition;
	glm::vec3 m_LocalRotation;
	glm::vec3 m_LocalScale;

	TransformBaseComponent* m_Parent;
	std::vector<TransformBaseComponent*> m_Children;

	glm::mat4 m_LocalModelMatrix;
	glm::mat4 m_WorldModelMatrix;

	glm::vec3 m_Forward;

	friend class SceneSerializer;
};