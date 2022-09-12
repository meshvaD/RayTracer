#include "../include/hittables.hpp"
#include "../include/vector3.hpp"
#include <iostream>

#include <math.h>
#include <limits>
#include <string>

using namespace cv;

void Hittables::add(std::shared_ptr<Hittable> shape)
{
    hit_list.push_back(shape);
}

bool Hittables::intersect(Ray& ray, double t_max, hit_record& record) const
{
    double closest = t_max;
    bool hit = false;

    for (auto shape_ptr : hit_list)
    {
        if (shape_ptr->intersect(ray, closest, record))
        {
            hit = true;
            closest = record.t;
            record.material = shape_ptr->material;
        }
    }
    return hit;
}   

bool Sphere::intersect(Ray& ray, double t_max, hit_record& record) const
{
    vector3 o_c = ray.origin() - center;

    double a = dot(ray.direction(), ray.direction());
    double b = 2.0 * dot(ray.direction(), o_c);
    double c = dot(o_c, o_c) - radius*radius;

    double discrim = b*b - 4*a*c;

    if (discrim < 0)
        return false;
    
    double root = (-b - sqrt(discrim))/ (2.0*a); //one root of quadratic

    if(root < 0.01 || root > t_max) //only possitive root within range
        return false;

    // check if poi within theta/ phi bounds
    point3 poi = ray.at(root);
    double curr_theta = atan2(poi.x[0] - center.x[0], poi.x[2] - center.x[2]); // = tan^-1(z/x);
    curr_theta = std::fmod(curr_theta + 2*PI, 2*PI);

    double curr_phi = acos(poi.x[1]/radius);

    if (curr_theta > theta_max || curr_theta < theta_min || curr_phi > phi_max || curr_phi < phi_min)
        return false;

    record.t = root;
    
    //get vector facing OUT
    int multiplier = radius > 0 ? 1 : -1; //for hollow sphere, opposite normal

    vector3 n_out = unit_vector(ray.at(record.t) - center);
    if (dot(n_out, ray.direction()) > 0)
    {
        record.normal = n_out * -1 * multiplier;
        record.front_face = false;
    }
    else
    {
        record.normal = n_out * multiplier;
        record.front_face = true;
    }    

    colour col = material->col;
    if (material->im_path != "")
    {
        int new_x = (poi.x[0] - center.x[0]) * img.cols/radius/2 + img.cols/2;
        int new_y = (poi.x[1] - center.x[1]) * img.rows/radius/2 + img.rows/2;
        if (new_x <= img.cols && new_y <= img.rows && new_x >= 0 && new_y >= 0)
        {
            Vec3b bgr = img.at<Vec3b>(new_x, new_y);
            col = colour(bgr.val[2]/255., bgr.val[1]/255., bgr.val[0]/255.);

            // std::cerr << "path exists and in bounds: " << new_x <<","<< new_y << '\n' << std::flush;
        }
    }

    record.col = col;
    return true;
}

void RecPlane::solvePlaneEq()
{
    vector3 normal = unit_vector(cross(v1-v3, v2-v3));

    a = normal.x[0];
    b = normal.x[1];
    c = normal.x[2];
    d = -a*v1.x[0] + -b*v1.x[1] + -c*v1.x[2];

    std::cerr << a << ", " << b << ", " << c << ", " << d << std::endl;
}

bool RecPlane::inConstBounds(double a, double b1, double b2) const
{
    if (a <= b1 && a >= b2)
        return true;
    if (a > b1 && a < b2)
        return true;
    return false;
}

bool RecPlane::inBounds(double x, double y, double z) const
{
    bool x_bnds = !inConstBounds(x, v1.x[0], v2.x[0]) && !inConstBounds(x, v3.x[0], v2.x[0]);
    bool y_bnds = !inConstBounds(y, v1.x[1], v2.x[1]) && !inConstBounds(y, v3.x[1], v2.x[1]);
    bool z_bnds = !inConstBounds(z, v1.x[2], v2.x[2]) && !inConstBounds(z, v3.x[2], v2.x[2]);

    if (x_bnds || y_bnds || z_bnds)
        return false;
    return true;
}

double RecPlane::get_scale() const
{
    double x = std::abs(v1.x[0] - v2.x[0]);
    if (std::abs(v3.x[0] - v2.x[0]) > x)
        x = std::abs(v3.x[0] - v2.x[0]);

    double y = std::abs(v1.x[1] - v2.x[1]);
    if (std::abs(v3.x[1] - v2.x[1]) > y)
        x = std::abs(v3.x[1] - v2.x[1]);
        
    double z = std::abs(v1.x[2] - v2.x[2]);
    if (std::abs(v3.x[2] - v2.x[2]) > z)
        x = std::abs(v3.x[2] - v2.x[2]);

    if (x > y && x > z)
        return x;
    if (y > x && y > z)
        return y;
    return z;
}

std::vector<std::vector<double>>* RecPlane::get_center() const
{
    double other_x = v3.x[0];
    if (v1.x[0] != v2.x[0])
        other_x = v2.x[0];

    double other_y = v3.x[1];
    if (v1.x[1] != v2.x[1])
        other_y = v2.x[1];

    double other_z = v3.x[2];
    if (v1.x[2] != v2.x[2])
        other_z = v2.x[2];

    std::vector<std::vector<double>>* v = new std::vector<std::vector<double>>();

    if (v1.x[0] - other_x != 0)
        v->push_back({0, (v1.x[0]-other_x)/2});
    if (v1.x[1] - other_y != 0)
        v->push_back({1,(v1.x[1]-other_y)/2});
    if (v1.x[2] - other_z != 0)
        v->push_back({2,(v1.x[2]-other_z)/2 - 1});

    return v;
}

