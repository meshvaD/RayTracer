#pragma once

#include <vector>
#include <map>
#include <memory>
#include <string>

#include "ray.hpp"
#include "record.hpp"

#define PI 3.14159265

#include <opencv2/opencv.hpp>

class Hittable 
{
public:
    virtual bool intersect(Ray& ray, double t_max, hit_record& record) const = 0;
    std::shared_ptr<Material> material;
};

class Hittables : public Hittable
{
public:
    void add(std::shared_ptr<Hittable> shape);
    virtual bool intersect(Ray& ray, double t_max, hit_record& record) const override; 

private:
    std::vector<std::shared_ptr<Hittable>> hit_list;
};

class Sphere : public Hittable
{
public:
    Sphere(double r, double x, double y, double z, std::shared_ptr<Material> material, double theta_min=0, double theta_max=2*PI, double phi_min=0, double phi_max=PI) 
        : radius(r), center(vector3(x,y,z)), theta_min(theta_min), theta_max(theta_max), phi_min(phi_min), phi_max(phi_max)
    {
        this->material = material;  
        if (material->im_path != "")
            img = cv::imread(material->im_path, cv::IMREAD_UNCHANGED);          
    };

    virtual bool intersect(Ray& ray, double t_max, hit_record& record) const override;

private:
    double radius;
    vector3 center;

    double theta_min;
    double theta_max;
    double phi_min;
    double phi_max;

    cv::Mat img;

};

class RecPlane : public Hittable
{
public:
    RecPlane(point3 v1, point3 v2, point3 v3, std::shared_ptr<Material> material, int hollow)
        : v1(v1), v2(v2), v3(v3), hollow(hollow)
    {
        this->material = material;
        if (material->im_path != "")
            img = cv::imread(material->im_path, cv::IMREAD_UNCHANGED);
        solvePlaneEq();
    }
    virtual bool intersect(Ray& ray, double t_max, hit_record& record) const override;

    point3 v1;
    point3 v2;
    point3 v3;
private:
    double a;
    double b;
    double c;
    double d;

    int hollow;

    cv::Mat img;

    void solvePlaneEq();
    virtual bool inBounds(double x, double y, double z) const;
    bool inConstBounds(double a, double b1, double b2) const;
    double get_scale() const;
    std::vector<std::vector<double>>* get_center() const;
};

class Rectangle : public Hittable
{
public:
    Rectangle(point3 front_top_left, point3 back_bottom_right, std::shared_ptr<Material> material, bool hollow=false);

    virtual bool intersect(Ray& ray, double t_max, hit_record& record) const override;

private:
    std::vector<RecPlane> sides;
    int hollow;
};

class TriPlane : public RecPlane
{  
using RecPlane::RecPlane;  
private:
    bool inBounds(double x, double y, double z) const override;
    double onSameSide(point3 p1, point3 a, point3 b, point3 c) const;
};