#include <vector>

#include <SoDebug.h>
#include <Inventor/SbFile.h>

#include "gtest/gtest.h"
#include "utils.h"

TEST(testUtils, pdiff) {
    const SbString oivhome = SoDebug::GetEnv("OIVHOME");

    ASSERT_FALSE(oivhome.isEmpty()) << "OIVHOME is not set or empty!";

    const SbImage img1(oivhome +
                       "/3rdparty/perceptualdiff/test/Aqsis_vase.png");
    const SbImage img2(oivhome +
                       "/3rdparty/perceptualdiff/test/Aqsis_vase_ref.png");
    const SbImage img3(oivhome +
                       "/3rdparty/perceptualdiff/test/Bug1102605.png");
    const SbImage img4(oivhome +
                       "/3rdparty/perceptualdiff/test/Bug1102605_ref.png");
    const SbImage img5(oivhome +
                       "/3rdparty/perceptualdiff/test/Bug1471457.png");
    const SbImage img6(oivhome +
                       "/3rdparty/perceptualdiff/test/Bug1471457_ref.png");
    const SbImage img7(oivhome + "/3rdparty/perceptualdiff/test/cam_mb.png");
    const SbImage img8(oivhome +
                       "/3rdparty/perceptualdiff/test/cam_mb_ref.png");
    const SbImage img9(oivhome + "/3rdparty/perceptualdiff/test/fish1.png");
    const SbImage img10(oivhome + "/3rdparty/perceptualdiff/test/fish2.png");

    EXPECT_FALSE(img1.isNull());
    EXPECT_TRUE(img9.getFormat() == SbImage::Format_RGBA32);
    EXPECT_TRUE(img10.getFormat() == SbImage::Format_RGBA32);

    EXPECT_IMG_EQ(img1, img1);
    EXPECT_IMG_EQ(img3, img3);

    EXPECT_IMG_NE(img1, img3);

    EXPECT_IMG_NE(img1, img2);
    EXPECT_IMG_NE(img3, img4);
    EXPECT_IMG_EQ(img5, img6);
    EXPECT_IMG_EQ(img7, img8);
    EXPECT_IMG_NE(img9, img10);
}

TEST(testUtils, md5) {
    const SbString oivhome = SoDebug::GetEnv("OIVHOME");

    ASSERT_FALSE(oivhome.isEmpty()) << "OIVHOME is not set or empty!";

    const SbString filename =
        oivhome + "/3rdparty/perceptualdiff/test/Aqsis_vase.png";

    const int64_t size = SbFile::size(filename);
    EXPECT_EQ(29973, size);

    SbFile file;
    EXPECT_TRUE(file.open(filename, "rb"));
    std::vector<unsigned char> data(size);
    EXPECT_EQ(size, file.read(data.data(), 1, size));
    EXPECT_EQ("d74e61fbc163c07c7deaa30648704e87",
              utils::md5(data.data(), size));
}

TEST(testUtils, pathexist) {
    const SbString oivhome = SoDebug::GetEnv("OIVHOME");

    ASSERT_FALSE(oivhome.isEmpty()) << "OIVHOME is not set or empty!";
    EXPECT_TRUE(utils::pathexist(oivhome.getString()));
}
