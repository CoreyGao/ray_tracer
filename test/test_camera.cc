#include "camera.h"
#include <gtest/gtest.h>

using Graphics::Camera;

TEST(CameraTest, testall){
    Camera c(Math::PI / 2, 0.2, 100, 10, 10);

    EXPECT_EQ((unsigned int)10, c.GetHorizontolPixNum());
    EXPECT_EQ((unsigned int)10, c.GetVerticalPixNum());
}
