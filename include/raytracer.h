#pragma once

#include "render.h"
#include <vector>
#include <memory>
#include "surface.h"


#ifdef TESTER
#include "gtest/gtest_prod.h"
#endif

namespace Math{
    class Vector3D;
}

namespace Graphics{
    class IntersectInfo;
    class Camera;
    class RGB;
    class Scene;
    class Surface;
    class Ray;
}

namespace Graphics
{
    class IntersectInfo
    {
        public:
            double ratio;
            Graphics::Ray r;
            Math::Point3D interPoint;
            Math::Vector3D surfaceNormal;
            const Material *mat; 

            IntersectInfo(const Material *m, Graphics::Ray r):ratio(0), r(r), interPoint(), surfaceNormal(), mat(m){}

            IntersectInfo() = default;
            inline bool IsEmpty() const{
                return ratio == 0 && interPoint== Math::Point3D() && surfaceNormal== Math::Vector3D();
            }
    };


    class RaytracerSurfaceInterface
    {
        public:
            virtual bool IntersectWithRay(const Graphics::Ray &r, IntersectInfo &info) const = 0;
            virtual const Material &GetMaterial() const = 0;
    };

    class RayTracerTriangle : public Triangle, public RaytracerSurfaceInterface
    {
        public:
            RayTracerTriangle(const Triangle &t)
                :Triangle(t)
            {
            }
            RayTracerTriangle(const RayTracerTriangle &rt)
                :Triangle(rt)
            {
            }

            virtual bool IntersectWithRay(const Graphics::Ray &r, IntersectInfo &info) const;
            virtual const Material &GetMaterial() const;
    };

    class RayTracerSphere : public Sphere, public RaytracerSurfaceInterface
    {
        public:
            RayTracerSphere(const Sphere& s)
                :Sphere(s)
            {
            }
            RayTracerSphere(const RayTracerSphere & rs)
                :Sphere(rs)
            {
            }
            virtual bool IntersectWithRay(const Graphics::Ray &r, IntersectInfo &info) const;
            virtual const Material &GetMaterial() const;
    };


    class RayTracer : public Render
    {
        public:
            RayTracer() = default;
            ~RayTracer() = default;


            virtual void Draw(const Graphics::Scene &scene);

        private:
            std::vector<std::vector<Graphics::IntersectInfo>> GetIntersectInfo(
                const std::vector<std::shared_ptr<RaytracerSurfaceInterface>> &surfaces, 
                const std::vector<std::vector<Graphics::Ray> > &rays ) const;

            void CalculateColor(const Graphics::IntersectInfo &info, 
                    Graphics::RGB &rgb, const Graphics::Scene &scene) const;

            void CalRays(std::vector<std::vector<Graphics::Ray>> &rays);

#ifdef TESTER
    friend class RayTracerTest;
    FRIEND_TEST(RayTracerTest, test1);
#endif
    };
}
