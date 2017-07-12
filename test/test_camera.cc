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

    /** 
     * logically following test can pass. but due to the precision problem, all tests failed
     * still don't know how to handler precision problems.
     * **/

    const vector<vector<Point3D>> &ps=  c.GetPoints();
    for(int i = 0 ; i < c.GetVerticalPixNum() ; i++){
        for(int j = 0 ; j < c.GetHorizontolPixNum(); j++){
            EXPECT_NEAR(ps[i][j].x,  -1, 0.001);
            EXPECT_NEAR(ps[i][j].y, -1 + (j + 0.5)/5, 0.001);
            EXPECT_NEAR(ps[i][j].z, -1 + (i + 0.5)/5, 0.001);
        }
    }

    
}
