#include "../include/camera.hpp"

Camera::Camera(point3 origin, double viewport_height, double aspect_ratio, double focal_length)
{
    this->origin = origin;
    this->focal_length = focal_length;
    horizontal = vector3(aspect_ratio * viewport_height, 0, 0);
    vertical = vector3(0, viewport_height, 0);
    lower_left_corner = origin - horizontal/2 - vertical/2 - vector3(0, 0, focal_length);
}

Ray Camera::rayFromCamera(double u, double v)
{
    return Ray(origin, lower_left_corner + horizontal*u + vertical*v);
}

vector3 Camera::getUpperLeftCorner()
{
    return origin - horizontal/2 + vertical/2 - vector3(0,0,focal_length);
}