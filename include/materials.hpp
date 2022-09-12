#pragma once

#include <string>
#include "vector3.hpp"
#include "ray.hpp"

struct hit_record; //circular dependency : materials <-> record
#include "record.hpp"

//each material returns reflected/refracted ray + curr pixel colour
class Material 
{
public:
    virtual Ray scatter(Ray& ray, hit_record& record)=0;
    colour col;
    std::string im_path;
};

//linear gradient between 2 cols -- not a material, does not scatter ray, only gets colour
class Gradient
{
public:
    Gradient(colour s, colour e) : start(s), end(e) {};
    colour get_colour(Ray& ray);

private:
    colour start;
    colour end;
};

class Diffuse : public Material
{
public:
    Diffuse(colour col=colour(0,0,0), std::string im_path="") 
    {
        this->col = col;
        this->im_path = im_path;
    };
    Ray scatter(Ray& ray, hit_record& record) override;
};

class Metal : public Material
{
public:
    Metal(colour col=colour(0,0,0), std::string im_path="", double fuzz=0) : fuzz(fuzz < 1 ? fuzz : 1)
    {
        this->col = col;
        this->im_path = im_path;
    }
    Ray scatter(Ray& ray, hit_record& record) override;
private:
    double fuzz;
};

class Glass : public Material
{
public:
    Glass(double ir=1.5) : ir(ir)
    {
        this->col = colour(1,1,1);
    }
    Ray scatter(Ray& ray, hit_record& record) override;
private:
    double ir; //of material -- (air is 1.0)
    double reflectance (double cos, double ref_idx);
};

vector3 random_in_unit_sphere();
vector3 refract(double cos_theta_in, vector3 in, vector3 normal, double index_refrac);
vector3 reflect(vector3 normal, vector3 ray_dir);