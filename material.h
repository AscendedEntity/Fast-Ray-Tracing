#ifndef MATERIAL_H
#define MATERIAL_H

#include "const_utility.h"

struct hitRecord;

/// <summary>
/// Parent class of all object materials
/// </summary>
class Material 
{
public:
    /// <summary>
    /// Determins the reflected ray
    /// </summary>
    /// <param name="r_in">Reference to the incoming ray</param>
    /// <param name="rec">Record to store the details of the interaction between the ray and the object</param>
    /// <param name="attenuation">Colour of the object</param>
    /// <param name="scattered">Reference to the scattered ray</param>
    /// <returns>True, if a reflected ray is created</returns>
    virtual bool scatter(const Ray& r_in, const hitRecord& rec, colour& attenuation, Ray& scattered) const = 0;
    /// <summary>
    /// Determines the colour emitted by the light source
    /// </summary>
    /// <returns>The colour of the light emitted by the material</returns>
    virtual colour emitted() const
    {
        return colour(0.0, 0.0, 0.0);
    }
};

/// <summary>
/// Defines the behaviour of a matte like material
/// </summary>
class Lambertian : public Material 
{
public:
    colour albedo;

    Lambertian(const colour& a)
    {
        albedo = a;
    }

    virtual bool scatter(const Ray& r_in, const hitRecord& rec, colour& attenuation, Ray& scattered) const override 
    {
        auto scatterDir = rec.normal + randomUnitVector();

        if (scatterDir.nearZero())
            scatterDir = rec.normal;

        scattered = Ray(rec.p, scatterDir);
        attenuation = albedo;
        return true;
    }
};

/// <summary>
/// Defines the behaviour of a metal
/// </summary>
class Metal : public Material 
{
public:
    colour albedo;
    double fuzz;
    Metal(const colour& a, double f)
    {
        albedo = a;
        fuzz = f;
    }

    virtual bool scatter(const Ray& r_in, const hitRecord& rec, colour& attenuation, Ray& scattered) const override 
    {
        vec3 reflected = reflect(unitVector(r_in.direction), rec.normal);
        scattered = Ray(rec.p, reflected + fuzz * random_in_unit_sphere());
        attenuation = albedo;
        return (dot(scattered.direction, rec.normal) > 0);
    }
};

/// <summary>
/// Defines the behaviour of a dielectric
/// </summary>
class Dielectric : public Material 
{
private:
    // Schlick's approximation for reflectance
    static double reflectance(double cosine, double ref_idx) 
    {
        auto r0 = (1 - ref_idx) / (1 + ref_idx);
        r0 = r0 * r0;
        return r0 + (1 - r0) * pow((1 - cosine), 5);
    }
public:
    double ir;
    Dielectric(double index_of_refraction)
    {
        ir = index_of_refraction;
    }

    virtual bool scatter(const Ray& r_in, const hitRecord& rec, colour& attenuation, Ray& scattered) const override 
    {
        attenuation = colour(1.0, 1.0, 1.0);
        double refractionRatio = rec.frontFace ? (1.0 / ir) : ir;

        vec3 unitDir = unitVector(r_in.direction);

        double cosTheta = fmin(dot(-unitDir, rec.normal), 1.0);
        double sinTheta = sqrt(1.0 - cosTheta * cosTheta);

        bool canRefract = refractionRatio * sinTheta <= 1.0;
        vec3 direction;

        if (!canRefract || reflectance(cosTheta, refractionRatio) > randomDouble())
            direction = reflect(unitDir, rec.normal);
        else
            direction = refract(unitDir, rec.normal, refractionRatio);

        scattered = Ray(rec.p, direction);
        return true;
    }
};

/// <summary>
/// Defines the behaviour of a light source
/// </summary>
class Light : public Material
{
public:
    colour emit;

    Light(colour colour)
    {
        emit = colour;
    }

    virtual bool scatter(const Ray& r, const hitRecord& record, colour& attenuation, Ray& scattered) const override 
    {
        return false;
    }

    virtual colour emitted() const override
    {
        return emit;
    }
};

/// <summary>
/// Defines the behaviour of a metal which is also an emitter
/// </summary>
class MetalEmitter : public Material
{
public:
    colour albedo;
    double fuzz;
    MetalEmitter(const colour& a, double f)
    {
        albedo = a;
        fuzz = f;
    }

    virtual bool scatter(const Ray& r_in, const hitRecord& rec, colour& attenuation, Ray& scattered) const override
    {
        vec3 reflected = reflect(unitVector(r_in.direction), rec.normal);
        scattered = Ray(rec.p, reflected + fuzz * random_in_unit_sphere());
        attenuation = albedo;
        return (dot(scattered.direction, rec.normal) > 0);
    }

    virtual colour emitted() const override
    {
        return albedo;
    }
};

/// <summary>
/// Defines the behaviour of a matte like material which is also an emitter
/// </summary>
class LambertianEmitter : public Material 
{
public:
    colour albedo;

    LambertianEmitter(const colour& a)
    {
        albedo = a;
    }

    virtual bool scatter(const Ray& r_in, const hitRecord& rec, colour& attenuation, Ray& scattered) const override 
    {
        auto scatterDir = rec.normal + randomUnitVector();

        if (scatterDir.nearZero())
            scatterDir = rec.normal;

        scattered = Ray(rec.p, scatterDir);
        attenuation = albedo;
        return true;
    }

    virtual colour emitted() const override
    {
        return albedo;
    }
};
#endif