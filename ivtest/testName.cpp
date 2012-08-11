#include "gtest/gtest.h"

#include <Inventor/SbName.h>

TEST(testName, Constructors) {
    SbName name1;

    EXPECT_TRUE(!name1);

    SbName name2("an ascii string");

    EXPECT_TRUE(name2 == "an ascii string");

    SbName name3(name2);
    EXPECT_TRUE(name3 == name2);

    SbName name4(SbString("an ascii string"));
    EXPECT_TRUE(name4 == name2);
}

TEST(testName, length) {
    SbName name("an ascii string");
    EXPECT_EQ(15, name.getLength());
}

TEST(testName, find) {
    SbName name("an ascii string");

    EXPECT_EQ(-1, name.find('z'));
    EXPECT_EQ( 0, name.find('a'));
    EXPECT_EQ( 6, name.find('i'));
    EXPECT_EQ(14, name.find('g'));
}

TEST(testName, rfind) {
    SbName name("an ascii string");

    EXPECT_EQ(-1, name.rfind('z'));
    EXPECT_EQ( 3, name.rfind('a'));
    EXPECT_EQ(12, name.rfind('i'));
    EXPECT_EQ(14, name.rfind('g'));
}

TEST(testName, operators)
{
    SbName name("an ascii string");

    EXPECT_TRUE(name == "an ascii string");
    EXPECT_TRUE("an ascii string" == name);
    EXPECT_TRUE(name == SbName("an ascii string"));

    EXPECT_TRUE(name != "");
    EXPECT_TRUE(name != "another ascii string");
    EXPECT_TRUE("another ascii string" != name);
    EXPECT_TRUE(name != SbName("another ascii string"));

    EXPECT_TRUE(SbName("abc") < SbName("def"));
}
