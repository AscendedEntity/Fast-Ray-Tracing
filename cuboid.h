#include "hittable.h"
#include "vec3.h"

/// <summary>
/// Contains all funcitons used to create and manage xy planes
/// </summary>
class xyPlane : public Hittable
{
public:
	double x0, x1, y0, y1, k;
	shared_ptr<Material> mat_ptr;

	xyPlane()
	{}

	xyPlane(double x0, double x1, double y0, double y1, double k, shared_ptr<Material>mat_ptr)
	{
		this->x0 = x0;
		this->x1 = x1;
		this->y0 = y0;
		this->y1 = y1;
		this->k = k;
		this->mat_ptr = mat_ptr;
	}

	virtual bool hit(const Ray& r, double t_min, double t_max, hitRecord& rec) const override;
    virtual bool boundingBox(BoundingBox& output) const override
    {
        output = BoundingBox(point(x0, y0, k - 0.0001), point(x1, y1, k + 0.0001));
        return true;
    }
};

bool xyPlane::hit(const Ray& r, double tMin, double tMax, hitRecord& rec) const 
{
    auto t = (k - r.origin.z()) / r.direction.z();
    
    if (t < tMin || t > tMax)
        return false;
    
    auto x = r.origin.x() + t * r.direction.x();
    auto y = r.origin.y() + t * r.direction.y();

    if (x < x0 || x > x1 || y < y0 || y > y1)
        return false;

    rec.t = t;
    rec.setFaceNormal(r, vec3(0.0, 0.0, 1.0));
    rec.mat_ptr = mat_ptr;
    rec.p = r.at(t);
    return true;
}

/// <summary>
/// Contains all functions used to create and manage yz planes
/// </summary>
class yzPlane : public Hittable
{
public:
    double y0, y1, z0, z1, k;
    shared_ptr<Material> mat_ptr;

    yzPlane()
    {}

    yzPlane(double y0, double y1, double z0, double z1, double k, shared_ptr<Material> mat_ptr)
    {
        this->y0 = y0;
        this->y1 = y1;
        this->z0 = z0;
        this->z1 = z1;
        this->k = k;
        this->mat_ptr = mat_ptr;
    }

    virtual bool hit(const Ray& r, double t_min, double t_max, hitRecord& rec) const override;
    virtual bool boundingBox(BoundingBox& output) const override
    {
        output = BoundingBox(point(k - 0.0001, y0, z0), point(k + 0.0001, y1, z1));
        return true;
    }
};

bool yzPlane::hit(const Ray& r, double tMin, double tMax, hitRecord& rec) const
{
    auto t = (k - r.origin.x()) / r.direction.x();

    if (t < tMin || t > tMax)
        return false;

    auto y = r.origin.y() + t * r.direction.y();
    auto z = r.origin.z() + t * r.direction.z();

    if (y < y0 || y > y1 || z < z0 || z > z1)
        return false;

    rec.t = t;
    rec.setFaceNormal(r, vec3(1.0, 0.0, 0.0));
    rec.mat_ptr = mat_ptr;
    rec.p = r.at(t);
    return true;
}

/// <summary>
/// Contains all functions to create and manage xz planes
/// </summary>
class xzPlane : public Hittable 
{
public:
    double x0, x1, z0, z1, k;
    shared_ptr<Material> mat_ptr;

    xzPlane() 
    {}

    xzPlane(double x0, double x1, double z0, double z1, double k, shared_ptr<Material> mat_ptr)
    {
        this->x0 = x0;
        this->x1 = x1;
        this->z0 = z0;
        this->z1 = z1;
        this->k = k;
        this->mat_ptr = mat_ptr;
    }

    virtual bool hit(const Ray& r, double t_min, double t_max, hitRecord& rec) const override;
    virtual bool boundingBox(BoundingBox& output) const override
    {
        output = BoundingBox(point(x0, k - 0.0001, z0), point(x1, k + 0.0001, z1));
        return true;
    }
};

bool xzPlane::hit(const Ray& r, double tMin, double tMax, hitRecord& rec) const 
{
    auto t = (k - r.origin.y()) / r.direction.y();
    
    if (t < tMin || t > tMax)
        return false;

    auto x = r.origin.x() + t * r.direction.x();
    auto z = r.origin.z() + t * r.direction.z();

    if (x < x0 || x > x1 || z < z0 || z > z1)
        return false;

    rec.t = t;
    rec.setFaceNormal(r, vec3(0.0, 1.0, 0.0));
    rec.mat_ptr = mat_ptr;
    rec.p = r.at(t);
    return true;
}

/// <summary>
/// Contains all functions to create and manage a box
/// </summary>
class Cuboid : public Hittable
{
public:
    HittableList cuboid;
    point a;
    point b;

    Cuboid()
    {}

    Cuboid(point a, point b, shared_ptr<Material> mat_ptr)
    {
        this->a = a;
        this->b = b;

        cuboid.add(make_shared<xyPlane>(a.x(), b.x(), a.y(), b.y(), a.z(), mat_ptr));
        cuboid.add(make_shared<xyPlane>(a.x(), b.x(), a.y(), b.y(), b.z(), mat_ptr));

        cuboid.add(make_shared<yzPlane>(a.y(), b.y(), a.z(), b.z(), b.x(), mat_ptr));
        cuboid.add(make_shared<yzPlane>(a.y(), b.y(), a.z(), b.z(), a.x(), mat_ptr));

        cuboid.add(make_shared<xzPlane>(a.x(), b.x(), a.z(), b.z(), b.y(), mat_ptr));
        cuboid.add(make_shared<xzPlane>(a.x(), b.x(), a.z(), b.z(), a.y(), mat_ptr));
    }

    virtual bool hit(const Ray& r, double tMin, double tMax, hitRecord& record) const override;
    virtual bool boundingBox(BoundingBox& output) const override 
    {
        output = BoundingBox(a, b);
        return true;
    }
};

bool Cuboid::hit(const Ray& r, double tMin, double tMax, hitRecord& record) const
{
    return cuboid.hit(r, tMin, tMax, record);
}