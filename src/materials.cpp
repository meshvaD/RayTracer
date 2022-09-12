#include "../include/materials.hpp"
#include <limits>
#include <math.h>
#include <iostream>

colour Gradient::get_colour(Ray& ray)
{
    //blendedValue=(1−t)⋅startValue+t⋅endValue
    //scale ray vec from 0 - 1
    vector3 unit = unit_vector(ray.direction());

    double t = (1.0 + unit.x[1]) * 0.5;
    return start*(1.0-t) + end*t;
}

Ray Diffuse::scatter(Ray& ray, hit_record& record)
{
    point3 poi = ray.at(record.t);
    vector3 offseted_normal = record.normal + unit_vector(random_in_unit_sphere());
    Ray s(poi, offseted_normal);

    return s;
}

vector3 random_in_unit_sphere()
{
    while(true)
    {
        double x = random_bounds(-1,1);
        double y = random_bounds(-1,1);
        double z = random_bounds(-1,1);

        if (x*x + y*y + z*z <= 1)
            return vector3(x,y,z);
    }
}

Ray Metal::scatter(Ray& ray, hit_record& record)
{
    point3 poi = ray.at(record.t);
    vector3 reflected = reflect(record.normal, ray.direction());
    Ray s(poi, reflected + unit_vector(random_in_unit_sphere()) * fuzz);

    return s;
}

Ray Glass::scatter(Ray& ray, hit_record& record)
{
    vector3 unit_direction = unit_vector(ray.direction());
    double cos_theta = std::fmin(dot(unit_direction*-1, record.normal), 1.0);

    double sin_theta = sqrt(1 - cos_theta*cos_theta);
    double index_refrac = record.front_face ? (1.0/ir) : ir;
    
    vector3 dir;
    if (sin_theta * index_refrac > 1.0 || reflectance(cos_theta, index_refrac) > random_bounds(0, 1))
        dir = reflect(record.normal, unit_direction);
    else
        dir = refract(cos_theta, unit_direction, record.normal, index_refrac);

    return Ray(ray.at(record.t), dir);
}

double Glass::reflectance(double cos, double ref_idx) //sclick approx for reflectance
{
    auto r0 = (1-ref_idx) / (1+ref_idx);
    r0 = r0*r0;
    return r0 + (1-r0)*pow((1 - cos),5);
}

//in = incident, out=refracted ray
vector3 refract(double cos_theta_in, vector3 uv, vector3 n, double ir)
{
    vector3 r_out_perp =  (uv + n*cos_theta_in) * ir;
    vector3 r_out_parallel = n * -sqrt(fabs(1.0 - pow(magnitude(r_out_perp),2)));
    return r_out_perp + r_out_parallel;
}

vector3 reflect(vector3 normal, vector3 ray_dir)
{
    return normal*(-2*dot(ray_dir, normal)) + ray_dir;
}

