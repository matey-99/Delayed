#include "ColliderComponent.h"

ColliderComponent::ColliderComponent(Actor* owner)
	: Component(owner)
{
}

float ColliderComponent::Dist2PointAABB(glm::vec3 point, BoundingBox box) {
    float dist2 = 0.0f;

    for (int i = 0; i < 3; ++i) {
        float p = point[i];
        if (p < box.Min[i]) dist2 += (box.Min[i] - p) * (box.Min[i] - p);
        if (p > box.Max[i]) dist2 += (p - box.Max[i]) * (p - box.Max[i]);
    }

    return dist2;
}
