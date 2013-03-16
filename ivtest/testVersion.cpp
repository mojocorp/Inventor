#include "gtest/gtest.h"

#include <QString>

#include <Inventor/SbBasic.h>

TEST(testVersion, allDefinesAreCoherent) {

    EXPECT_STREQ(SO_VERSION_STR, qPrintable(QString("%1.%2.%3").arg(SO_VERSION_MAJOR)
                                                               .arg(SO_VERSION_MINOR)
                                                               .arg(SO_VERSION_PATCH)));

    // SO_VERSION expands a numeric value of the form 0xMMNNPP (MM = major, NN = minor, PP = patch)
    EXPECT_EQ(SO_VERSION, QString("0x%1").arg(SO_VERSION_MAJOR*10000 + SO_VERSION_MINOR*100 + SO_VERSION_PATCH).toInt(0, 16));
}
