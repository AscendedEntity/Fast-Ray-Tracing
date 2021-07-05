#ifndef CONST_UTILITY_H
#define CONST_UTILITY_H

#include <random>
#include <limits>
#include <memory>
#include "vec3.h"
#include "ray.h"

using std::shared_ptr;
using std::make_shared;
using std::sqrt;

const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

/// <summary>
/// Converts degrees to radians
/// </summary>
/// <param name="degrees">Degrees</param>
/// <returns>Degrees in radians</returns>
inline double radians(double degrees) 
{
    return degrees * pi / 180.0;
}

/// <summary>
/// Generates a random double in the range (0, 1)
/// </summary>
inline double randomDouble() 
{
    static std::uniform_real_distribution<double> distribution(0.0, 1.0);
    static std::mt19937 generator;
    return distribution(generator);
}

/// <summary>
/// Generates a random double in the range (min, max)
/// </summary>
/// <param name="min"></param>
/// <param name="max"></param>
inline double randomDouble(double min, double max) 
{
    return min + (max - min) * randomDouble();
}

/// <summary>
/// Generates a random int in the range (min, max)
/// </summary>
/// <param name="min"></param>
/// <param name="max"></param>
inline int randomInt(int min, int max)
{
    return static_cast<int>(randomDouble(min, max + 1));
}

/// <summary>
/// Restrics value of x in (min, max)
/// </summary>
/// <param name="x">Variable</param>
/// <param name="min">Least value of x</param>
/// <param name="max">Most value of x</param>
inline double clamp(double x, double min, double max) {
    if (x < min) 
        return min;
    if (x > max) 
        return max;
    return x;
}

#endif