#include "gtest/gtest.h"

#include <machine.h>

TEST(testEndian, swap16) {
    EXPECT_EQ(0x2301, swap16(0x0123));
    EXPECT_EQ(0x0123, swap16(0x2301));
}

TEST(testEndian, swap32) {
    EXPECT_EQ(0x67452301, swap32(0x01234567));
    EXPECT_EQ(0x01234567, swap32(0x67452301));
}

TEST(testEndian, swap64) {
    EXPECT_EQ(0xefcdab8967452301ll, swap64(0x0123456789abcdefll));
    EXPECT_EQ(0x0123456789abcdefll, swap64(0xefcdab8967452301ll));
}
