#include "../include/surface.h"
#include "../include/ray.h"
#include "../include/math/Matrix33.h"

using namespace Math;

namespace Graphics
{
    bool Triangle::IntersectWithRay(const Ray &r, IntersectInfo &info) const
    {
        Point3D e = r.GetPoint(0);
        Vector3D d = r.GetPoint(1) - e;

        Vector3D ab = a - b;
        Vector3D ac = a - c;
        Vector3D ae = a - e;

        double detA = Matrix33(ab.x, ac.x, d.x, ab.y, ac.y, d.y, ab.z, ac.z, d.z).Det();

        double detBeta = Matrix33(ae.x, ac.x, d.x, ae.y, ac.y, d.y, ae.z, ac.z, d.z).Det();
        double beta = detBeta / detA ;
        double detGame = Matrix33(ab.x, ae.x, d.x, ab.y, ae.y, d.y, ab.z, ae.z, d.z).Det();
        double gama = detGame / detA;
        double detT = Matrix33(ab.x, ac.x, ae.x, ab.y, ac.y, ae.y, ab.z, ac.z, ae.z).Det();
        double t = detT / detA;

        if(t < r.GetStartRatio() || t> r.GetEndRatio())
            return false;
        if(beta < 0 || beta > 1)
            return false;
        if(gama < 0 || gama > (1 - beta))
            return false;

        info.interPoint = r.GetPoint(t);
        info.surfaceNormal = normal;
        info.ratio = t;

        return true;
    }

    Vector3D Triangle::GetNormal(const Point3D &p)
    {
        return normal;
    }


    bool Sphere::IntersectWithRay(const Ray &r, IntersectInfo &info) const{
         Point3D e = r.GetPoint(0);
         const Vector3D& d = r.GetDir();
         Vector3D ec(e.x - center.x, e.y - center.y, e.z - center.z);
         double A = Dot(d, d);
         double B = 2 * Dot(d, ec);
         double C = Dot(ec, ec) - std::pow(radius, 2); 

         double root = std::pow(B, 2) - 4 * A * C;
         if(root < 0 )
             return false;
         
         double t1 = (-B + std::sqrt(root)) / (2 * A);
         double t2 = (-B - std::sqrt(root)) / (2 * A);
        
         double t = 0;
         if(t2 >= r.GetStartRatio() && t2 <= r.GetEndRatio())
             t = t2;
         else if(t1 >= r.GetStartRatio() && t1 <= r.GetEndRatio())
             t = t1;
         else
             return false;

         Point3D p = r.GetPoint(t);
         info.interPoint = p;
         Vector3D n(p.x - center.x, p.y - center.y, p.z - center.z);
         n.Normalize();
         info.surfaceNormal = n;
         return true;

    }

    Vector3D Sphere::GetNormal(const Point3D &p)
    {
        return Vector3D(0, 0, 0);
    }
}
