#pragma once

#include "glm/glm.hpp"

struct BoundingSphere {

    glm::vec3 Center;
    float Radius;

    BoundingSphere();

    BoundingSphere(glm::vec3 center, float radius);

    BoundingSphere(std::vector<glm::vec3> &points);
};

