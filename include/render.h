#pragma once

#include <vector>
#include <memory>
#include "GL/glut.h"
#include "math/Point.h"

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

namespace Graphics{
    class Render
    {
        public:
            virtual void Draw(const Scene &scene) = 0;

            inline void SetCamera(const Camera *c)
            {
                m_camera = c;
            }

        protected:
            const Camera *m_camera;
    };

    class RayTracer : public Render
    {
        public:
            RayTracer() = default;
            ~RayTracer() = default;


            virtual void Draw(const Graphics::Scene &scene);

        private:
            std::vector<std::vector<Graphics::IntersectInfo>> GetIntersectInfo(
                const std::vector<std::shared_ptr<Graphics::Surface>> &surfaces, 
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
