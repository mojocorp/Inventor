#include <sstream>
#include <iomanip>
#include <vector>
#include <sys/stat.h> // stat
#include <errno.h>    // errno, ENOENT, EEXIST
#if defined(_WIN32)
#include <direct.h> // _mkdir
#endif

#include "utils.h"
#include <CompareArgs.h>
#include <RGBAImage.h>
#include <Metric.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

namespace utils {

bool
pdiff(const SbImage &img1, const SbImage &img2) {
    if (img1.getSize() != img2.getSize()) {
        return false;
    }

    const int width = img1.getSize()[0];
    const int height = img1.getSize()[1];
    const int comp = img1.getNumComponents();

    CompareArgs args;
    args.ImgA = new RGBAImage(width, height, "ref");
    args.ImgB = new RGBAImage(width, height, "img");

    const unsigned char *p1 = img1.getConstBytes();
    const unsigned char *p2 = img2.getConstBytes();
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            const int offset = (x + y * width);
            const int alpha1 = (comp < 4) ? 255 : p1[comp * offset + 3];
            const int alpha2 = (comp < 4) ? 255 : p2[comp * offset + 3];
            args.ImgA->Set(p1[comp * offset + 0], p1[comp * offset + 1],
                           p1[comp * offset + 2], alpha1, offset);
            args.ImgB->Set(p2[comp * offset + 0], p2[comp * offset + 1],
                           p2[comp * offset + 2], alpha2, offset);
        }
    }

#if 1
    args.ImgDiff = new RGBAImage(width, height, "diff.png");
    bool res = Yee_Compare(args);
    if (!res) {
        utils::write_image(img1, "/tmp/ref.png");
        utils::write_image(img2, "/tmp/test.png");
        std::vector<int> buffer(width * height);
        for (int y = 0; y < height; y++)
            for (int x = 0; x < width; x++)
                buffer[x + y * width] = args.ImgDiff->Get(x, height - y - 1);
        stbi_write_png("/tmp/diff.png", width, height, 4, buffer.data(),
                       width * 4);
    }
    return res;
#else
    return Yee_Compare(args);
#endif
}

// https://gist.github.com/creationix/4710780

// leftrotate function definition
#define LEFTROTATE(x, c) (((x) << (c)) | ((x) >> (32 - (c))))

