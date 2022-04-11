#include "Frustum.h"

void Frustum::SetCamInternals(float angle, float ratio, float nearDistance, float farDistance) {
    m_Ratio = ratio;
    m_FOV = angle;
    m_Tan = (float) tan(glm::radians(angle) * 0.5f);
    m_NearDistance = nearDistance;
    m_FarDistance = farDistance;

    m_NearHeight = nearDistance * m_Tan;
    m_NearWidth = m_NearHeight * m_Ratio;
    m_FarHeight = farDistance * m_Tan;
    m_FarWidth = m_FarHeight * m_Ratio;
}

void Frustum::SetCamDef(const glm::vec3 &p, const glm::vec3 &front, const glm::vec3 &up) {

    glm::vec3 nearCenter, farCenter, X, Y, Z;
    Z = -front;
    X = glm::cross(up, Z);
    Y = glm::cross(Z, X);

    nearCenter = p - Z * m_NearDistance;
    farCenter = p - Z * m_FarDistance;

    m_NearTopLeft = nearCenter + Y * m_NearHeight - X * m_NearWidth;
    m_NearTopRight = nearCenter + Y * m_NearHeight + X * m_NearWidth;
    m_NearBottomLeft = nearCenter - Y * m_NearHeight - X * m_NearWidth;
    m_NearBottomRight = nearCenter - Y * m_NearHeight + X * m_NearWidth;

    m_FarTopLeft = farCenter + Y * m_FarHeight - X * m_NearWidth;
    m_FarTopRight = farCenter + Y * m_FarHeight + X * m_NearWidth;
    m_FarBottomLeft = farCenter - Y * m_FarHeight - X * m_NearWidth;
    m_FarBottomRight = farCenter - Y * m_FarHeight + X * m_NearWidth;

    m_Planes[4].SetNormalAndPoint(-Z, nearCenter);
    m_Planes[5].SetNormalAndPoint(Z, farCenter);

    glm::vec3 aux, normal;

    aux = (nearCenter + Y * m_NearHeight) - p;
    aux = glm::normalize(aux);
    normal = aux * X;
    m_Planes[0].SetNormalAndPoint(normal, nearCenter + Y * m_NearHeight);

    aux = (nearCenter - Y * m_NearHeight) - p;
    aux = glm::normalize(aux);
    normal = X * aux;
    m_Planes[1].SetNormalAndPoint(normal, nearCenter - Y * m_NearHeight);

    aux = (nearCenter - X * m_NearWidth) - p;
    aux = glm::normalize(aux);
    normal = aux * Y;
    m_Planes[2].SetNormalAndPoint(normal, nearCenter - X * m_NearWidth);

    aux = (nearCenter + X * m_NearWidth) - p;
    aux = glm::normalize(aux);
    normal = Y * aux;
    m_Planes[3].SetNormalAndPoint(normal, nearCenter + X * m_NearWidth);

}

int Frustum::PointInFrustum(const glm::vec3 &p) {
    int result = INSIDE;
    for(auto m_Plane : m_Planes) {
        if (m_Plane.Distance(p) < 0)
            return OUTSIDE;
    }
    return result;
}

int Frustum::SphereInFrustum(const BoundingSphere &s) {
    float distance;
    int result = INSIDE;
    for(auto & m_Plane : m_Planes) {
        distance = m_Plane.Distance(s.Center);
        if (distance < -s.Radius)
            return OUTSIDE;
        else if (distance < s.Radius)
            result =  INTERSECT;
    }
    return result;
}

int Frustum::BoxInFrustum(BoundingBox b) {
    int result = INSIDE, out,in;

    // for each plane do ...
    for(auto & m_Plane : m_Planes) {

        out=0;in=0;
        for (int k = 0; k < 8 && (in==0 || out==0); k++) {
            if (m_Plane.Distance(b.GetPoints()[k]) < 0)
                out++;
            else
                in++;
        }
        if (!in)
            return (OUTSIDE);
        else if (out)
            result = INTERSECT;
    }
    return result;
}
