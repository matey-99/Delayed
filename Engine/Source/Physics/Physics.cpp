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
    /* --- Old way of calculating ray casting. More optimal (I think), especially for bigger distances, but doesn't work properly. --- */
    /*
    std::vector<Ref<Actor>> allActors = SceneManager::GetInstance()->GetCurrentScene()->GetActors();
    std::vector<Ref<Actor>> actorsInDistance;
    for (auto actor : allActors)
    {
        if (actor.get() == ignoreActor)
            continue;

        if (auto collider = actor->GetComponent<ColliderComponent>())
        {
            glm::vec3 extents;
            if (auto box = Cast<BoxColliderComponent>(collider))
            {
                extents = box->GetBoundingBox().Extents;
            }

            glm::vec3 subtract = ray.Origin - actor->GetTransform()->GetWorldPosition();
            subtract -= extents;
            subtract *= ray.Direction;
            float magnitude = Math::Magnitude(subtract);

            if (magnitude <= distance)
            {
                actorsInDistance.push_back(actor);
            }
        }
    }

    for (auto actor : actorsInDistance)
    {
        if (auto collider = actor->GetComponent<ColliderComponent>())
        {
            if (collider->IsIntersect(ray))
                return true;
        }
    }

    return false;
    */

    std::vector<Ref<Actor>> allActors = SceneManager::GetInstance()->GetCurrentScene()->GetActors();
    std::vector<Ref<ColliderComponent>> colliders;
    for (auto actor : allActors)
    {
        if (actor.get() == ignoreActor)
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
    /* --- Old way of calculating ray casting. More optimal (I think), especially for bigger distances, but doesn't work properly. --- */
    /*
    std::vector<Ref<Actor>> allActors = SceneManager::GetInstance()->GetCurrentScene()->GetActors();
    std::vector<Ref<Actor>> actorsInDistance;
    for (auto actor : allActors)
    {
        if (actor.get() == ignoreActor)
            continue;

        if (auto collider = actor->GetComponent<ColliderComponent>())
        {
            if (Math::Distance(ray.Origin, actor->GetTransform()->GetWorldPosition()) <= distance)
            {
                actorsInDistance.push_back(actor);
            }
        }
    }

    std::sort(actorsInDistance.begin(), actorsInDistance.end(), [ray](Ref<Actor> a1, Ref<Actor> a2) {
        float a1Distance = Math::Distance(ray.Origin, a1->GetTransform()->GetWorldPosition());
        float a2Distance = Math::Distance(ray.Origin, a2->GetTransform()->GetWorldPosition());

        return a1Distance < a2Distance;
        });

    for (auto actor : actorsInDistance)
    {
        if (auto collider = actor->GetComponent<ColliderComponent>())
        {
            if (collider->IsIntersect(ray))
            {
                hit.Collider = collider;
                hit.Distance = Math::Distance(ray.Origin, actor->GetTransform()->GetWorldPosition());

                return true;
            }
        }
    }

    return false;
    */

    std::vector<Ref<Actor>> allActors = SceneManager::GetInstance()->GetCurrentScene()->GetActors();
    std::vector<Ref<ColliderComponent>> colliders;
    for (auto actor : allActors)
    {
        if (actor.get() == ignoreActor)
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
