#include "TransformComponent.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Scene/Scene.h"
#include "Math/Math.h"

TransformComponent::TransformComponent(Actor* owner)
	: TransformBaseComponent(owner)
{
}


