#pragma once

#include "Core.h"
#include "Math/Math.h"
#include "Physics/RayCastHit.h"

#define RAY_CAST_POINTS_STRIDE 0.3f // If any object is thinner than this value, it's possible that it wouldn't be find by RayCast method

class Physics
{
public:
	static bool RayCast(Ray ray, float distance = Math::Infinity, bool allowTriggers = true, Actor* ignoreActor = nullptr);
	static bool RayCast(Ray ray, RayCastHit& hit, float distance = Math::Infinity, bool allowTriggers = true, Actor* ignoreActor = nullptr);
	static bool RayCast(glm::vec3 origin, glm::vec3 direction, float distance = Math::Infinity, bool allowTriggers = true, Actor* ignoreActor = nullptr);
	static bool RayCast(glm::vec3 origin, glm::vec3 direction, RayCastHit& hit, float distance = Math::Infinity, bool allowTriggers = true, Actor* ignoreActor = nullptr);

	static const float Gravity;
};