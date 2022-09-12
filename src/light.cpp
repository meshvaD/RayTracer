#include "../include/light.hpp"

colour Light::getLight(Ray& ray)
{
    vector3 x_1 = ray.origin(); //x1, x2 pt on line
    vector3 x_2 = ray.origin() + ray.direction();
    vector3 x_0 = center;

    double dist = magnitude(cross(x_0 - x_1, x_0 - x_2)) / magnitude((x_2 - x_1));
    // std::cerr << dist << '\n';
    double c = 1-dist/scaling;

    return clamp(col * c);
}

colour Lights::getLight(Ray& ray)
{
    colour col = colour(0,0,0);
    for (int i=0; i<light_list.size(); i++)
    {
        col += light_list[i]->getLight(ray);
    }
    return col;
}

void Lights::add(std::shared_ptr<Light> light)
{
    light_list.push_back(light);
}