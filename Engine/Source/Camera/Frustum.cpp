#include "Frustum.h"

void Frustum::UpdateFrustum(const glm::mat4 &viewProjection) {

    for (int i = 0; i < 4; i++) m_Planes[0][i] = viewProjection[i][3] + viewProjection[i][0];
    for (int i = 0; i < 4; i++) m_Planes[1][i] = viewProjection[i][3] - viewProjection[i][0];
    for (int i = 0; i < 4; i++) m_Planes[2][i] = viewProjection[i][3] + viewProjection[i][1];
    for (int i = 0; i < 4; i++) m_Planes[3][i] = viewProjection[i][3] - viewProjection[i][1];
    for (int i = 0; i < 4; i++) m_Planes[4][i] = viewProjection[i][3] + viewProjection[i][2];
    for (int i = 0; i < 4; i++) m_Planes[5][i] = viewProjection[i][3] - viewProjection[i][2];

}

bool Frustum::PointInFrustum(const glm::vec3 &p) {
    for (auto &plane: m_Planes) {
        glm::vec3 tmp(plane[0], plane[1], plane[2]);
        float dist = glm::dot(p, tmp) + plane[3];
        if (dist < 0) return false;
    }
    return true;
}

bool Frustum::SphereInFrustum(const BoundingSphere &s) {
    for (auto &plane: m_Planes) {
        glm::vec3 tmp(plane[0], plane[1], plane[2]);
        float dist = glm::dot(s.Center, tmp) + plane[3] + s.Radius;
        if (dist < 0) return false;
    }
    return true;
}

bool Frustum::BoxInFrustum(const BoundingBox &b) {
    float x = b.Center.x;
    float y = b.Center.y;
    float z = b.Center.z;
    float size = glm::length(b.Extents);

    for (auto &plane: m_Planes) {
        if (plane[0] * (x - size) + plane[1] * (y - size) + plane[2] * (z - size) + plane[3] > 0) continue;
        if (plane[0] * (x + size) + plane[1] * (y - size) + plane[2] * (z - size) + plane[3] > 0) continue;
        if (plane[0] * (x - size) + plane[1] * (y + size) + plane[2] * (z - size) + plane[3] > 0) continue;
        if (plane[0] * (x + size) + plane[1] * (y + size) + plane[2] * (z - size) + plane[3] > 0) continue;
        if (plane[0] * (x - size) + plane[1] * (y - size) + plane[2] * (z + size) + plane[3] > 0) continue;
        if (plane[0] * (x + size) + plane[1] * (y - size) + plane[2] * (z + size) + plane[3] > 0) continue;
        if (plane[0] * (x - size) + plane[1] * (y + size) + plane[2] * (z + size) + plane[3] > 0) continue;
        if (plane[0] * (x + size) + plane[1] * (y + size) + plane[2] * (z + size) + plane[3] > 0) continue;
        return false;
    }
    return true;
}
