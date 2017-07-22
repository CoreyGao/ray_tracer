#include "shading.h"
#include "math/Vector3D.h"

using namespace Graphics;
using namespace Math;

RGB Shading::DiffuseColor(const RGB &k, const RGB &i, const Vector3D &l, const Vector3D &n)
{
    double costheta = Dot(n, -l);
    costheta = (0 > costheta)?0:costheta;
    return k * i * costheta;
}

RGB Shading::SpecularColor(const RGB &k, const RGB &i, const Vector3D &l, const Vector3D &n, const Vector3D &v, double factor) 
{
    Vector3D uV = v.Unit();
    Vector3D uL = l.Unit();
    Vector3D h = -uV + -uL;
    h.Normalize();
    double costheta = Dot(n, h);
    costheta = (0 > costheta)?0:costheta;
    return (k * i) * pow(costheta, factor);
}

RGB Shading::AmbientColor(const RGB &k, const RGB &i) 
{
    return k * i;
}
