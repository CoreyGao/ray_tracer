#include "tools.h"
#include <algorithm>

using namespace Math;
namespace Graphics
{
    Vector3D NonInlinearV(const Vector3D &v)
    {
        Vector3D u = v.Unit();
        float *min = &u.x;
        min = (abs(*min) < abs(u.y)) ? min : &u.y;
        min = (abs(*min) < abs(u.z)) ? min : &u.z;
        *min = 1;
        return u;
    }

    Matrix33 BuildBasis(const Vector3D &t)
    {
        Vector3D w = t.Unit();
        Vector3D l = NonInlinearV(w);
        Vector3D u = Cross(l, w).Unit();
        Vector3D v = Cross(w, u).Unit();
        return Matrix33(u, v, w);
    }

    Math::Matrix33 BuildRotateMatix(const Math::Vector3D &d, double theta)
    {

        Matrix33 basis = BuildBasis(d);
        double cost = cos(theta);
        double sint = sin(theta);
        Matrix33 rotate(cost, -sint, 0, sint, cost, 0, 0, 0, 1);
        return basis.Transfer() * rotate * basis;
    }
}

