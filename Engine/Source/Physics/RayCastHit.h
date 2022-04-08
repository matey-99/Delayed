#pragma once

#include "Core.h"
#include "Scene/Component/Collider/ColliderComponent.h"

struct RayCastHit
{
public:
	Ref<ColliderComponent> Collider;
	float Distance;
};