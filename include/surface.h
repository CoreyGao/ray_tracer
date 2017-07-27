#pragma once

#include "math/Vector.h"
#include "math/Point.h"
#include "ray.h"
#include "material.h"

namespace Graphics{
    class Material;

    class Surface
    {
        public:
            Surface(const Material &m):m_mat(m)
            {
            }
            virtual ~Surface()
            {
            }
            virtual Math::Vector3D GetNormal(const Math::Point3D& p) const = 0;
            const Material &GetMaterial() const{return m_mat;};

        protected:
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
            Triangle(const Triangle &t):Surface(t), a(t.a), b(t.b), c(t.c), normal(t.normal){}
            virtual ~Triangle()
            {

            }
            virtual Math::Vector3D GetNormal(const Math::Point3D &p) const;
            inline const Math::Point3D &GetVertexA() const
            {
                return a;
            }

            inline const Math::Point3D &GetVertexB() const
            {
                return b;
            }

            inline const Math::Point3D &GetVertexC() const
            {
                return c;
            }

        private:
            Math::Point3D a,b,c;
            Math::Vector3D normal;
    };

    class Sphere : public Surface{
        public:
            Sphere(const Material &m, const Math::Point3D &c, double r)
                :Surface(m), center(c), radius(r){}
            virtual Math::Vector3D GetNormal(const Math::Point3D& p) const;
            virtual ~Sphere()
            {
            }

            inline double GetRadius() const
            {
                return radius;
            }

            inline const Math::Point3D &GetCenter() const
            {
                return center;
            }

        private:
            Math::Point3D center;
            double radius;
    };
}
