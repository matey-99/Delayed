#include "Frustum.h"

Frustum::Frustum(const CameraComponent &mCamera) : m_Camera(mCamera) {
    SetCamInternals();
    SetCamDef();
}

void Frustum::SetCamInternals() {
    m_Ratio = m_Camera.GetAspectRatio().x / m_Camera.GetAspectRatio().y;
    m_Tan = (float) tan(glm::radians(m_Camera.GetFieldOfView()) * 0.5f);

    m_NearHeight = m_Camera.GetNearClipPlane() * m_Tan;
    m_NearWidth = m_NearHeight * m_Ratio;
    m_FarHeight = m_Camera.GetFarClipPlane() * m_Tan;
    m_FarWidth = m_FarHeight * m_Ratio;
}

void Frustum::SetCamDef() {

    glm::vec3 p, nearCenter, farCenter, X, Y, Z;
    p = m_Camera.GetWorldPosition();
    X = m_Camera.GetRight();
    Z = m_Camera.GetFront();
    Y = glm::cross(Z, X);

    nearCenter = m_Camera.GetWorldPosition() - Z * m_Camera.GetNearClipPlane();
    farCenter = m_Camera.GetWorldPosition() - Z * m_Camera.GetFarClipPlane();

    m_NearTopLeft = nearCenter + Y * m_NearHeight - X * m_NearWidth;
    m_NearTopRight = nearCenter + Y * m_NearHeight + X * m_NearWidth;
    m_NearBottomLeft = nearCenter - Y * m_NearHeight - X * m_NearWidth;
    m_NearBottomRight = nearCenter - Y * m_NearHeight + X * m_NearWidth;

    m_FarTopLeft = farCenter + Y * m_FarHeight - X * m_NearWidth;
    m_FarTopRight = farCenter + Y * m_FarHeight + X * m_NearWidth;
    m_FarBottomLeft = farCenter - Y * m_FarHeight - X * m_NearWidth;
    m_FarBottomRight = farCenter - Y * m_FarHeight + X * m_NearWidth;

    m_Planes[NEAR].SetNormalAndPoint(-Z, nearCenter);
    m_Planes[FAR].SetNormalAndPoint(Z, farCenter);

    glm::vec3 aux, normal;

    aux = (nearCenter + Y * m_NearHeight) - p;
    aux = glm::normalize(aux);
    normal = aux * X;
    m_Planes[TOP].SetNormalAndPoint(normal, nearCenter + Y * m_NearHeight);

    aux = (nearCenter - Y * m_NearHeight) - p;
    aux = glm::normalize(aux);
    normal = X * aux;
    m_Planes[BOTTOM].SetNormalAndPoint(normal, nearCenter - Y * m_NearHeight);

    aux = (nearCenter - X * m_NearWidth) - p;
    aux = glm::normalize(aux);
    normal = aux * Y;
    m_Planes[LEFT].SetNormalAndPoint(normal, nearCenter - X * m_NearWidth);

    aux = (nearCenter + X * m_NearWidth) - p;
    aux = glm::normalize(aux);
    normal = Y * aux;
    m_Planes[RIGHT].SetNormalAndPoint(normal, nearCenter + X * m_NearWidth);

}

int Frustum::PointInFrustum(glm::vec3 &p) {
    int result = INSIDE;
    for(auto m_Plane : m_Planes) {
        if (m_Plane.Distance(p) < 0)
            return OUTSIDE;
    }
    return result;
}

int Frustum::SphereInFrustum(BoundingSphere &s) {
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

int Frustum::BoxInFrustum(BoundingBox &b) {
    return 0;
}
