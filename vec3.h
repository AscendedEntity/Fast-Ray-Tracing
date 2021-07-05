#ifndef VEC3_H
#define VEC3_H

#include <cmath>
#include <iostream>

inline double rd()
{
    static std::uniform_real_distribution<double> distribution(0.0, 1.0);
    static std::mt19937 generator;
    return distribution(generator);
}

inline double rd(double min, double max)
{
    return min + (max - min) * rd();
}
/// <summary>
/// Custom class to handle 3D vector operations
/// </summary>
class vec3 
{
public:
    double p[3];
    
    vec3()
    {
        p[0] = 0;
        p[1] = 0;
        p[2] = 0;
    }

    vec3(double xr, double yg, double zb)
    {
        p[0] = xr;
        p[1] = yg;
        p[2] = zb;
    }

    double x() const 
    { 
        return p[0]; 
    }

    double y() const 
    { 
        return p[1]; 
    }
    
    double z() const 
    { 
        return p[2]; 
    }

    vec3 operator - () const
    { 
        return vec3(-p[0], -p[1], -p[2]); 
    }

    double operator [] (int i) const
    { 
        return p[i]; 
    }

    double& operator [] (int i) 
    { 
        return p[i]; 
    }

    vec3& operator += (const vec3& v) 
    {
        p[0] += v.p[0];
        p[1] += v.p[1];
        p[2] += v.p[2];
        return *this;
    }

    vec3& operator *= (const double t) 
    {
        p[0] *= t;
        p[1] *= t;
        p[2] *= t;
        return *this;
    }

    vec3& operator /= (const double t) 
    {
        return *this *= 1 / t;
    }

    double length() const 
    {
        return std::sqrt(length_squared());
    }

    double length_squared() const 
    {
        return p[0] * p[0] + p[1] * p[1] + p[2] * p[2];
    }

    inline static vec3 random()
    {
        return vec3(rd(), rd(), rd());
    }

    inline static vec3 random(double min, double max)
    {
        return vec3(rd(min, max), rd(min, max), rd(min, max));
    }

    bool nearZero() const 
    {
        const auto s = 1e-8;
        return (fabs(p[0]) < s) && (fabs(p[1]) < s) && (fabs(p[2]) < s);
    }
};

using point = vec3;
using colour = vec3;

inline std::ostream& operator << (std::ostream& out, const vec3& v) 
{
    return out << v.p[0] << ' ' << v.p[1] << ' ' << v.p[2];
}

inline vec3 operator + (const vec3& u, const vec3& v) 
{
    return vec3(u.p[0] + v.p[0], u.p[1] + v.p[1], u.p[2] + v.p[2]);
}

inline vec3 operator - (const vec3& u, const vec3& v) 
{
    return vec3(u.p[0] - v.p[0], u.p[1] - v.p[1], u.p[2] - v.p[2]);
}

inline vec3 operator * (const vec3& u, const vec3& v) 
{
    return vec3(u.p[0] * v.p[0], u.p[1] * v.p[1], u.p[2] * v.p[2]);
}

inline vec3 operator * (double t, const vec3& v) 
{
    return vec3(t * v.p[0], t * v.p[1], t * v.p[2]);
}

inline vec3 operator * (const vec3& v, double t) 
{
    return t * v;
}

inline vec3 operator / (vec3 v, double t) 
{
    return (1 / t) * v;
}

inline double dot(const vec3& u, const vec3& v) 
{
    return u.p[0] * v.p[0]
        + u.p[1] * v.p[1]
        + u.p[2] * v.p[2];
}

inline vec3 cross(const vec3& u, const vec3& v) 
{
    return vec3(u.p[1] * v.p[2] - u.p[2] * v.p[1],
        u.p[2] * v.p[0] - u.p[0] * v.p[2],
        u.p[0] * v.p[1] - u.p[1] * v.p[0]);
}

inline vec3 unitVector(vec3 v) 
{
    return v / v.length();
}

inline vec3 random_in_unit_sphere() 
{
    while (true) 
    {
        auto p = vec3::random(-1, 1);
        if (p.length_squared() >= 1) 
            continue;
        return p;
    }
}

vec3 randomUnitVector() 
{
    return unitVector(random_in_unit_sphere());
}

vec3 random_in_hemisphere(const vec3& normal) 
{
    vec3 in_unit_sphere = random_in_unit_sphere();
    if (dot(in_unit_sphere, normal) > 0.0)
        return in_unit_sphere;
    else
        return -in_unit_sphere;
}

vec3 reflect(const vec3& v, const vec3& n) 
{
    return v - 2 * dot(v, n) * n;
}

vec3 refract(const vec3& r_in, const vec3& n, double iot)
{
    auto cosTheta = fmin(dot(-r_in, n), 1.0);
    vec3 rPerpendicular = iot * (r_in + cosTheta * n);
    vec3 rParallel = -sqrt(fabs(1.0 - rPerpendicular.length_squared())) * n;
    return rPerpendicular + rParallel;
}

vec3 random_in_unit_disk() 
{
    while (true) 
    {
        auto p = vec3(rd(-1, 1), rd(-1, 1), 0);
        if (p.length_squared() >= 1) 
            continue;
        return p;
    }
}

#endif