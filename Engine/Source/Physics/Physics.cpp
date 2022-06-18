#include "Physics.h"

#include "Math/Ray.h"
#include "Scene/Actor.h"
#include "Scene/SceneManager.h"
#include "Scene/Component/Collider/ColliderComponent.h"
#include "Scene/Component/Collider/BoxColliderComponent.h"
#include "Scene/Component/Collider/SphereColliderComponent.h"

const float Physics::Gravity = 9.8f;

bool Physics::RayCast(Ray ray, float distance, bool allowTriggers, Actor* ignoreActor)
{
    std::vector<Actor*> allActors = SceneManager::GetInstance()->GetCurrentScene()->GetEnabledActors();
    std::vector<Ref<ColliderComponent>> colliders;
    for (auto actor : allActors)
    {
        if (actor == ignoreActor)
            continue;

        if (auto collider = actor->GetComponent<ColliderComponent>())
        {
            if (collider->IsTrigger())
                continue;

            if (collider->IsIntersect(ray))
            {
                colliders.push_back(collider);
            }
        }
    }

    for (auto collider : colliders)
    {
        for (float step = 0.0f; step < distance; step += RAY_CAST_POINTS_STRIDE)
        {
            if (collider->IsIntersect(ray.Origin + ray.Direction * step))
                return true;
        }

        if (collider->IsIntersect(ray.Origin + ray.Direction * distance))
            return true;
    }

    return false;
}

bool Physics::RayCast(Ray ray, RayCastHit& hit, float distance, bool allowTriggers, Actor* ignoreActor)
{
    std::vector<Actor*> allActors = SceneManager::GetInstance()->GetCurrentScene()->GetEnabledActors();
    std::vector<Ref<ColliderComponent>> colliders;
    for (auto actor : allActors)
    {
        if (actor == ignoreActor)
            continue;

        if (auto collider = actor->GetComponent<ColliderComponent>())
        {
            if (collider->IsTrigger())
                continue;

            if (collider->IsIntersect(ray))
            {
                colliders.push_back(collider);
            }
        }
    }

    for (auto collider : colliders)
    {
        for (float step = 0.0f; step < distance; step += RAY_CAST_POINTS_STRIDE)
        {
            if (collider->IsIntersect(ray.Origin + ray.Direction * step))
            {
                hit.Collider = collider;
                hit.Distance = step;

                return true;
            }
        }

        if (collider->IsIntersect(ray.Origin + ray.Direction * distance))
        {
            hit.Collider = collider;
            hit.Distance = distance;

            return true;
        }
    }

    return false;
}

bool Physics::RayCast(glm::vec3 origin, glm::vec3 direction, float distance, bool allowTriggers, Actor* ignoreActor)
{
	Ray ray(origin, direction);

    return RayCast(ray, distance, allowTriggers, ignoreActor);
}

bool Physics::RayCast(glm::vec3 origin, glm::vec3 direction, RayCastHit& hit, float distance, bool allowTriggers, Actor* ignoreActor)
{
    Ray ray(origin, direction);

    return RayCast(ray, hit, distance, allowTriggers, ignoreActor);
}
