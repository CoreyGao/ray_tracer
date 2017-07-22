#pragma once

#include "math/Vector3D.h"
#include "math/Point.h"
#include "ray.h"
#include "material.h"

namespace Graphics{
    class Material;
    class IntersectInfo{
        public:
            double ratio;
            Graphics::Ray r;
            Math::Point3D interPoint;
            Math::Vector3D surfaceNormal;
            const Material &mat; 

            IntersectInfo(const Material &m, Graphics::Ray r):ratio(0), r(r), interPoint(), surfaceNormal(), mat(m){}

            //urgly
            IntersectInfo(const Material &m):ratio(0), r(), interPoint(), surfaceNormal(), mat(m){}
            inline bool IsEmpty() const{
                return ratio == 0 && interPoint== Math::Point3D() && surfaceNormal== Math::Vector3D();
            }
    };

    class Surface{
        public:
            Surface(const Material &m):m_mat(m){}
            virtual ~Surface()
            {
            }
            virtual bool IntersectWithRay(const Graphics::Ray &r, IntersectInfo &info) const = 0;
            virtual Math::Vector3D GetNormal(const Math::Point3D& p) = 0;
            const Material &GetMaterial() const{return m_mat;};

        private:
            const Material &m_mat;
    };


    class Triangle : public Surface{
        public:
            Triangle(const Material &m, const Math::Point3D &a, const Math::Point3D &b, const Math::Point3D &c)
                :Surface(m), a(a), b(b), c(c)
            {
                normal = Cross(Math::Vector3D(b.x - a.x, b.y - a.y, b.z - a.z), Math::Vector3D(c.x - a.x, c.y - a.y, c.z - a.z));
                normal.Normalize();
            }
            Triangle(const Triangle &t):Surface(t), a(t.a), b(t.b), c(t.c){}
            virtual ~Triangle()
            {

            }
            virtual Math::Vector3D GetNormal(const Math::Point3D &p);
            virtual bool IntersectWithRay(const Graphics::Ray &r, IntersectInfo &info) const;

        private:
            Math::Point3D a,b,c;
            Math::Vector3D normal;
    };

    class Sphere : public Surface{
        public:
            Sphere(const Material &m, const Math::Point3D &c, double r)
                :Surface(m), center(c), radius(r){}
            virtual Math::Vector3D GetNormal(const Math::Point3D& p);
            virtual ~Sphere()
            {
            }
            virtual bool IntersectWithRay(const Graphics::Ray &r, IntersectInfo &info) const;

        private:
            Math::Point3D center;
            double radius;
    };
}
