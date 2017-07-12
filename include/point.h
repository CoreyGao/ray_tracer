#pragma once

#include "math/Vector3D.h"

namespace Graphics{
    class Point2D{
        public:
            double x, y;
            Point2D(){}
            Point2D(const Point2D &p):x(p.x), y(p.y) { }
            Point2D(double x, double y):x(x), y(y) {}
            ~Point2D(){}
    };

    class Point3D{
        public:
            double x, y, z;
            Point3D():x(0), y(0), z(0){}
            Point3D(double x, double y, double z)
                :x(x), y(y), z(z) { }

            Point3D(const Point3D &p)
                :x(p.x), y(p.y), z(p.z) { }
            ~Point3D() {}


            inline Point3D operator - () const {
                return Point3D(-x, -y, -z);
            }

            inline Point3D operator + (const Point3D& p) const {
                Point3D t = *this;
                t += p;
                return t;
            }

            inline Point3D operator - (const Point3D& p) const {
                Point3D t = *this;
                t -= p;
                return t;
            }

            inline Point3D operator * (double scale) const {
                Point3D t = *this;
                t *= scale;
                return t;
            }

            inline Point3D operator / (double scale) const {
                Point3D t = *this;
                t /= scale;
                return t;
            }


            inline void operator += (const Point3D& p) {
                x += p.x;
                y += p.y;
                z += p.z;
            }

            inline void operator -= (const Point3D& p) {
                x -= p.x;
                y -= p.y;
                z -= p.z;
            }

            inline void operator *= (double r) {
                x *= r;
                y *= r;
                z *= r;
            }

            inline void operator /= (double r) {
                x /= r;
                y /= r;
                z /= r;
            }

            inline bool operator == (const Point3D &p) const{
                return x == p.x && y == p.y && z== p.z;
            }
    };
}
