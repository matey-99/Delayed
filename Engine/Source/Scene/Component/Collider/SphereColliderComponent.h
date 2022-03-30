#pragma once

#include "ColliderComponent.h"
#include "Math/BoundingSphere.h"

class SphereColliderComponent : public ColliderComponent
{
public:
	SphereColliderComponent(Actor* owner);

	virtual void Start() override;
	virtual void Update(float deltaTime) override;
	virtual void Destroy() override;

	inline BoundingSphere GetBoundingSphere() const { return m_BoundingSphere; }

protected:
	virtual bool CheckCollisions() override;

private:
	BoundingSphere m_BoundingSphere;

	glm::vec3 m_Center;
	float m_Size;

	glm::vec3 m_OwnerLastPosition;

	friend class SceneSerializer;
};