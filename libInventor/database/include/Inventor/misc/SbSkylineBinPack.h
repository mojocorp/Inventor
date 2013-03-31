#ifndef SB_SKYLINEBINPACK_H
#define SB_SKYLINEBINPACK_H

#include <Inventor/SbVec2s.h>
#include <Inventor/SbBox2s.h>

#include <vector>

/// Implements bin packing algorithms that use the SKYLINE data structure to store the bin contents. Uses
/// GuillotineBinPack as the waste map.
class SbSkylineBinPack
{
public:

    /// Instantiates a bin of size (0,0). Call Init to create a new bin.
    SbSkylineBinPack();

    /// Instantiates a bin of the given size.
    SbSkylineBinPack(const SbVec2s & size);

    /// (Re)initializes the packer to an empty bin of width x height units. Call whenever
    /// you need to restart with a new bin.
    void init(const SbVec2s & size);

    /// Inserts a single rectangle into the bin, possibly rotated.
    bool insert(int width, int height, SbBox2s & rect, bool allowRotation = true);

    const SbVec2s & getSize() const { return binSize; }
private:
    SbVec2s binSize;

    /// Represents a single level (a horizontal line) of the skyline/horizon/envelope.
    struct SkylineNode
    {
        /// The starting x-coordinate (leftmost).
        int x;

        /// The y-coordinate of the skyline level line.
        int y;

        /// The line width. The ending coordinate (inclusive) will be x+width-1.
        int width;
    };

    std::vector<SkylineNode> skyLine;

    bool rectangleFits(int skylineNodeIndex, int width, int height, int &y) const;

    void addLevel(int skylineNodeIndex, const SbBox2s &rect);

    /// Merges all skyline nodes that are at the same level.
    void merge();
};

#endif /* SB_SKYLINEBINPACK_H */
