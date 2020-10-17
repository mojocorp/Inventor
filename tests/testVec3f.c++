#include <gtest/gtest.h>

#include <Inventor/SbVec.h>

TEST(SbVec3f, Size) {
    //  WARNING!!!!!  Transcription of arrays of this class assume that the
    //                only data stored in this class are three consecutive
    //                values. Do not add any extra data members!!!
    EXPECT_EQ(3 * sizeof(float), sizeof(SbVec3f));
}

TEST(SbVec3f, Constructors) {

    {
        SbVec3f v(0.0f, 0.0f, 0.0f);
        EXPECT_FLOAT_EQ(v[0], 0.0f);
        EXPECT_FLOAT_EQ(v[1], 0.0f);
        EXPECT_FLOAT_EQ(v[2], 0.0f);
    }

    {
        SbVec3f v(2.0f, 3.0f, 4.0f);
        EXPECT_FLOAT_EQ(v[0], 2.0f);
        EXPECT_FLOAT_EQ(v[1], 3.0f);
        EXPECT_FLOAT_EQ(v[2], 4.0f);
    }

    {
        const float a[] = {2.0f, 3.0f, 4.0f};
        SbVec3f     v(a);
        EXPECT_FLOAT_EQ(v[0], 2.0f);
        EXPECT_FLOAT_EQ(v[1], 3.0f);
        EXPECT_FLOAT_EQ(v[2], 4.0f);
    }
}

TEST(SbVec3f, Length) {

    SbVec3f v(0.0f, 0.0f, 0.0f);
    EXPECT_FLOAT_EQ(v.length(), 0.0f);
    v.normalize();
    EXPECT_FLOAT_EQ(v.length(), 0.0f);

    v = SbVec3f(2.0f, 3.0f, 4.0f);
    EXPECT_FLOAT_EQ(v.length(), 5.3851647f);
    v.normalize();
    EXPECT_FLOAT_EQ(v.length(), 1.0f);

    v = SbVec3f(2000.0f, 3000.0f, 4000.0f);
    EXPECT_FLOAT_EQ(v.length(), 5385.165f);
    v.normalize();
    EXPECT_FLOAT_EQ(v.length(), 1.0f);
}

TEST(SbVec3f, Negate) {
    SbVec3f a(20.0f, 30.0f, 40.0f);
    a.negate();
    EXPECT_TRUE(a == SbVec3f(-20.0f, -30.0f, -40.0f));
    EXPECT_TRUE(a == -SbVec3f(20.0f, 30.0f, 40.0f));
}

TEST(SbVec3f, Dot) {
    SbVec3f a(2, 3, 4);
    EXPECT_FLOAT_EQ(29, a.dot(a));
    EXPECT_FLOAT_EQ(4.0f, a.dot(SbVec3f(2, 4, -3)));
}

TEST(SbVec3f, Equality) {
    EXPECT_TRUE(SbVec3f(2, 3, 4) == SbVec3f(2, 3, 4));
    EXPECT_FALSE(SbVec3f(2, 3, 4) == SbVec3f(0, 3, 2));

    EXPECT_FALSE(SbVec3f(2, 3, 4) != SbVec3f(2, 3, 4));
    EXPECT_TRUE(SbVec3f(2, 3, 4) != SbVec3f(0, 3, 4));
}

TEST(SbVec3f, Add) {
    {
        SbVec3f a(20, 30, 40);
        SbVec3f b(2, 3, 4);
        EXPECT_TRUE(a + b == SbVec3f(22, 33, 44));
    }

    {
        SbVec3f a(20, 30, 40);
        SbVec3f b(2, 3, 4);
        a += b;
        EXPECT_TRUE(a == SbVec3f(22, 33, 44));
    }
}

TEST(SbVec3f, Sub) {
    {
        SbVec3f a(22, 33, 44);
        SbVec3f b(2, 3, 4);
        EXPECT_TRUE(a - b == SbVec3f(20, 30, 40));
    }

    {
        SbVec3f a(22, 33, 44);
        SbVec3f b(2, 3, 4);
        a -= b;
        EXPECT_TRUE(a == SbVec3f(20, 30, 40));
    }
}

TEST(SbVec3f, Mult) {
    {
        SbVec3f a(2, 3, 4);
        EXPECT_TRUE((a * 10) == SbVec3f(20, 30, 40));
        EXPECT_TRUE((10 * a) == SbVec3f(20, 30, 40));
        EXPECT_TRUE(a == SbVec3f(2, 3, 4));
    }

    {
        SbVec3f a(2, 3, 4);
        a *= 10;
        EXPECT_TRUE(a == SbVec3f(20, 30, 40));
    }
}

TEST(SbVec3f, Div) {
    {
        SbVec3f a(21, 31, 41);
        EXPECT_TRUE((a / 10.0).equals(SbVec3f(2.1, 3.1, 4.1), 1E-6));
        EXPECT_TRUE(a == SbVec3f(21, 31, 41));
    }

    {
        SbVec3f a(21, 31, 41);
        a /= 10.0;
        EXPECT_TRUE(a.equals(SbVec3f(2.1, 3.1, 4.1), 1E-6));
    }
}
