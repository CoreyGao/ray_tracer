#include "color.h"
#include <gtest/gtest.h>

using Graphics::RGB;

TEST(ColorTest, Constructor){
    RGB c;
    EXPECT_FLOAT_EQ(0, c.r);
    EXPECT_FLOAT_EQ(0, c.g);
    EXPECT_FLOAT_EQ(0, c.b);

    RGB a(0.1, 0.1, 0.1);
    EXPECT_FLOAT_EQ(0.1, a.r);
    EXPECT_FLOAT_EQ(0.1, a.g);
    EXPECT_FLOAT_EQ(0.1, a.b);

    RGB r(a);
    EXPECT_FLOAT_EQ(0.1, r.r);
    EXPECT_FLOAT_EQ(0.1, r.g);
    EXPECT_FLOAT_EQ(0.1, r.b);
}

TEST(ColorTest, Op){
    RGB a(0.1, 0.1, 0.1);
    RGB b(0.2, 0.2, 0.2);

    a+=b;
    EXPECT_FLOAT_EQ(0.3, a.r);
    EXPECT_FLOAT_EQ(0.3, a.g);
    EXPECT_FLOAT_EQ(0.3, a.b);

    RGB c =  a * b;
    EXPECT_FLOAT_EQ(0.06, c.r);
    EXPECT_FLOAT_EQ(0.06, c.g);
    EXPECT_FLOAT_EQ(0.06, c.b);

    RGB d = c * 5;
    EXPECT_FLOAT_EQ(0.3, d.r);
    EXPECT_FLOAT_EQ(0.3, d.g);
    EXPECT_FLOAT_EQ(0.3, d.b);
    
    RGB e = d + c;
    EXPECT_FLOAT_EQ(0.36, e.r);
    EXPECT_FLOAT_EQ(0.36, e.g);
    EXPECT_FLOAT_EQ(0.36, e.b);

}
