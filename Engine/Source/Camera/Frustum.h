#pragma once

#include <Math/BoundingBox.h>
#include <Math/BoundingSphere.h>
#include "glm/glm.hpp"

struct Plane {
    glm::vec3 normal;
    float distance;

    void SetNormalAndPoint(const glm::vec3 &n, const glm::vec3 &p) {
        normal = glm::normalize(n);
        distance = -glm::dot(normal, p);
    }

    float Distance(const glm::vec3 &p) {
        return (distance + glm::dot(normal, p));
    }
};

class Frustum {
public:

    enum {
        OUTSIDE, INTERSECT, INSIDE
    };

    Plane m_Planes[6];

    glm::vec3 m_NearTopLeft, m_NearTopRight, m_NearBottomLeft, m_NearBottomRight, m_FarTopLeft, m_FarTopRight, m_FarBottomLeft, m_FarBottomRight;
    float m_Tan, m_Ratio, m_FOV, m_NearDistance, m_FarDistance;
    float m_NearWidth, m_NearHeight, m_FarWidth, m_FarHeight;

    Frustum() = default;

    ~Frustum() = default;

    // Run if window resized
    void SetCamInternals(float angle, float ratio, float nearD, float farD);

    // Run if camera moves
    void SetCamDef(const glm::vec3 &p, const glm::vec3 &front, const glm::vec3 &up);

    int PointInFrustum(const glm::vec3 &p);

    int SphereInFrustum(const BoundingSphere &s);

    int BoxInFrustum(BoundingBox b);
};