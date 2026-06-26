#pragma once

#include <glm/glm.hpp>
#include "Ray.h"

struct AABB
{
    glm::vec3 min;
    glm::vec3 max;

    bool intersect(const Ray& ray) const;
};