#include "utils.h"
#include "CompareArgs.h"
#include "RGBAImage.h"
#include "Metric.h"

bool pdiff(const QImage & img1, const QImage & img2)
{
    if (img1.width() != img2.width() ||
        img1.height() != img2.height())
        return false;

    CompareArgs args;

    args.ImgA = new RGBAImage(img1.width(), img1.height(), "ref");
    args.ImgB = new RGBAImage(img2.width(), img2.height(), "img");
    for (int y=0; y<img1.height(); y++) {
        for (int x=0; x<img1.width(); x++) {
            args.ImgA->Set(x, y, img1.pixel(x, y));
            args.ImgB->Set(x, y, img2.pixel(x, y));
        }
    }
    //args.ImgDiff = new RGBAImage(img1.width(), img1.height(), "diff.png");
    return Yee_Compare(args);
}
