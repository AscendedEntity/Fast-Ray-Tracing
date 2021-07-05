#ifndef RAY_H
#define RAY_H

#include "vec3.h"

/// <summary>
/// Contains all the functions used to create and manage a light ray
/// </summary>
class Ray 
{
public:
    /// <summary>
    /// Point of origin of the ray
    /// </summary>
    point origin;
    /// <summary>
    /// Direction of the ray
    /// </summary>
    vec3 direction;
    
    /// <summary>
    /// Default constructor
    /// </summary>
    Ray() 
    {}

    /// <summary>
    /// Parameterized constructor
    /// </summary>
    /// <param name="orig">Point of origin of the ray</param>
    /// <param name="dir">Direction of the ray</param>
    Ray(const point& orig, const vec3& dir)
    {
        origin = orig;
        direction = dir;
    }

    /// <summary>
    /// Finds the point at A + Bt
    /// </summary>
    /// <param name="t"></param>
    /// <returns>Point at A + Bt</returns>
    point at(double t) const 
    {
        return origin + t * direction;
    }
};

#endif