std::string
md5(const unsigned char *initial_msg, size_t initial_len) {

    // Message (to prepare)
    uint8_t *msg = NULL;

    // Note: All variables are unsigned 32 bit and wrap modulo 2^32 when
    // calculating

    // r specifies the per-round shift amounts

    uint32_t r[] = {7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22,
                    5, 9,  14, 20, 5, 9,  14, 20, 5, 9,  14, 20, 5, 9,  14, 20,
                    4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23,
                    6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21};

    // Use binary integer part of the sines of integers (in radians) as
    // constants// Initialize variables:
    uint32_t k[] = {
        0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee, 0xf57c0faf, 0x4787c62a,
        0xa8304613, 0xfd469501, 0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be,
        0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821, 0xf61e2562, 0xc040b340,
        0x265e5a51, 0xe9b6c7aa, 0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8,
        0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed, 0xa9e3e905, 0xfcefa3f8,
        0x676f02d9, 0x8d2a4c8a, 0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c,
        0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70, 0x289b7ec6, 0xeaa127fa,
        0xd4ef3085, 0x04881d05, 0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665,
        0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039, 0x655b59c3, 0x8f0ccc92,
        0xffeff47d, 0x85845dd1, 0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1,
        0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391};

    uint32_t h0 = 0x67452301;
    uint32_t h1 = 0xefcdab89;
    uint32_t h2 = 0x98badcfe;
    uint32_t h3 = 0x10325476;

    // Pre-processing: adding a single 1 bit
    // append "1" bit to message
    /* Notice: the input bytes are considered as bits strings,
       where the first bit is the most significant bit of the byte.[37] */

    // Pre-processing: padding with zeros
    // append "0" bit until message length in bit ≡ 448 (mod 512)
    // append length mod (2 pow 64) to message

    int new_len = ((((initial_len + 8) / 64) + 1) * 64) - 8;

    msg =
        (uint8_t *)calloc(new_len + 64, 1); // also appends "0" bits
                                            // (we alloc also 64 extra bytes...)
    memcpy(msg, initial_msg, initial_len);
    msg[initial_len] = 128; // write the "1" bit

    uint32_t bits_len = 8 * initial_len; // note, we append the len
    memcpy(msg + new_len, &bits_len, 4); // in bits at the end of the buffer

    // Process the message in successive 512-bit chunks:
    // for each 512-bit chunk of message:
    int offset;
    for (offset = 0; offset < new_len; offset += (512 / 8)) {

        // break chunk into sixteen 32-bit words w[j], 0 ≤ j ≤ 15
        uint32_t *w = (uint32_t *)(msg + offset);

        // Initialize hash value for this chunk:
        uint32_t a = h0;
        uint32_t b = h1;
        uint32_t c = h2;
        uint32_t d = h3;

        // Main loop:
        uint32_t i;
        for (i = 0; i < 64; i++) {
            uint32_t f, g;

            if (i < 16) {
                f = (b & c) | ((~b) & d);
                g = i;
            } else if (i < 32) {
                f = (d & b) | ((~d) & c);
                g = (5 * i + 1) % 16;
            } else if (i < 48) {
                f = b ^ c ^ d;
                g = (3 * i + 5) % 16;
            } else {
                f = c ^ (b | (~d));
                g = (7 * i) % 16;
            }

            uint32_t temp = d;
            d = c;
            c = b;
            b = b + LEFTROTATE((a + f + k[i] + w[g]), r[i]);
            a = temp;
        }

        // Add this chunk's hash to result so far:

        h0 += a;
        h1 += b;
        h2 += c;
        h3 += d;
    }

    // cleanup
    free(msg);

    std::stringstream ss;
    ss << std::hex << std::setfill('0');
    const uint32_t *buffer[] = {&h0, &h1, &h2, &h3};
    for (int i = 0; i < 4; i++) {
        const uint8_t *p = (uint8_t *)buffer[i];
        for (int j = 0; j < 4; j++)
            ss << std::setw(2) << (int)p[j];
    }
    return ss.str();
}

bool
write_image(const SbImage &img, const SbString &filename) {
    const int width = img.getSize()[0];
    const int height = img.getSize()[1];
    const int comp = img.getNumComponents();
    SbImage   img2 = utils::hflip_image(img);

    stbi_write_png(filename.getString(), width, height, comp,
                   img2.getConstBytes(), width * comp);

    return true;
}

SbImage
hflip_image(const SbImage &src) {
    SbImage dst = src;

    const unsigned char *psrc = src.getConstBytes();
    unsigned char *      pdst = dst.getBytes();
    const int            width = src.getSize()[0];
    const int            height = src.getSize()[1];
    const int            stride = src.getNumComponents() * width;

    for (int y = 0; y < height; y++) {
        memcpy(pdst + y * stride, psrc + (height - y - 1) * stride, stride);
    }
    return dst;
}

#ifdef _WIN32
#define stat _stat
#define S_IFDIR _S_IFDIR
#endif

bool
pathexist(const std::string &path) {
    struct stat info;
    if (stat(path.c_str(), &info) != 0) {
        return false;
    }
    return (info.st_mode & S_IFDIR) != 0;
}

bool
mkpath(std::string path) {
    if (pathexist(path))
        return true;

    while (!path.empty() && (path.back() == '/' || path.back() == '\\'))
        path.resize(path.size() - 1);

    // find parent path
    int pos = path.find_last_of('/');
    if (pos == std::string::npos)
#ifdef _WIN32
        pos = path.find_last_of('\\');
    if (pos == std::string::npos)
#endif
        return false;

    const std::string parent = path.substr(0, pos);

    // try to create parent path
    if (mkpath(parent)) {
#if defined(_WIN32)
        return (_mkdir(path.c_str()) == 0);
#else
        return (mkdir(path.c_str(), mode_t(0755)) == 0);
#endif
    }
    return false;
}

} // namespace utils
