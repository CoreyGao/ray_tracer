#pragma once

#include <vector>
#include <memory>
#include "GL/glut.h"

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

namespace RayTracer{
    class Render{
        public:
            Render() = default;
            ~Render() = default;

            void Draw(const Graphics::Camera &camera, const Graphics::Scene &scene);

            std::vector<std::vector<Graphics::IntersectInfo>> GetIntersectInfo(
                const std::vector<std::shared_ptr<Graphics::Surface>> &surfaces, 
                const std::vector<std::vector<Graphics::Ray> > &rays ) const;

            void CalculateColor(const Graphics::IntersectInfo &info, 
                    Graphics::RGB &rgb, const Graphics::Scene &scene) const;

            Graphics::RGB DiffuseColor(const Graphics::RGB &k , const Graphics::RGB &i, 
                    const Math::Vector3D &l, const Math::Vector3D &n) const;
            Graphics::RGB SpecularColor(const Graphics::RGB &k, const Graphics::RGB &i, 
                    const Math::Vector3D &l, const Math::Vector3D &n, const Math::Vector3D &v, double factor) const;
            Graphics::RGB AmbientColor(const Graphics::RGB &k, const Graphics::RGB &i) const;
    };
}
