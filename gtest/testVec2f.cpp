#include "gtest/gtest.h"

#include <Inventor/SbVec2f.h>
#include <Inventor/SbVec3f.h>

TEST(SbVec2f, Constructors) {

    {
        SbVec2f v(0, 0);
        EXPECT_FLOAT_EQ (v[0], 0.0);
        EXPECT_FLOAT_EQ (v[1], 0.0);
    }

    {
        SbVec2f v(2, 3);
        EXPECT_FLOAT_EQ (v[0], 2.0);
        EXPECT_FLOAT_EQ (v[1], 3.0);
    }

    {
        const float a[]={ 2.0f, 3.0f};
        SbVec2f v(a);
        EXPECT_FLOAT_EQ (v[0], 2.0);
        EXPECT_FLOAT_EQ (v[1], 3.0);
    }
}

TEST(SbVec2f, Length) {

    SbVec2f v(0, 0);
    EXPECT_FLOAT_EQ (v.length(), 0.0);
    //EXPECT_FLOAT_EQ (v.normalized().length(), 0.0);
    v.normalize();
    EXPECT_FLOAT_EQ (v.length(), 0.0);

    v = SbVec2f(3, 4);
    EXPECT_FLOAT_EQ (v.length(), 5);
    v.normalize();
    EXPECT_FLOAT_EQ (v.length(), 1.0);

    v = SbVec2f(3000, 4000);
    EXPECT_FLOAT_EQ (v.length(), 5000);
    v.normalize();
    EXPECT_FLOAT_EQ (v.length(), 1.0);
}

TEST(SbVec2f, Negate) {
    SbVec2f a(20, 30);
    a.negate();
    EXPECT_TRUE(a == SbVec2f(-20, -30));
    EXPECT_TRUE(a == -SbVec2f(20, 30));
}

TEST(SbVec2f, Dot) {
    SbVec2f a(3, 4);
    EXPECT_FLOAT_EQ(a.dot(a), 25);
    EXPECT_FLOAT_EQ(a.dot(SbVec2f(4, -3)), 0);
}

TEST(SbVec2f, Equality) {
    EXPECT_TRUE(SbVec2f(2, 3) == SbVec2f(2, 3));
    EXPECT_FALSE(SbVec2f(2, 3) == SbVec2f(0, 3));
    EXPECT_FALSE(SbVec2f(2, 3) == SbVec2f(2, 0));

    EXPECT_FALSE(SbVec2f(2, 3) != SbVec2f(2, 3));
    EXPECT_TRUE(SbVec2f(2, 3) != SbVec2f(0, 3));
    EXPECT_TRUE(SbVec2f(2, 3) != SbVec2f(2, 0));
}

TEST(SbVec2f, Add) {
    {
        SbVec2f a(20, 30);
        SbVec2f b(2, 3);
        EXPECT_TRUE(a + b == SbVec2f(22, 33));
    }

    {
        SbVec2f a(20, 30);
        SbVec2f b(2, 3);
        a += b;
        EXPECT_TRUE(a == SbVec2f(22, 33));
    }
}

TEST(SbVec2f, Sub) {
    {
        SbVec2f a(22, 33);
        SbVec2f b(2, 3);
        EXPECT_TRUE(a - b == SbVec2f(20, 30));
    }

    {
        SbVec2f a(22, 33);
        SbVec2f b(2, 3);
        a -= b;
        EXPECT_TRUE(a == SbVec2f(20, 30));
    }
}

TEST(SbVec2f, Mult) {
    {
        SbVec2f a(3, 4);
        EXPECT_TRUE((a * 10) == SbVec2f(30, 40));
        EXPECT_TRUE((10 * a) == SbVec2f(30, 40));
        EXPECT_TRUE(a == SbVec2f(3, 4));
    }

    {
        SbVec2f a(3, 4);
        a *= 10;
        EXPECT_TRUE(a == SbVec2f(30, 40));
    }
}

TEST(SbVec2f, Div) {
    {
        SbVec2f a(31, 41);
        EXPECT_TRUE((a / 10.0).equals(SbVec2f(3.1, 4.1),1E-6));
        EXPECT_TRUE(a == SbVec2f(31, 41));
    }

    {
        SbVec2f a(31, 41);
        a /= 10.0;
        EXPECT_TRUE(a.equals(SbVec2f(3.1, 4.1), 1E-6));
    }
}
