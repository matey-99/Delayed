#include "BoundingSphere.h"

#include <utility>

void MostSeparatedPointsOnAABB(int &min, int &max, std::vector<glm::vec3> &points) {
    int minx = 0, maxx = 0, miny = 0, maxy = 0, minz = 0, maxz = 0;

    for (int i = 1; i < points.size(); ++i) {
        if (points[i].x < points[minx].x) minx = i;
        if (points[i].x < points[maxx].x) maxx = i;
        if (points[i].y < points[miny].y) miny = i;
        if (points[i].y < points[maxy].y) maxy = i;
        if (points[i].z < points[minz].z) minz = i;
        if (points[i].z < points[maxz].z) maxz = i;
    }

    float dist2x = glm::dot(points[maxx] - points[minx], points[maxx] - points[minx]);
    float dist2y = glm::dot(points[maxy] - points[miny], points[maxy] - points[miny]);
    float dist2z = glm::dot(points[maxz] - points[minz], points[maxz] - points[minz]);

    min = minx;
    max = maxx;
    if (dist2y > dist2x && dist2y > dist2z) {
        max = maxy;
        min = miny;
    }
    if (dist2z > dist2x && dist2z > dist2y) {
        max = maxz;
        min = minz;
    }
}

void SphereFromDistantPoints(BoundingSphere *sphere, std::vector<glm::vec3> &points) {
    int min, max;
    MostSeparatedPointsOnAABB(min, max, points);

    sphere->Center = (points[min] + points[max]) * 0.5f;
    sphere->Radius = glm::dot(points[max] - sphere->Center, points[max] - sphere->Center);
    sphere->Radius = glm::sqrt(sphere->Radius);
}

void SphereOfSphereAndPt(BoundingSphere *sphere, glm::vec3 &point) {
    glm::vec3 d = point - sphere->Center;
    float dist2 = glm::dot(d, d);

    if (dist2 > sphere->Radius * sphere->Radius) {
        float dist = glm::sqrt(dist2);
        float newRadius = (sphere->Radius + dist) * 0.5f;
        float k = (newRadius - sphere->Radius) / dist;
        sphere->Radius = newRadius;
        sphere->Center += d * k;
    }
}

BoundingSphere::BoundingSphere() : Center(glm::vec3(0.0f)), Radius(1.0f) {}

BoundingSphere::BoundingSphere(glm::vec3 center, float radius) : Center(center), Radius(radius) {}

BoundingSphere::BoundingSphere(std::vector<glm::vec3> &points) {
    if (points.empty())
        return;

    SphereFromDistantPoints(this, points);

    for(auto point : points) {
        SphereOfSphereAndPt(this, point);
    }
}
