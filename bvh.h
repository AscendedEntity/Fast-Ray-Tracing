#ifndef BVH_NODE
#define BVH_NODE

#include "const_utility.h"
#include "hittable.h"
#include "hittableList.h"
#include <algorithm>

/// <summary>
/// Contains all the functions required to create and manage a bounding volume hierarchy (BVH) tree
/// </summary>
class BVH_Node : public Hittable
{
public:
	/// <summary>
	/// Left child of the node
	/// </summary>
	shared_ptr<Hittable>left;
	/// <summary>
	/// Right child of the node
	/// </summary>
	shared_ptr<Hittable>right;
	/// <summary>
	/// Bounding box of the node
	/// </summary>
	BoundingBox box;

	/// <summary>
	/// Default constructor
	/// </summary>
	BVH_Node()
	{}

	/// <summary>
	/// Parameterized constructor to create the BVH tree
	/// </summary>
	/// <param name="srcObjects">List of all the objects in the world</param>
	/// <param name="start">Start index of the objects list in the current node</param>
	/// <param name="end">(End + 1) index of the objects list in the current node</param>
	BVH_Node(const std::vector<shared_ptr<Hittable>>& srcObjects, size_t start, size_t end);

	/// <summary>
	/// Function to check if the bounding box of the current node is intersected by the ray
	/// </summary>
	/// <param name="ray">Reference to the Ray object</param>
	/// <param name="tMin">Minimum acceptable distance from the ray's origin</param>
	/// <param name="tMax">Maximum acceptable distance from the ray's origin</param>
	/// <param name="record">Reference to the record object</param>
	/// <returns>True, if the bounding box of the current node is intersected by the ray</returns>
	virtual bool hit(const Ray& ray, double tMin, double tMax, hitRecord& record) const override;
	/// <summary>
	/// Gets the bounding box of the current node
	/// </summary>
	/// <param name="output">Reference to the bounding box object</param>
	/// <returns>True</returns>
	virtual bool boundingBox(BoundingBox& output) const override;
};

bool BVH_Node::boundingBox(BoundingBox& output) const
{
	output = box;
	return true;
}

bool BVH_Node::hit(const Ray& ray, double tMin, double tMax, hitRecord& record) const
{
	// traverse the BVH tree

	if (!box.hit(ray, tMin, tMax))
		return false;

	bool hitLeft = left->hit(ray, tMin, tMax, record);
	bool hitRight = right->hit(ray, tMin, hitLeft ? record.t : tMax, record);

	return hitLeft || hitRight;
}

/// <summary>
/// Comparator function to sort the objects list
/// </summary>
/// <param name="a">Item 1 in the objects list</param>
/// <param name="b">Item 2 in the objects list</param>
/// <param name="axis">Axis along which objects must be sorted</param>
/// <returns>True, if item 1 should appear before item 2 in the list</returns>
inline bool compareBox(const shared_ptr<Hittable> a, const shared_ptr<Hittable> b, int axis) 
{
	// comparator function to sort the objects list

	BoundingBox b1;
	BoundingBox b2;

	if (!a->boundingBox(b1) || !b->boundingBox(b2))
		std::cerr << "No bounding box in bvh_node constructor.\n";

	return b1.a.p[axis] < b2.a.p[axis];
}

bool compareX(const shared_ptr<Hittable> a, const shared_ptr<Hittable> b) 
{
	return compareBox(a, b, 0);
}

bool compareY(const shared_ptr<Hittable> a, const shared_ptr<Hittable> b) 
{
	return compareBox(a, b, 1);
}

bool compareZ(const shared_ptr<Hittable> a, const shared_ptr<Hittable> b) 
{
	return compareBox(a, b, 2);
}

BVH_Node::BVH_Node(const std::vector<shared_ptr<Hittable>>& srcObjects, size_t start, size_t end)
{
	auto objects = srcObjects;
	int axis = randomInt(0, 2);
	auto comparator = (axis == 0) ? compareX : (axis == 1) ? compareY : compareZ;

	size_t object_span = end - start;

	if (object_span == 1)
		left = right = objects[start];
	else if (object_span == 2)
	{
		if (comparator(objects[start], objects[start + 1]))
		{
			left = objects[start];
			right = objects[start + 1];
		}
		else
		{
			left = objects[start + 1];
			right = objects[start];
		}
	}
	else
	{
		std::sort(objects.begin() + start, objects.begin() + end, comparator);
		auto mid = (start + end) / 2;
		left = make_shared<BVH_Node>(objects, start, mid);
		right = make_shared<BVH_Node>(objects, mid, end);
	}

	BoundingBox boxLeft, boxRight;

	left->boundingBox(boxLeft);
	right->boundingBox(boxRight);

	box = combinedBox(boxLeft, boxRight);
}

#endif