#include "surface.h"
#include <gtest/gtest.h>
#include "raytracer.h"
#include "math/commondef.h"

using Math::Point3D;;
using Math::Vector3D;;

namespace Graphics
{
    Material m = {RGB(0.1, 0.1, 0.1), RGB(0.3, 0.3, 0.3), RGB(0.5, 0.5, 0.5), 10};

    TEST(TriangleTest, Intersect){
        Triangle t(m, Point3D(1, -1, -1), Point3D(1, 1, -1), Point3D(1, 0, 1));
        RayTracerTriangle rt(t);
        Ray r(Point3D(0, 0, 0), Vector3D(1, 0, 0), 0, 100);

        IntersectInfo i(&m, r);
        EXPECT_TRUE(rt.IntersectWithRay(r,i));
        EXPECT_TRUE(Vector3D(1, 0, 0) == i.surfaceNormal);
        EXPECT_TRUE(Math::almost_equal(1.0, i.ratio, 2));

        Ray rOut1(Point3D(0, 0, 0), Vector3D(1, -1, -1.1), 0, 100);
        EXPECT_FALSE(rt.IntersectWithRay(rOut1, i));

        Ray rOut2(Point3D(0, 0, 0), Vector3D(1, 1, -1.1), 0, 100);
        EXPECT_FALSE(rt.IntersectWithRay(rOut2, i));

        Ray rIn1(Point3D(0, 0, 0), Vector3D(1, 0.2, 0.1), 0, 100);
        EXPECT_TRUE(rt.IntersectWithRay(rIn1, i));

        Ray rIn2(Point3D(0, 0, 0), Vector3D(1, 0, 0.9), 0, 100);
        EXPECT_TRUE(rt.IntersectWithRay(rIn2, i));
    }
}
