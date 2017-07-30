#pragma once

#include "common.h"
#include "render.h"
#include "color.h"
#include "ray.h"
#include "surface.h"

#include <math/Matrix.h>

#include <vector>
#include <climits>

#ifdef TESTER
#include "gtest/gtest_prod.h"
#endif

namespace Graphics
{

    class Fragment
    {
        public:
            ubyte r, g, b, z;


            Fragment()
                :r(UCHAR_MAX), g(UCHAR_MAX), b(UCHAR_MAX), z(UCHAR_MAX)
            {
            }

            void Clear()
            {
                r = UCHAR_MAX;
                g = UCHAR_MAX;
                b = UCHAR_MAX;
                z = UCHAR_MAX;
            }

            ~Fragment()
            {
            }

            Fragment(const Fragment &f)
                :r(f.r), g(f.g), b(f.b), z(f.z)
            {
            }

            friend std::ostream &operator<<(std::ostream &os, const Fragment &f);
    };



    class Rasterizer : public Render
    {
        public:
            Rasterizer() { }
            ~Rasterizer(){}

            virtual void Draw(const Scene &scene);
            void SetCamera(const Camera *camera);

        private:
            void OnCameraChange();
            void DrawLine(const Ray2D &line);
            void DrawTriangle(const Triangle &t, const Scene &scene);

            RGB CalculateColor(const Math::Point3D &p, const Material &mat, 
                    const Scene &scene, const Math::Vector3D &n);
            void DrawFragments(int x, int y, const UBYTE_RGB &urgb, int z);


        private:
            std::vector<std::vector<Fragment>> m_fragments;

            Math::Matrix44 m_vpT;
            Math::Matrix44 m_orthoT;
            Math::Matrix44 m_perspectiveT;
            Math::Matrix44 m_allT;

            Math::Matrix44 m_camT;
            Math::Matrix44 m_camInverseT;

#ifdef TESTER
            friend class RasterizerTest;
            FRIEND_TEST(RasterizerTest, OP);
#endif
    };
}
