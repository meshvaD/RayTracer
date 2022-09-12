#include "../include/vector3.hpp"

#include <iostream>
#include <math.h>
#include <random>

vector3 vector3::operator+=(vector3 y)
{
    x[0] += y.x[0];
    x[1] += y.x[1];
    x[2] += y.x[2];

    return *this;
}

vector3 vector3::operator*=(const double c)
{
    x[0] *= c;
    x[1] *= c;
    x[2] *= c;

    return *this;
}

vector3 vector3::operator/=(const double c)
{
    return operator*=(1/c);
}

void vector3::print()
{
    std::cout << int(x[0] * 255.999) << ' ' << int(x[1] * 255.999) << ' ' << int(x[2] * 255.999) << '\n';
}

vector3 unit_vector(vector3 a)
{
    double mag = pow(pow(a.x[0], 2) + pow(a.x[1], 2) + pow(a.x[2], 2) , 0.5);
    return vector3(a.x[0]/mag, a.x[1]/mag, a.x[2]/mag);
}

double dot(vector3 a, vector3 b)
{
    return a.x[0]*b.x[0] + 
            a.x[1]*b.x[1] + 
            a.x[2]*b.x[2];
}

vector3 cross(vector3 a, vector3 b)
{
    return vector3(a.x[1] * b.x[2] - a.x[2] * b.x[1],
                    - a.x[0] * b.x[2] + a.x[2] * b.x[0],
                    + a.x[0] * b.x[1] - a.x[1] * b.x[0]);
}

double magnitude(vector3 a)
{
    return sqrt(a.x[0] * a.x[0] + a.x[1] * a.x[1] + a.x[2] * a.x[2]);
}

double random_bounds(double min, double max)
{
    int full_rand = rand();
    double in_range = min + (max-min)*rand()/(RAND_MAX+1.0);
    return in_range;
}

colour clamp(colour c)
{
    for (int i=0; i<3; i++)
    {
        if (c.x[i] < 0)
            c.x[i] = 0;
        else if (c.x[i] > 1)
            c.x[i] = 1;
    }
    return c;
}

colour randColour()
{
    return colour(random_bounds(0,1), random_bounds(0,1), random_bounds(0,1));
}