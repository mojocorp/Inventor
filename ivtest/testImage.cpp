#include "gtest/gtest.h"
#include "utils.h"

#include <Inventor/SbImage.h>

TEST(testImage, jpg) {
    SbString filename = getenv("OIV_HOME") + SbString("/data/textures/brick.jpg");

    SbImage img(filename);

    EXPECT_FALSE(img.isNull());
    EXPECT_EQ(128, img.getSize()[0]);
    EXPECT_EQ(128, img.getSize()[1]);
    EXPECT_EQ(SbImage::Format_RGB24, img.getFormat());

    EXPECT_EQ(147, img.getConstBytes()[3*(0+0*128)]);
    EXPECT_EQ(188, img.getConstBytes()[3*(127+0*128)]);
    EXPECT_EQ(213, img.getConstBytes()[3*(127+127*128)]);
    EXPECT_EQ(169, img.getConstBytes()[3*(0+127*128)]);
    EXPECT_EQ(204, img.getConstBytes()[3*(64+64*128)]);
}

TEST(testImage, rgb) {
    SbString filename = getenv("OIV_HOME") + SbString("/data/textures/brick.rgb");

    SbImage img(filename);

    EXPECT_FALSE(img.isNull());
    EXPECT_EQ(128, img.getSize()[0]);
    EXPECT_EQ(128, img.getSize()[1]);
    EXPECT_EQ(SbImage::Format_RGB24, img.getFormat());

    EXPECT_EQ(151, img.getConstBytes()[3*(0+0*128)]);
    EXPECT_EQ(184, img.getConstBytes()[3*(127+0*128)]);
    EXPECT_EQ(211, img.getConstBytes()[3*(127+127*128)]);
    EXPECT_EQ(172, img.getConstBytes()[3*(0+127*128)]);
    EXPECT_EQ(205, img.getConstBytes()[3*(64+64*128)]);
}
