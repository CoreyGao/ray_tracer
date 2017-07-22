#include "math/Point.h"
#include <gtest/gtest.h>

using Math::Point3D;

TEST(Point3DTest, Constructor)
{
    Point3D p1;
    EXPECT_DOUBLE_EQ(0, p1.x);
    EXPECT_DOUBLE_EQ(0, p1.y);
    EXPECT_DOUBLE_EQ(0, p1.z);

    Point3D p2(1.1, 1.1, 1.1);
    EXPECT_DOUBLE_EQ(1.1, p2.x);
    EXPECT_DOUBLE_EQ(1.1, p2.y);
    EXPECT_DOUBLE_EQ(1.1, p2.z);

    Point3D p3(p2);
    EXPECT_DOUBLE_EQ(1.1, p3.x);
    EXPECT_DOUBLE_EQ(1.1, p3.y);
    EXPECT_DOUBLE_EQ(1.1, p3.z);
}

TEST(Point3DTest, Operation)
{
    Point3D p1;
    Point3D p2(1.1, 1.1, 1.1);
    Point3D p3(-5, -5, -5);

    Point3D p4 = -p2;
    EXPECT_DOUBLE_EQ(-1.1, p4.x);
    EXPECT_DOUBLE_EQ(-1.1, p4.y);
    EXPECT_DOUBLE_EQ(-1.1, p4.z);


    Math::Vector3D v1 = p2 - p3;
    EXPECT_DOUBLE_EQ(6.1, v1.x);
    EXPECT_DOUBLE_EQ(6.1, v1.y);
    EXPECT_DOUBLE_EQ(6.1, v1.z);

    Point3D p5 = p2 + v1;
    
    EXPECT_DOUBLE_EQ(7.2, p5.x);
    EXPECT_DOUBLE_EQ(7.2, p5.y);
    EXPECT_DOUBLE_EQ(7.2, p5.z);

    Point3D p6 = p2 - v1;
    
    EXPECT_TRUE(p6 == p3);
}
