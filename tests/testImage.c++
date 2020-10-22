#include "gtest/gtest.h"
#include "utils.h"

#include <SoDebug.h>
#include <Inventor/SbImage.h>

TEST(testImage, formats) {
    ASSERT_EQ(0, SbImage::Format_Invalid);
    ASSERT_EQ(1, SbImage::Format_Luminance);
    ASSERT_EQ(2, SbImage::Format_Luminance_Alpha);
    ASSERT_EQ(3, SbImage::Format_RGB24);
    ASSERT_EQ(4, SbImage::Format_RGBA32);
}

TEST(testImage, jpg) {
    const SbString oivhome = SoDebug::GetEnv("OIVHOME");

    ASSERT_FALSE(oivhome.isEmpty()) << "OIVHOME is not set or empty!";

    SbString filename = oivhome + "/data/textures/brick.jpg";

    SbImage img(filename);

    EXPECT_FALSE(img.isNull());
    EXPECT_EQ(128, img.getSize()[0]);
    EXPECT_EQ(128, img.getSize()[1]);
    EXPECT_EQ(SbImage::Format_RGB24, img.getFormat());

    EXPECT_TRUE(utils::md5(img.getConstBytes(), 3 * 128 * 128) ==
                "82746c26e854c66effa9044b90b356cf");
}

TEST(testImage, rgb) {
    const SbString oivhome = SoDebug::GetEnv("OIVHOME");

    ASSERT_FALSE(oivhome.isEmpty()) << "OIVHOME is not set or empty!";

    SbString filename = oivhome + "/data/textures/brick.rgb";

    SbImage img(filename);

    EXPECT_FALSE(img.isNull());
    EXPECT_EQ(128, img.getSize()[0]);
    EXPECT_EQ(128, img.getSize()[1]);
    EXPECT_EQ(SbImage::Format_RGB24, img.getFormat());

    EXPECT_TRUE(utils::md5(img.getConstBytes(), 3 * 128 * 128) ==
                "ebd3a848456dce1df933a30bcd722cec");
}
