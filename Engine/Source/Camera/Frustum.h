#pragma once

#include <Math/BoundingBox.h>
#include <Math/BoundingSphere.h>
#include "glm/glm.hpp"

class Frustum {
public:
    float m_Planes[6][4];

    Frustum() = default;

    ~Frustum() = default;

    void UpdateFrustum(const glm::mat4 &viewProjection);

    bool PointInFrustum(const glm::vec3 &p);

    bool SphereInFrustum(const BoundingSphere &s);

    bool BoxInFrustum(const BoundingBox &b);

};