#pragma once

#include <memory>
#include "materials.hpp"
#include "vector3.hpp"

class Material;

struct hit_record
{
    vector3 normal;
    double t;
    bool front_face;
    std::shared_ptr<Material> material;
    colour col;
};