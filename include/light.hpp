#pragma once

#include "vector3.hpp"
#include "ray.hpp"

#include <vector>
#include <memory>

// class Light 
// {
// public:
//     virtual colour getLight(Ray& ray);
// };

class Light
{
public:
    Light(point3 center, double scaling, colour col=vector3(1,1,1)) 
        : center(center), col(col), scaling(scaling) {};
    colour getLight(Ray& ray);

private:
    point3 center;
    colour col;
    double scaling;
};

class Lights
{
public:
    void add(std::shared_ptr<Light> light);
    colour getLight(Ray& ray);

private:
    std::vector<std::shared_ptr<Light>> light_list;
};