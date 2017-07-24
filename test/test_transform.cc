#include <gtest/gtest.h>
#include "transform.h"

using Graphics::Transform;
using Math::Matrix33;
using Math::Point3D;
using Math::Vector3D;
using Math::PI;

TEST(TransformTest, Basic)
{
    Transform t;
    EXPECT_TRUE(t.GetBasis() == Matrix33(1, 0, 0, 0, 1, 0, 0, 0, 1));
    EXPECT_TRUE(t.GetPos() == Point3D());
}

TEST(TransformTest, Op)
{
    Transform l;
    l.Move(Vector3D(1, 0, 0));
    EXPECT_TRUE(l.GetPos() == Point3D(1, 0, 0));

    Transform t;
    t.RotateRoundX(PI/2);
    EXPECT_TRUE(t.GetBasis() == Matrix33(1, 0, 0, 0, 0, 1, 0, -1, 0));

    t.RotateRoundY(PI/2);
    EXPECT_TRUE(t.GetBasis() == Matrix33(0, 1, 0, 0, 0, 1, 1, 0, 0));

    t.RotateRoundZ(PI/2);
    EXPECT_TRUE(t.GetBasis() == Matrix33(0, 0, 1,  0, -1, 0, 1, 0, 0));

    Transform m;
    m.Rotate(Vector3D(1, 1, 1), PI * 2/3);
    EXPECT_TRUE(m.GetBasis() == Matrix33(0, 1, 0, 0, 0, 1, 1, 0, 0));
}
