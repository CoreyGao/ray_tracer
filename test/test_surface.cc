#include "surface.h"
#include <gtest/gtest.h>
#include "math/commondef.h"

using namespace Graphics;
using namespace Math;

Material m = {RGB(0.1, 0.1, 0.1), RGB(0.3, 0.3, 0.3), RGB(0.5, 0.5, 0.5), 10};
/*
template<class T>
Surface* CreateSurface();


template<>
Surface* CreateSurface<Triangle>(){
    return new Triangle(m, Point3D(1, -1, -1), Point3D(1, 1, -1), Point3D(1, 0, 1));
}

template<>
Surface* CreateSurface<Sphere>(){
    return new Sphere(m, Point3D(-10, 0, 0), 1);
}

template<class T>
class SurfaceTest : public ::testing::Test{
    protected:
        SurfaceTest():surface(CreateSurface<T>()){}
        virtual ~SurfaceTest() {delete surface;}

        Surface const *surface;
};

using ::testing::Types;

using Imp  = Types<Triangle, Sphere>;
TYPED_TEST_CASE(SurfaceTest, Imp);

TYPED_TEST(SurfaceTest, test1){

}*/

TEST(TriangleTest, Intersect){
    Triangle t(m, Point3D(1, -1, -1), Point3D(1, 1, -1), Point3D(1, 0, 1));
    Ray r(Point3D(0, 0, 0), Vector3D(1, 0, 0), 0, 100);

    IntersectInfo i(m, r);
    EXPECT_TRUE(t.IntersectWithRay(r,i));
    EXPECT_TRUE(Vector3D(1, 0, 0) == i.surfaceNormal);
    EXPECT_TRUE(Math::almost_equal(1.0, i.ratio, 2));

    Ray rOut1(Point3D(0, 0, 0), Point3D(1, -1, -1.1), 0, 100);
    EXPECT_FALSE(t.IntersectWithRay(rOut1, i));
    
    Ray rOut2(Point3D(0, 0, 0), Point3D(1, 1, -1.1), 0, 100);
    EXPECT_FALSE(t.IntersectWithRay(rOut2, i));

    Ray rIn1(Point3D(0, 0, 0), Point3D(1, 0.2, 0.1), 0, 100);
    EXPECT_TRUE(t.IntersectWithRay(rIn1, i));

    Ray rIn2(Point3D(0, 0, 0), Point3D(1, 0, 0.9), 0, 100);
    EXPECT_TRUE(t.IntersectWithRay(rIn2, i));
}
