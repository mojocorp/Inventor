#ifndef UTILS_H
#define UTILS_H

#include <QImage>

bool pdiff(const QImage & img1, const QImage & img2);

#define EXPECT_IMG_EQ(expected, actual) EXPECT_TRUE(pdiff(expected, actual))
#define EXPECT_IMG_NE(expected, actual) EXPECT_FALSE(pdiff(expected, actual))

#endif
