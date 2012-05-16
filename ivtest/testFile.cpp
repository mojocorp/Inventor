#include "gtest/gtest.h"

#include <Inventor/SbFile.h>

TEST(SbFile, BaseName) {

    SbString filename("/tmp/archive.tar.gz");

    EXPECT_TRUE(SbFile::baseName(filename) == "archive.tar.gz");

    SbFile f(filename);
    EXPECT_TRUE(f.baseName() == "archive.tar.gz");
}

TEST(SbFile, Extension) {

    SbString filename("/tmp/archive.tar.gz");

    EXPECT_TRUE(SbFile::extension(filename) == "tar.gz");

    SbFile f(filename);
    EXPECT_TRUE(f.extension() == "tar.gz");
}
