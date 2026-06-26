#include "AABB.h"

bool AABB::intersect(const Ray& ray) const
{
    float tx1 = (min.x - ray.origin.x) / ray.direction.x;
    float tx2 = (max.x - ray.origin.x) / ray.direction.x;

    float tmin = std::min(tx1, tx2);
    float tmax = std::max(tx1, tx2);

    float ty1 = (min.y - ray.origin.y) / ray.direction.y;
    float ty2 = (max.y - ray.origin.y) / ray.direction.y;

    tmin = std::max(tmin, std::min(ty1, ty2));
    tmax = std::min(tmax, std::max(ty1, ty2));

    float tz1 = (min.z - ray.origin.z) / ray.direction.z;
    float tz2 = (max.z - ray.origin.z) / ray.direction.z;

    tmin = std::max(tmin, std::min(tz1, tz2));
    tmax = std::min(tmax, std::max(tz1, tz2));

    return tmax >= tmin && tmax > 0.0f;
}