#include <gtest/gtest.h>

#include <Inventor/SbColor.h>

TEST(SbColor, Constructors) {
    SbColor c1(SbVec3f(1.0f, 0.0f, 1.0f));

    EXPECT_FLOAT_EQ(1.0f, c1[0]);
    EXPECT_FLOAT_EQ(0.0f, c1[1]);
    EXPECT_FLOAT_EQ(1.0f, c1[2]);

    const float rgb[] = {1.0f, 0.0f, 1.0f};
    SbColor     c2(rgb);

    EXPECT_FLOAT_EQ(1.0f, c2[0]);
    EXPECT_FLOAT_EQ(0.0f, c2[1]);
    EXPECT_FLOAT_EQ(1.0f, c2[2]);
}

TEST(SbColor, setHSV) {
    SbColor c1;
    c1.setHSVValue(0.0f, 1.0f, 1.0f);
    EXPECT_FLOAT_EQ(1.0f, c1[0]);
    EXPECT_FLOAT_EQ(0.0f, c1[1]);
    EXPECT_FLOAT_EQ(0.0f, c1[2]);

    c1.setHSVValue(1.0f, 1.0f, 1.0f);
    EXPECT_FLOAT_EQ(1.0f, c1[0]);
    EXPECT_FLOAT_EQ(0.0f, c1[1]);
    EXPECT_FLOAT_EQ(0.0f, c1[2]);

    c1.setHSVValue(1.0f / 3.0f, 1.0f, 1.0f);
    EXPECT_FLOAT_EQ(0.0f, c1[0]);
    EXPECT_FLOAT_EQ(1.0f, c1[1]);
    EXPECT_FLOAT_EQ(0.0f, c1[2]);

    c1.setHSVValue(0.3f, 0.3f, 0.3f);
    EXPECT_FLOAT_EQ(0.2280f, c1[0]);
    EXPECT_FLOAT_EQ(0.3f, c1[1]);
    EXPECT_FLOAT_EQ(0.21f, c1[2]);
}

TEST(SbColor, getHSV) {
    SbColor c1;
    float   h, s, v;

    c1.setValue(1.0f, 0.0f, 0.0f);
    c1.getHSVValue(h, s, v);
    EXPECT_FLOAT_EQ(0.0f, h);
    EXPECT_FLOAT_EQ(1.0f, s);
    EXPECT_FLOAT_EQ(1.0f, v);

    c1.setValue(0.0f, 1.0f, 0.0f);
    c1.getHSVValue(h, s, v);
    EXPECT_FLOAT_EQ(1.0f / 3.0f, h);
    EXPECT_FLOAT_EQ(1.0f, s);
    EXPECT_FLOAT_EQ(1.0f, v);

    c1.setValue(0.2280f, 0.3f, 0.21f);
    c1.getHSVValue(h, s, v);
    EXPECT_FLOAT_EQ(0.3f, h);
    EXPECT_FLOAT_EQ(0.3f, s);
    EXPECT_FLOAT_EQ(0.3f, v);
}
