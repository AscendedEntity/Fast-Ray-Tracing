#ifndef HITTABLE_H
#define HITTABLE_H

#include "ray.h"
#include "const_utility.h"
#include "bounding_box.h"

class Material;

/// <summary>
/// Stores the data about the intersection of the ray and object
/// </summary>
struct hitRecord
{
    point p;
    vec3 normal;
    double t;
    bool frontFace;
    shared_ptr<Material> mat_ptr;

    inline void setFaceNormal(const Ray& r, const vec3& outwardNormal) 
    {
        frontFace = dot(r.direction, outwardNormal) < 0;
        normal = frontFace ? outwardNormal : -outwardNormal;
    }
};

/// <summary>
/// Describes the traits of an object in the scene
/// </summary>
class Hittable 
{
public:
    /// <summary>
    /// Function used to check if the ray intersects with the object
    /// </summary>
    /// <param name="r">Reference to the ray object</param>
    /// <param name="tMin">Minimum value of t in A + Bt</param>
    /// <param name="tMax">Maximum value of t in A + Bt</param>
    /// <param name="rec">Reference to the record to store the intersection data</param>
    /// <returns>True, if the ray and object intersect</returns>
    virtual bool hit(const Ray& r, double tMin, double tMax, hitRecord& rec) const = 0;
    /// <summary>
    /// Gets the reference to the bounding box
    /// </summary>
    /// <param name="output">Reference to the bounding box variable</param>
    /// <returns>The bounding box of the object</returns>
    virtual bool boundingBox(BoundingBox& output) const = 0;
};

#endif