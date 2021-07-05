#ifndef CAMERA_H
#define CAMERA_H

#include "const_utility.h"

/// <summary>
/// Contains all the functions to create a camera view
/// </summary>
class Camera 
{
private:
    /// <summary>
    /// Position of the camera
    /// </summary>
    point origin;
    /// <summary>
    /// Vector in horizontal direction of the camera
    /// </summary>
    vec3 horizontal;
    /// <summary>
    /// Vector in vertical direction of the camera
    /// </summary>
    vec3 vertical;
    /// <summary>
    /// Co-ordinates of the lower-left point of the viewport
    /// </summary>
    point lowerLeft;
    /// <summary>
    /// Unit vector in the x direction of the camera
    /// </summary>
    vec3 u;
    /// <summary>
    /// Unit vector in the y direction of the camera
    /// </summary>
    vec3 v;
    /// <summary>
    /// Unit vector in the z direction of the camera
    /// </summary>
    vec3 w;
    /// <summary>
    /// Radius of the lens of the camera
    /// </summary>
    double lensRadius;

public:
    /// <summary>
    /// Default constructor
    /// </summary>
    Camera()
    {}

    /// <summary>
    /// Parameterized constructor that creates the camera
    /// </summary>
    /// <param name="lookFrom">Position of the camera in space</param>
    /// <param name="lookAt">Position of the point to focus on in space</param>
    /// <param name="aspect">Aspect ratio of the image</param>
    /// <param name="vfov">Vertical field of view</param>
    /// <param name="aperture">Aperture of the camera lens</param>
    /// <param name="focusDistance">Maximum distance at which camera has clear focus</param>
    Camera(vec3 lookFrom, vec3 lookAt, double aspect, double vfov, double aperture, double focusDistance) 
    {
        auto theta = radians(vfov);
        auto h = tan(theta / 2.0);
        auto aspectRatio = aspect;
        auto viewportHeight = 2.0 * h;
        auto viewportWidth = aspectRatio * viewportHeight;

        vec3 worldUp = vec3(0.0, 1.0, 0.0);

        w = unitVector(lookFrom - lookAt);
        u = unitVector(cross(worldUp, w));
        v = cross(w, u);

        origin = lookFrom;
        horizontal = viewportWidth * u * focusDistance;
        vertical = viewportHeight * v * focusDistance;
        lowerLeft = origin - horizontal / 2.0 - vertical / 2.0 - w * focusDistance;
        lensRadius = aperture / 2.0;
    }

    /// <summary>
    /// Gets the ray from the point on the camera lens to a point on the screen
    /// </summary>
    /// <param name="x">x co-ordinate of the point on the screen</param>
    /// <param name="y">y co-ordinate of the point on the screen</param>
    /// <returns>The created ray object</returns>
    Ray getRay(double x, double y) const 
    {
        vec3 random = lensRadius * random_in_unit_disk();
        vec3 offset = u * random.x() + v * random.y();

        return Ray(origin + offset, lowerLeft + x * horizontal + y * vertical - (origin + offset));
    }
};
#endif