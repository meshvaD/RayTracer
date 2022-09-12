#include "vector3.hpp"
#include "ray.hpp"

class Camera
{
public:
    Camera(point3 origin, double viewport_height, double aspect_ratio, double focal_length);
    Ray rayFromCamera(double u, double v);
    vector3 getUpperLeftCorner();

    vector3 horizontal;
    vector3 vertical;

private:
    point3 origin;
    vector3 lower_left_corner;

    double focal_length;
};