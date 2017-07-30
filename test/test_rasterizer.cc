#include <gtest/gtest.h>

#include "math/commondef.h"
#include "camera.h"
#include "rasterizer.h"
#include "scene.h"

namespace Graphics
{
    class RasterizerTest: public ::testing::Test{
        protected:
            Rasterizer ras;
    };

    TEST_F(RasterizerTest, OP)
    {
        Camera c(Math::PI/2, -0.2, -50, 30, 30);
        ras.SetCamera(&c);

        Math::Matrix44 unit = Math::Matrix44::UnitMatrix();
        EXPECT_TRUE(unit == ras.m_camT);

        Math::Matrix44 mvp(15, 0, 0, 14.5,
                0, 15, 0, 14.5,
                0, 0, 1, 0,
                0, 0, 0, 1);
        EXPECT_TRUE(mvp == ras.m_vpT);

        Math::Matrix44 morthoT(2 / 0.4, 0, 0, 0,
                0, 2/0.4, 0, 0,
                0, 0, 2/49.8, 50.2/49.8,
                0, 0, 0, 1);
        EXPECT_TRUE(morthoT == ras.m_orthoT);

        Math::Matrix44 persT(-0.2, 0, 0, 0,
                0, -0.2, 0, 0,
                0, 0, -50.2, -10,
                0, 0, 1, 0);
        EXPECT_TRUE(persT == ras.m_perspectiveT);
        EXPECT_TRUE(ras.m_allT == mvp * morthoT * persT * unit);
        
        Ray2D r(Math::Point2D(0, 0), Math::Vector2D(1, 1), 0, 4*std::sqrt(2));
        ras.DrawLine(r);
        for(auto row:ras.m_fragments)
        {
            for(auto f:row)
            {
                std::cout<<f;
            }
            std::cout<<std::endl;
        }
        std::cout << "-----------------------------"<<std::endl;

        ras.m_fragments.clear();
        ras.m_fragments.resize(c.GetVerticalPixNum());
        for(auto &row:ras.m_fragments)
        {
            for(unsigned int i = 0 ; i < c.GetHorizontolPixNum() ; i++)
                row.push_back(Fragment());
        }

        
        Material mat = {RGB(0.0, 0.0, 0.0), RGB(1, 0.5, 0.5), RGB(0.1, 0.1, 0.1), 10};
        Scene scene; 
        auto t = std::make_shared<Triangle>(mat, Math::Point3D(-0.5, -0.5, -1),
                Math::Point3D(0.5, -0.5, -1), Math::Point3D(0, 0.5, -1));
        scene.AddSurface(t);

        DiffuseLight d(RGB(0.5, 0.4, 0.3), Math::Vector3D(0, sqrt(3), -1));
        SpecularLight s(RGB(1, 0, 0), Math::Vector3D(0, 1, -1));
        AmbientLight a(RGB(0.1, 0.0, 0.1));

        scene.SetDiffuseLight(d);
        scene.AddSpecularLight(s);
        scene.SetAmbientLight(a);

        ras.DrawTriangle(*t, scene);

        for(auto row:ras.m_fragments)
        {
            for(auto f:row)
            {
                std::cout<<f;
            }
            std::cout<<std::endl;
        }
    }
}
