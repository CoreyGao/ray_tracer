#include "ray.h"
#include <gtest/gtest.h>


using Graphics::Ray;
using Math::Point3D;
using Math::Vector3D;

const int START_RATIO = -300;
const int END_RATIO = 300;


class RayTest : public ::testing::Test
{
    protected:
        virtual void SetUp()
        {
            pStart.x = -21.5;
            pStart.y = 2;
            pStart.z = 0;

            pT.x = 3.7;
            pT.y = 5.5;
            pT.z = -11;

            dir.x = 25.2;
            dir.y = 3.5;
            dir.z = -11;

            dir.Normalize();

            pInLine.x = 230.5;
            pInLine.y = 37;
            pInLine.z = -110;

            pOffLine.x = 321;
            pOffLine.y = 0;
            pOffLine.z = -5423;
        }

        virtual void TearDown()
        {

        }


        Point3D pStart;
        Point3D pT;
        Point3D pInLine;
        Point3D pOffLine;
        Vector3D dir;
};

TEST_F(RayTest, AllInOne)
{
    Ray r(pStart, pT, START_RATIO, END_RATIO);
    Ray rCopy(r);
    Ray rDir(pStart, dir, START_RATIO, END_RATIO);


    EXPECT_EQ(true, r.GetDir() == dir);
    EXPECT_EQ(START_RATIO, r.GetStartRatio());
    EXPECT_EQ(END_RATIO, r.GetEndRatio());
    EXPECT_EQ(true, r.IsPointOnRay(pInLine));
    EXPECT_EQ(true, !r.IsPointOnRay(pOffLine));

    EXPECT_EQ(true, rCopy.GetDir() == dir);
    EXPECT_EQ(START_RATIO, rCopy.GetStartRatio());
    EXPECT_EQ(END_RATIO, rCopy.GetEndRatio());
    EXPECT_EQ(true, rCopy.IsPointOnRay(pInLine));
    EXPECT_EQ(true, !rCopy.IsPointOnRay(pOffLine));

    EXPECT_EQ(true, rDir.GetDir() == dir);
    EXPECT_EQ(START_RATIO, rDir.GetStartRatio());
    EXPECT_EQ(END_RATIO, rDir.GetEndRatio());
    EXPECT_EQ(true, rDir.IsPointOnRay(pInLine));
    EXPECT_EQ(true, !rDir.IsPointOnRay(pOffLine));
}



