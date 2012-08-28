#include <gtest/gtest.h>

#include <Inventor/SbBox3f.h>

#include <limits>

TEST(SbBox3f, Constructors) {

    {
        SbBox3f b;
        EXPECT_TRUE(b.isEmpty());
    }

    {
        SbVec3f pmin(0, 1, 2);
        SbVec3f pmax(3, 4, 5);
        SbBox3f b(0, 1, 2, 3, 4, 5);
        EXPECT_TRUE(b.getMin() == pmin);
        EXPECT_TRUE(b.getMax() == pmax);
    }

    {
        SbVec3f pmin(0, 1, 2);
        SbVec3f pmax(3, 4, 5);
        SbBox3f b(pmin, pmax);
        EXPECT_TRUE(b.getMin() == pmin);
        EXPECT_TRUE(b.getMax() == pmax);
    }
}

TEST(SbBox3f, MakeEmpty) {
    SbVec3f pmin(0, 1, 2);
    SbVec3f pmax(3, 4, 5);
    SbBox3f b(pmin, pmax);

    EXPECT_FALSE(b.isEmpty());
    EXPECT_TRUE(b.hasVolume());

    b.makeEmpty();

    EXPECT_TRUE(b.isEmpty());
    EXPECT_FALSE(b.hasVolume());
}

TEST(SbBox3f, ExtendBy) {
    SbVec3f pmin(0, 1, 2);
    SbVec3f pmax(3, 4, 5);
    SbBox3f b;

    EXPECT_TRUE(b.isEmpty());

    b.extendBy(pmin);
    EXPECT_TRUE(b.getMax() == pmin);
    EXPECT_FALSE(b.isEmpty());

    b.extendBy(pmax);
    EXPECT_TRUE(b.getMin() == pmin);
    EXPECT_TRUE(b.getMax() == pmax);
}

TEST(SbBox3f, Size) {
    SbVec3f pmin(0, 1, 2);
    SbVec3f pmax(3, 4, 5);
    SbBox3f b(pmin, pmax);

    EXPECT_TRUE(b.getSize() == SbVec3f(3, 3, 3));
}
