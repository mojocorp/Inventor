#pragma once

#include <string>

#include <Inventor/SbImage.h>

namespace utils {
bool pdiff(const SbImage &img1, const SbImage &img2);

std::string md5(const unsigned char *initial_msg, size_t initial_len);

bool    write_image(const SbImage &img, const SbString &filename);
SbImage hflip_image(const SbImage &img);

bool pathexist(const std::string &path);
bool mkpath(std::string path);
} // namespace utils

#define EXPECT_IMG_EQ(expected, actual)                                        \
    EXPECT_TRUE(utils::pdiff(expected, actual))
#define EXPECT_IMG_NE(expected, actual)                                        \
    EXPECT_FALSE(utils::pdiff(expected, actual))
