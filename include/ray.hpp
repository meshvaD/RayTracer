#pragma once

#include "vector3.hpp"

//format origin + x * dir

class Ray
{
public:
    Ray (const point3& origin, const vector3& direction) : 
        orig(origin), dir(direction) {};

    point3 at (double t)
    {
        return orig + (dir * t);
    }

    vector3 direction() const {return dir;}
    point3 origin() const {return orig;}

private:
    point3 orig;
    vector3 dir;
};