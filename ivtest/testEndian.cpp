#include "gtest/gtest.h"

#include <machine.h>

TEST(testEndian, swap16) {
    EXPECT_EQ(swap16(0x0123), 0x2301);
    EXPECT_EQ(swap16(0x2301), 0x0123);
}

TEST(testEndian, swap32) {
    EXPECT_EQ(swap32(0x01234567), 0x67452301);
    EXPECT_EQ(swap32(0x67452301), 0x01234567);
}

TEST(testEndian, swap64) {
    EXPECT_EQ(swap64(0x0123456789abcdefll), 0xefcdab8967452301ll);
    EXPECT_EQ(swap64(0xefcdab8967452301ll), 0x0123456789abcdefll);
}

