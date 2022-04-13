#pragma once

#include "glm/glm.hpp"
#include "Math/Ray.h"

struct BoundingSphere {

    glm::vec3 Center;
    float Radius;

    BoundingSphere();

    BoundingSphere(glm::vec3 center, float radius);

    BoundingSphere(std::vector<glm::vec3> &points);

    bool IsIntersect(const Ray& ray);
    bool IsIntersect(const glm::vec3& point);
};

