#include <QImage>

#include "gtest/gtest.h"
#include "utils.h"

TEST(testUtils, pdiff) {
    QImage img1(":/ivtest/pdiff/test/Aqsis_vase.png");
    QImage img2(":/ivtest/pdiff/test/Aqsis_vase_ref.png");
    QImage img3(":/ivtest/pdiff/test/Bug1102605.png");
    QImage img4(":/ivtest/pdiff/test/Bug1102605_ref.png");
    QImage img5(":/ivtest/pdiff/test/Bug1471457.png");
    QImage img6(":/ivtest/pdiff/test/Bug1471457_ref.png");
    QImage img7(":/ivtest/pdiff/test/cam_mb.png");
    QImage img8(":/ivtest/pdiff/test/cam_mb_ref.png");
    QImage img9(":/ivtest/pdiff/test/fish1.png");
    QImage img10(":/ivtest/pdiff/test/fish2.png");

    EXPECT_IMG_EQ(img1, img1);
    EXPECT_IMG_EQ(img3, img3);

    EXPECT_IMG_NE(img1, img3);

    EXPECT_IMG_NE(img1, img2);
    EXPECT_IMG_NE(img3, img4);
    EXPECT_IMG_EQ(img5, img6);
    EXPECT_IMG_EQ(img7, img8);
    EXPECT_IMG_NE(img9, img10);
}
