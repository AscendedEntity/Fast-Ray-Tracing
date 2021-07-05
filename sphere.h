#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.h"
#include "vec3.h"

/// <summary>
/// Contains all functions to create and manage a sphere
/// </summary>
class Sphere : public Hittable 
{
public:
    /// <summary>
    /// Center of the sphere
    /// </summary>
    point center;
    /// <summary>
    /// Radius of the sphere
    /// </summary>
    double radius;
    /// <summary>
    /// Material of the sphere
    /// </summary>
    shared_ptr<Material> mat_ptr;

    /// <summary>
    /// Parameterized constructor
    /// </summary>
    /// <param name="cen">Center of the sphere</param>
    /// <param name="r">Radius of the sphere</param>
    /// <param name="m">Material of the sphere</param>
    Sphere(point cen, double r, shared_ptr<Material> m)
    {
        center = cen;
        radius = r;
        mat_ptr = m;
    }

    virtual bool hit(const Ray& r, double tMin, double tMax, hitRecord& rec) const override;
    virtual bool boundingBox(BoundingBox& output) const override;
};

bool Sphere::hit(const Ray& r, double tMin, double tMax, hitRecord& rec) const 
{
    vec3 oc = r.origin - center;
    auto a = r.direction.length_squared();
    auto halfB = dot(oc, r.direction);
    auto c = oc.length_squared() - radius * radius;

    auto discriminant = halfB * halfB - a * c;
    if (discriminant < 0) 
        return false;

    auto sqrtD = sqrt(discriminant);

    auto root = (-halfB - sqrtD) / a;
    if (root < tMin || tMax < root) 
    {
        root = (-halfB + sqrtD) / a;
        if (root < tMin || tMax < root)
            return false;
    }

    rec.t = root;
    rec.p = r.at(rec.t);
    vec3 outwardNormal = (rec.p - center) / radius;
    rec.setFaceNormal(r, outwardNormal);
    rec.mat_ptr = mat_ptr;

    return true;
}

bool Sphere::boundingBox(BoundingBox& output) const 
{
    output = BoundingBox(center - vec3(radius, radius, radius), center + vec3(radius, radius, radius));
    return true;
}
#endif