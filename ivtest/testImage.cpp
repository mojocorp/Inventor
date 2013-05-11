#include "gtest/gtest.h"
#include "utils.h"

#include <Inventor/SbImage.h>

#include <QCryptographicHash>

TEST(testImage, jpg) {
    SbString filename = getenv("OIVHOME") + SbString("/data/textures/brick.jpg");

    SbImage img(filename);

    EXPECT_FALSE(img.isNull());
    EXPECT_EQ(128, img.getSize()[0]);
    EXPECT_EQ(128, img.getSize()[1]);
    EXPECT_EQ(SbImage::Format_RGB24, img.getFormat());

    QCryptographicHash hash(QCryptographicHash::Md5);
    hash.addData((const char*)img.getConstBytes(), 3*128*128);
    EXPECT_TRUE(hash.result().toHex() == "82746c26e854c66effa9044b90b356cf");
}

TEST(testImage, rgb) {
    SbString filename = getenv("OIVHOME") + SbString("/data/textures/brick.rgb");

    SbImage img(filename);

    EXPECT_FALSE(img.isNull());
    EXPECT_EQ(128, img.getSize()[0]);
    EXPECT_EQ(128, img.getSize()[1]);
    EXPECT_EQ(SbImage::Format_RGB24, img.getFormat());

    QCryptographicHash hash(QCryptographicHash::Md5);
    hash.addData((const char*)img.getConstBytes(), 3*128*128);
    EXPECT_TRUE(hash.result().toHex() == "ebd3a848456dce1df933a30bcd722cec");
}
