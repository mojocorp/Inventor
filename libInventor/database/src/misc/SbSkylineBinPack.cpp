#include "SbSkylineBinPack.h"

#include <limits>
#include <cassert>

using namespace std;

SbSkylineBinPack::SbSkylineBinPack()
    : binSize(0, 0)
{
}

SbSkylineBinPack::SbSkylineBinPack(const SbVec2s & size)
{
    init(size);
}

void SbSkylineBinPack::init(const SbVec2s & size)
{
    binSize = size;

    skyLine.clear();
    SkylineNode node;
    node.x = 0;
    node.y = 0;
    node.width = binSize[0];
    skyLine.push_back(node);
}

bool SbSkylineBinPack::insert(int width, int height, SbBox2s & rect, bool allowRotation)
{
    int bestIndex = -1;

    // Used to break ties if there are nodes at the same level. Then pick the narrowest one.
    int bestWidth = std::numeric_limits<int>::max();
    int bestHeight = std::numeric_limits<int>::max();

    rect.makeEmpty();

    for (size_t i = 0; i < skyLine.size(); ++i) {
        int y;
        if (rectangleFits(i, width, height, y)) {
            if (y + height < bestHeight || (y + height == bestHeight && skyLine[i].width < bestWidth)) {
                bestHeight = y + height;
                bestIndex = i;
                bestWidth = skyLine[i].width;
                rect.setBounds(skyLine[i].x, y, skyLine[i].x + width, y + height);
            }
        }
        if (allowRotation && rectangleFits(i, height, width, y)) {
            if (y + width < bestHeight || (y + width == bestHeight && skyLine[i].width < bestWidth)) {
                bestHeight = y + width;
                bestIndex = i;
                bestWidth = skyLine[i].width;
                rect.setBounds(skyLine[i].x, y, skyLine[i].x + height, y + width);
            }
        }
    }

    if (bestIndex != -1) {
        // Perform the actual packing.
        addLevel(bestIndex, rect);
    }

    return (bestIndex != -1);
}

bool SbSkylineBinPack::rectangleFits(int skylineNodeIndex, int width, int height, int &y) const
{
    int x = skyLine[skylineNodeIndex].x;

    if (x + width > binSize[0])
        return false;

    int widthLeft = width;
    int i = skylineNodeIndex;
    y = skyLine[skylineNodeIndex].y;
    while (widthLeft > 0) {
        y = max(y, skyLine[i].y);
        if (y + height > binSize[1])
            return false;
        widthLeft -= skyLine[i].width;
        ++i;
        assert(i < (int)skyLine.size() || widthLeft <= 0);
    }
    return true;
}

void SbSkylineBinPack::addLevel(int skylineNodeIndex, const SbBox2s & rect)
{
    SkylineNode newNode;
    newNode.x = rect.getMin()[0];
    newNode.y = rect.getMax()[1];
    newNode.width = rect.getMax()[0] - rect.getMin()[0];
    skyLine.insert(skyLine.begin() + skylineNodeIndex, newNode);

    assert(newNode.x + newNode.width <= binSize[0]);
    assert(newNode.y <= binSize[1]);

    for (size_t i = skylineNodeIndex+1; i < skyLine.size(); i++) {
        assert(skyLine[i-1].x <= skyLine[i].x);

        if (skyLine[i].x < skyLine[i-1].x + skyLine[i-1].width) {
            int shrink = skyLine[i-1].x + skyLine[i-1].width - skyLine[i].x;

            skyLine[i].x += shrink;
            skyLine[i].width -= shrink;

            if (skyLine[i].width <= 0) {
                skyLine.erase(skyLine.begin() + i);
                --i;
            }
            else
                break;
        }
        else
            break;
    }
    merge();
}

void SbSkylineBinPack::merge()
{
    for (size_t i = 0; i < skyLine.size()-1; i++) {
        if (skyLine[i].y == skyLine[i+1].y) {
            skyLine[i].width += skyLine[i+1].width;
            skyLine.erase(skyLine.begin() + (i+1));
            --i;
        }
    }
}
