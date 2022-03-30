#pragma once

#include "Scene/Component/Component.h"
#include "Math/BoundingBox.h"
#include "Math/BoundingSphere.h"

class ColliderComponent : public Component
{
public:
	ColliderComponent(Actor* owner);

protected:
	virtual bool CheckCollisions() = 0;

    static float Dist2PointAABB(glm::vec3 point, BoundingBox box);

	friend class SceneSerializer;
};