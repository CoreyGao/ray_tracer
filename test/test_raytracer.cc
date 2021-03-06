#include "raytracer.h"
#include "camera.h"
#include "scene.h"
#include "color.h"
#include "surface.h"
#include <gtest/gtest.h>
#include "shading.h"

using Math::PI;
using Math::Point3D;
using Math::Vector3D;
using Math::almost_equal;

using std::make_shared;
using std::shared_ptr;
using std::vector;
using std::dynamic_pointer_cast;

namespace Graphics{

    class RayTracerTest : public ::testing::Test{
        protected:
            RayTracer r;
    };

    TEST_F(RayTracerTest, test1){
        Camera camera(PI/2, -0.2, -50, 10, 10);

        r.SetCamera(&camera);
        Material mat = {RGB(0.1, 0.1, 0.1), RGB(0.5, 0.5, 0.5), RGB(0.1, 0.1, 0.1), 10};

        Scene scene; 
        auto t = make_shared<Triangle>(mat, Point3D(-1, -1, -1), Point3D(1, -1, -1), Point3D(0, 1, -1));
        scene.AddSurface(t);

        DiffuseLight d(RGB(0.5, 0.4, 0.3), Vector3D(0, sqrt(3), -1));
        SpecularLight s(RGB(0.3, 0.4, 0.5), Vector3D(0, 1, -1));
        AmbientLight a(RGB(0.1, 0.0, 0.1));


        RGB dResult = Shading::DiffuseColor(mat.k_diffuse, d.intensity, d.dir, t->GetNormal(Point3D(0, 0, 0)));
        EXPECT_TRUE(almost_equal(0.025f, dResult.r, 1));
        EXPECT_TRUE(almost_equal(0.02f, dResult.g, 1));
        EXPECT_TRUE(almost_equal(0.015f, dResult.b, 1));

        RGB sResult = Shading::SpecularColor(mat.k_specular, s.intensity, s.dir, t->GetNormal(Point3D(0, 0, 0)), Vector3D(0, -1, -1), 10);
        EXPECT_TRUE(almost_equal(0.15f, sResult.r, 1));
        EXPECT_TRUE(almost_equal(0.2f, sResult.g, 1));
        EXPECT_TRUE(almost_equal(0.25f, sResult.b, 1));

        RGB aResult = Shading::AmbientColor(mat.k_ambient, a.intensity);
        EXPECT_TRUE(almost_equal(0.01f, aResult.r, 1));
        EXPECT_TRUE(almost_equal(0.0f, aResult.g, 1));
        EXPECT_TRUE(almost_equal(0.01f, aResult.b, 1));

        scene.SetDiffuseLight(d);
        scene.AddSpecularLight(s);
        scene.SetAmbientLight(a);

        vector<vector<Graphics::Ray>> rays;
        r.CalRays(rays);
        vector<shared_ptr<RaytracerSurfaceInterface>> interfaces;

        vector<shared_ptr<Surface>> surfaces = scene.GetSurfaces();
        for(auto s:surfaces)
        {
            shared_ptr<Triangle> ri = dynamic_pointer_cast<Triangle>(s);
            if(ri)
                interfaces.push_back(make_shared<RayTracerTriangle>(*ri));

            shared_ptr<Sphere> si = dynamic_pointer_cast<Sphere>(s);
            if(si)
                interfaces.push_back(make_shared<RayTracerSphere>(*si));
        }


        vector<vector<IntersectInfo>> infos = r.GetIntersectInfo(interfaces, rays);
        for(auto infoRow : infos)
        {
            for(auto info : infoRow)
            {
                std::cout<<info.IsEmpty()<<" ";
            }
            std::cout<<std::endl;
        }

        vector<vector<RGB>> colors(camera.GetVerticalPixNum());
        for(unsigned int i = 0 ; i < camera.GetVerticalPixNum() ; i++)
        {
            colors[i].resize(camera.GetVerticalPixNum());
        }

        int rowIndex = 0;
        for(auto intersectInfoRow : infos)
        {
            int colIndex = 0;
            for(auto intersectIno : intersectInfoRow)
            {   
                r.CalculateColor(intersectIno, colors[rowIndex][colIndex], scene);

                colIndex++;
            }
            rowIndex++;
        }

        std::cout<<"------------------------------"<<std::endl;
        for(auto colorRow :colors)
        {
            for(auto color:colorRow)
            {
                std::cout<<color.r<<" " << color.g<<" " << color.b << "|";
            }
            std::cout<<std::endl;
        }
    }
}
