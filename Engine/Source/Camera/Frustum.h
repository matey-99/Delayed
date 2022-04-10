#pragma once

#include <Math/BoundingBox.h>
#include <Math/BoundingSphere.h>
#include <Scene/Component/CameraComponent.h>
#include "glm/glm.hpp"

struct Plane {
    glm::vec3 normal;
    float distance;

    void SetNormalAndPoint(const glm::vec3 &n, const glm::vec3 &p) {
        normal = glm::normalize(n);
        distance = -glm::dot(normal, p);
    }

    float Distance(glm::vec3 &p) const {
        return (distance + glm::dot(normal, p));
    }
};

class Frustum {
private:
    enum {
        TOP = 0, BOTTOM, LEFT, RIGHT, NEAR, FAR
    };

public:

    enum {
        OUTSIDE, INTERSECT, INSIDE
    };

    Plane m_Planes[6];

    CameraComponent m_Camera;

    glm::vec3 m_NearTopLeft, m_NearTopRight, m_NearBottomLeft, m_NearBottomRight, m_FarTopLeft, m_FarTopRight, m_FarBottomLeft, m_FarBottomRight;
    float m_Tan, m_Ratio;
    float m_NearWidth, m_NearHeight, m_FarWidth, m_FarHeight;

    Frustum(const CameraComponent &mCamera);

    ~Frustum() = default;

    // Run if window resized
    void SetCamInternals();

    // Run if camera moves
    void SetCamDef();

    int PointInFrustum(glm::vec3 &p);

    int SphereInFrustum(BoundingSphere &s);

    int BoxInFrustum(BoundingBox &b);
};