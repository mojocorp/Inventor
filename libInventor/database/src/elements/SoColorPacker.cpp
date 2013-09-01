#include <Inventor/elements/SoColorPacker.h>

///////////////////////////////////////////////////////////////////////////
//
// class:  SoColorPacker
//
// Maintains a packed color array to store current colors.  Intended to
// be used by all property nodes that can issue setDiffuse or setTransparency
/////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
//
// Description:
//  constructor for  SoColorPacker
//
// use: public
//
////////////////////////////////////////////////////////////////////////////
SoColorPacker::
SoColorPacker()
{
    packedColors = NULL;
    packedArraySize = 0;
    //Assign nodeids that can never occur in practice:
    diffuseNodeId = transpNodeId = 2;
}

/////////////////////////////////////////////////////////////////////////////
//
// Description:
//  destructor
//
//  use: public
////////////////////////////////////////////////////////////////////////////
SoColorPacker::~SoColorPacker()
{
    if(packedColors != NULL) delete [] packedColors;
}

////////////////////////////////////////////////////////////////////////////
//
// Description:
//
//  reallocate packed color array for SoColorPacker
//
// use: public, SoINTERNAL
//
////////////////////////////////////////////////////////////////////////////
void
SoColorPacker::reallocate(size_t size)
{
    if (packedColors != NULL) delete [] packedColors;
    packedColors = new uint32_t[size];
    packedArraySize = size;
}

void
SoColorPacker::packColors(const SbColor *diffuseColors, size_t numDiffuseColors,
                          const float *transparencies, size_t numTransparencies)
{
    //First determine if we have enough space:
    if (packedArraySize < numDiffuseColors)
        reallocate(numDiffuseColors);

    SbBool multTrans = (numTransparencies >= numDiffuseColors);
    for (size_t i=0; i< numDiffuseColors; i++) {
        packedColors[i] = diffuseColors[i].getPackedValue(transparencies[multTrans ? i : 0]);
    }
}
