#include "gtest/gtest.h"

#include <Inventor/SbFile.h>

TEST(SbFile, BaseName) {

    SbString filename1("/tmp/archive.tar.gz");
    SbString filename2("C:/tmp\\archive.tar.gz");

    EXPECT_TRUE(SbFile::baseName(filename1) == "archive.tar.gz");
    EXPECT_TRUE(SbFile::baseName(filename2) == "archive.tar.gz");

    SbString chineeseFont = SbString::fromUtf8("/Library/Fonts/\345\215\216\346\226\207\344\273\277\345\256\213.ttf");
    EXPECT_TRUE(SbFile::baseName(chineeseFont) == SbString::fromUtf8("\345\215\216\346\226\207\344\273\277\345\256\213.ttf"));

}

TEST(SbFile, Extension) {

    SbString filename("/tmp/../archive.tar.gz");

    EXPECT_TRUE(SbFile::extension(filename) == "tar.gz");

    SbString chineeseFont = SbString::fromUtf8("/Library/Fonts/\345\215\216\346\226\207\344\273\277\345\256\213.ttf");
    EXPECT_TRUE(SbFile::extension(chineeseFont) == "ttf");
}

TEST(SbFile, Dirname) {
    SbString filename1("/tmp/archive.tar.gz");
    SbString filename2("C:/tmp\\archive.tar.gz");

    EXPECT_TRUE(SbFile::dirName(filename1) == "/tmp/");
    EXPECT_TRUE(SbFile::dirName(filename2) == "C:/tmp\\");
}

// Helper to create an unicode file on disk.
SbString createUnicodeFilename()
{
    SbString chineeseFilename = SbString::fromUtf8("\345\215\216\346\226\207\344\273\277\345\256\213.ttf");
    if (!SbFile::exists(chineeseFilename)) {
        SbFile file;
        if (file.open(chineeseFilename, "wb")) {
            file.close();
        }else{
            printf("ERROR: Cannot create unicode file for testing.\n");
        }
    }

    return chineeseFilename;
}

TEST(SbFile, Exist) {
    SbString filename = createUnicodeFilename();

    EXPECT_TRUE(SbFile::exists(filename));
    EXPECT_FALSE(SbFile::exists("dummyfile"));
    EXPECT_FALSE(SbFile::exists(""));
}

TEST(SbFile, Open) {
    SbString filename = createUnicodeFilename();

    SbFile file;
    EXPECT_TRUE(file.open(filename, "rb"));
    EXPECT_TRUE(file.isOpen());
    file.close();
    EXPECT_FALSE(file.isOpen());
}