bool RecPlane::intersect(Ray& ray, double t_max, hit_record& record) const
{
    vector3 line = ray.direction();
    vector3 origin = ray.origin();

    if (a * line.x[0] + b * line.x[1] + c * line.x[2] == 0)
        return false;

    double t = -(a * origin.x[0] + b * origin.x[1] + c * origin.x[2] + d) / (a * line.x[0] + b * line.x[1] + c * line.x[2]);
    point3 poi = ray.at(t);

    // std::cerr << poi.x[0] << ", " << poi.x[1] << ", " << poi.x[2] << std::endl;

    if (!inBounds(poi.x[0], poi.x[1], poi.x[2]))
        return false;

    //t close to 0
    if (t <= 1e-3)
        return false;
        
    record.t = t;
    vector3 normal = vector3(a,b,c);

    if (dot(record.normal, ray.direction()) > 0)
    {
        return false;
        record.normal = normal * -1 * hollow;
        record.front_face = false;
    }
    else
    {
        record.normal = normal * hollow;
        record.front_face = true;
    }

    double scale = get_scale();
    std::vector<std::vector<double>>* centers = get_center();

    // std::cerr << poi.x[int((*centers)[0][0])] << " , " << poi.x[int((*centers)[1][0])] << std::endl;

    colour col = material->col;
    if (material->im_path != "")
    {
        int new_x = (poi.x[int((*centers)[0][0])] - (*centers)[0][1]) * img.cols/scale/2 + img.cols/2;
        int new_y = (poi.x[int((*centers)[1][0])] - (*centers)[1][1]) * img.rows/scale/2 + img.rows/2;

        // std::cerr << scale << " , " << poi.x[int((*centers)[0][0])] - (*centers)[0][1] << " , " << poi.x[int((*centers)[1][0])] - (*centers)[1][1] << std::endl;

        if (new_x <= img.cols && new_y <= img.rows && new_x >= 0 && new_y >= 0)
        {
            // std::cerr << "here" << std::endl;
            Vec3b bgr = img.at<Vec3b>(new_x, new_y);
            col = colour(bgr.val[2]/255., bgr.val[1]/255., bgr.val[0]/255.);

            // std::cerr << "path exists and in bounds: " << new_x <<","<< new_y << '\n' << std::flush;
        }
    }
    record.col = col;
    return true;
}

Rectangle::Rectangle(point3 front_top_left, point3 back_bottom_right, std::shared_ptr<Material> material, bool hollow)
{
    point3 ftl = front_top_left;
    point3 bbr = back_bottom_right;

    this->material = material;

    if (hollow)
        this->hollow = -1;
    else
        this->hollow = 1;

    sides.push_back(RecPlane(ftl, point3(ftl.x[0], ftl.x[1], bbr.x[2]), point3(ftl.x[0], bbr.x[1], bbr.x[2]), material, hollow)); //left
    sides.push_back(RecPlane(bbr, point3(bbr.x[0], ftl.x[1], bbr.x[2]), point3(bbr.x[0], bbr.x[1], ftl.x[2]), material, hollow)); //right

    sides.push_back(RecPlane(ftl, point3(ftl.x[0], bbr.x[1], ftl.x[2]), point3(bbr.x[0], bbr.x[1], ftl.x[2]), material, hollow)); //front
    sides.push_back(RecPlane(bbr, point3(bbr.x[0], ftl.x[1], bbr.x[2]), point3(ftl.x[0], ftl.x[1], bbr.x[2]), material, hollow)); //back

    sides.push_back(RecPlane(ftl, point3(ftl.x[0], ftl.x[1], bbr.x[2]), point3(bbr.x[0], ftl.x[1], ftl.x[2]), material, hollow)); //top
    sides.push_back(RecPlane(bbr, point3(bbr.x[0], bbr.x[1], ftl.x[2]), point3(ftl.x[0], bbr.x[1], ftl.x[2]), material, hollow)); //bottom
    
}

bool Rectangle::intersect(Ray& ray, double t_max, hit_record& record) const
{
    for (int i=0; i<sides.size(); i++)
    {
        if (sides[i].intersect(ray, t_max, record))
            return true;
    }
    return false;
}

bool TriPlane::inBounds(double x, double y, double z) const
{
    //make sure the point is on "proper side" of each traingle edge by comparing to other triangle vertice
    if (onSameSide(point3(x,y,z), v1, v2, v3) && onSameSide(point3(x,y,z), v2, v1, v3) && onSameSide(point3(x,y,z), v3, v1, v2))
        return true;
    return false;
}

//p2 is reference that is supposed to be on proper side, p1 is point being compared, a/b are other 3 vertices
double TriPlane::onSameSide(point3 p1, point3 p2, point3 a, point3 b) const
{
    vector3 cross1 = cross(b-a, p1-a);
    vector3 cross2 = cross(b-a, p2-a);

    //cross products in same direction (same sign) if p1 and p2 on same side
    if (dot(cross1, cross2) >= 0)
        return true;
    return false;
}