#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H

#include "hittable.h"

#include <memory>
#include <vector>

using std::shared_ptr;
using std::make_shared;

/// <summary>
/// Used to create a list of all the objects in the scene
/// </summary>
class HittableList : public Hittable
{
public:
    std::vector<shared_ptr<Hittable>> objects;
   
    HittableList() {}

    HittableList(shared_ptr<Hittable> object)
    {
        add(object);
    }

    void clear()
    {
        objects.clear();
    }

    void add(shared_ptr<Hittable> object)
    {
        objects.push_back(object);
    }

    virtual bool hit(const Ray& r, double tMin, double tMax, hitRecord& rec) const override;
    virtual bool boundingBox(BoundingBox& output) const override;
};

bool HittableList::hit(const Ray& r, double tMin, double tMax, hitRecord& rec) const
{
    hitRecord temp;
    bool hitAnything = false;
    auto closest = tMax;

    for (const auto& object : objects) 
    {
        if (object->hit(r, tMin, closest, temp)) 
        {
            hitAnything = true;
            closest = temp.t;
            rec = temp;
        }
    }

    return hitAnything;
}

bool HittableList::boundingBox(BoundingBox& output) const
{
    if (objects.empty())
        return false;

    BoundingBox temp;
    bool first = true;

    for (const auto& object : objects)
    {
        if (!object->boundingBox(temp))
            return false;
        output = first ? temp : combinedBox(output, temp);
        first = false;
    }

    return true;
}
#endif