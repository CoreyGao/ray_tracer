#include "surface.h"
#include "ray.h"
#include "math/Matrix.h"

using Math::Point3D;
using Math::Vector3D;

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
