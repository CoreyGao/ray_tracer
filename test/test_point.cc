#include "point.h"
#include <gtest/gtest.h>

using Graphics::Point3D;

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
    EXPECT_EQ(true, p1 == (p4 + p2));

    Point3D p5 = p2 + p3;
    EXPECT_DOUBLE_EQ(-3.9, p5.x);
    EXPECT_DOUBLE_EQ(-3.9, p5.y);
    EXPECT_DOUBLE_EQ(-3.9, p5.z);

    Point3D p6 = p2 - p3;
    EXPECT_DOUBLE_EQ(6.1, p6.x);
    EXPECT_DOUBLE_EQ(6.1, p6.y);
    EXPECT_DOUBLE_EQ(6.1, p6.z);

    Point3D p7 = p3 * 0.1;
    EXPECT_DOUBLE_EQ(-0.5, p7.x);
    EXPECT_DOUBLE_EQ(-0.5, p7.y);
    EXPECT_DOUBLE_EQ(-0.5, p7.z);

    Point3D p8 = p3 / 5;
    EXPECT_DOUBLE_EQ(-1, p8.x);
    EXPECT_DOUBLE_EQ(-1, p8.y);
    EXPECT_DOUBLE_EQ(-1, p8.z);

    p4 += p2;
    EXPECT_EQ(true, p1 == p4);

    p5 -= p3;
    EXPECT_EQ(true, p2 == p5);

    p6 *= 10;
    EXPECT_DOUBLE_EQ(61, p6.x);
    EXPECT_DOUBLE_EQ(61, p6.y);
    EXPECT_DOUBLE_EQ(61, p6.z);

    p7 /= 10;
    EXPECT_DOUBLE_EQ(-0.05, p7.x);
    EXPECT_DOUBLE_EQ(-0.05, p7.y);
    EXPECT_DOUBLE_EQ(-0.05, p7.z);
}
