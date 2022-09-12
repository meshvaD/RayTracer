#pragma once

class vector3
{
public:
    vector3() : x{0,0,0} {}
    vector3 (double x1, double x2, double x3) : x{x1, x2, x3} {}

    //add, mulitply, divide operators
    vector3 operator+=(vector3 y);
    vector3 operator*=(const double c);
    vector3 operator/=(const double c);

    //print, output colour
    void print();

public:
    double x[3];
};

//aliases
using point3 = vector3;
using colour = vector3;

//static
double dot(vector3 a, vector3 b);
vector3 cross(vector3 a, vector3 b);

colour clamp(colour c);
colour randColour();

vector3 unit_vector(vector3 a);
double random_bounds(double min, double max);
double magnitude(vector3 a);

inline vector3 operator+(const vector3& a, const vector3& b)
{
    return vector3(a.x[0] + b.x[0], a.x[1] + b.x[1], a.x[2] + b.x[2]);
}
inline vector3 operator+(const double c, const vector3& a)
{
    return a + vector3(c,c,c);
}

inline vector3 operator-(const vector3& a, const vector3& b)
{
    return vector3(a.x[0] - b.x[0], a.x[1] - b.x[1], a.x[2] - b.x[2]);
}
inline vector3 operator-(const double c, const vector3& a)
{
    return a - vector3(c,c,c);
}

inline vector3 operator*(const vector3& a, const double c)
{
    return vector3(a.x[0]*c, a.x[1]*c, a.x[2]*c);
}
inline vector3 operator*(const vector3& a, const vector3& b)
{
    return vector3(a.x[0] * b.x[0], a.x[1] * b.x[1], a.x[2] * b.x[2]);
}

inline vector3 operator/(const vector3& a, const double c)
{
    return a * (1/c);
}

inline bool operator==(const vector3& a, const vector3& b)
{
    if (a.x[0] == b.x[0] && a.x[1] == b.x[1] && a.x[2] == b.x[2])
        return true;
    return false;
}

inline bool operator!=(const vector3& a, const vector3& b)
{
    if (a.x[0] == b.x[0] && a.x[1] == b.x[1] && a.x[2] == b.x[2])
        return false;
    return true;
}