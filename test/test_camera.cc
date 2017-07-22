#include "camera.h"
#include <gtest/gtest.h>
#include "math/commondef.h"
#include <vector>
#include "ray.h"
#include <cmath>

using namespace Graphics;
using namespace ::std;
TEST(CameraTest, testall){
    Camera c(Math::PI / 2, 1, 10, 10);

    EXPECT_EQ(10, c.GetHorizontolPixNum());
    EXPECT_EQ(10, c.GetVerticalPixNum());
}
