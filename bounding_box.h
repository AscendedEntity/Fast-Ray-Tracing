#ifndef BOUNDING_BOX_H
#define BOUNDING_BOX_H

#include "const_utility.h"

/// <summary>
/// Contains all the functions required to create a bounding box
/// </summary>
class BoundingBox
{
public:
	/// <summary>
	/// End 1 of the diagonal
	/// </summary>
	point a;
	/// <summary>
	/// End 2 of the diagonal
	/// </summary>
	point b;

	/// <summary>
	/// Default constructor
	/// </summary>
	BoundingBox()
	{}

	/// <summary>
	/// Paraterized constructor
	/// </summary>
	/// <param name="a">End 1 of the diagonal</param>
	/// <param name="b">End 2 of the diagonal</param>
	BoundingBox(const point& a, const point& b)
	{
		this->a = a;
		this->b = b;
	}

	/// <summary>
	/// Function to check if the ray intersects the bounding box
	/// </summary>
	/// <param name="ray">Refernce to the Ray object</param>
	/// <param name="tMin">Minimum acceptable distance from the ray's origin</param>
	/// <param name="tMax">Maximum acceptable distance from the ray's origin</param>
	/// <returns>True, if the ray intersects the bounding box</returns>
	bool hit(const Ray& ray, double tMin, double tMax) const
	{
		for (int i = 0; i < 3; i++)
		{
			// checks if there is a range of t in R = A + Bt for which the ray intersects the bounding box

			auto inv = 1.0 / ray.direction[i];
			auto t0 = (a[i] - ray.origin[i]) * inv;
			auto t1 = (b[i] - ray.origin[i]) * inv;

			if (inv < 0.0)
				std::swap(t0, t1);

			tMin = fmax(tMin, t0);
			tMax = fmin(tMax, t1);

			if (tMax <= tMin)
				return false;
		}
		return true;
	}
};

/// <summary>
/// Takes 2 bounding boxes and creates a new box enclosing both
/// </summary>
/// <param name="b1">The first bounding box</param>
/// <param name="b2">The secong bounding box</param>
/// <returns>A new bounding box which is the combination of both inputs</returns>
BoundingBox combinedBox(BoundingBox b1, BoundingBox b2)
{
	// calculates the minimum and maximum x, y and z values to select the end points of the diagonal

	point near(fmin(b1.a.x(), b2.a.x()), fmin(b1.a.y(), b2.a.y()), fmin(b1.a.z(), b2.a.z()));
	point far(fmax(b1.b.x(), b2.b.x()), fmax(b1.b.y(), b2.b.y()), fmax(b1.b.z(), b2.b.z()));

	return BoundingBox(near, far);
}
#endif