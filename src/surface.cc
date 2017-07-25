#include "../include/surface.h"
#include "../include/ray.h"
#include "../include/math/Matrix33.h"

using namespace Math;

namespace Graphics
{
    Vector3D Triangle::GetNormal(const Point3D &p) const
    {
        return normal;
    }


    Vector3D Sphere::GetNormal(const Point3D &p) const
    {
        return (p - center).Unit();
    }
}
