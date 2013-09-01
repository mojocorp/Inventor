/*
 *
 *  Copyright (C) 2000 Silicon Graphics, Inc.  All Rights Reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  Further, this software is distributed without any warranty that it is
 *  free of the rightful claim of any third person regarding infringement
 *  or the like.  Any license provided herein, whether implied or
 *  otherwise, applies only to this software file.  Patent licenses, if
 *  any, provided herein do not apply to combinations of this program with
 *  other software, or any other product whatsoever.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 *  Contact information: Silicon Graphics, Inc., 1600 Amphitheatre Pkwy,
 *  Mountain View, CA  94043, or:
 *
 *  http://www.sgi.com
 *
 *  For further information regarding this notice, see:
 *
 *  http://oss.sgi.com/projects/GenInfo/NoticeExplan/
 *
 */

//  -*- C++ -*-

/*
 * Copyright (C) 1990,91   Silicon Graphics, Inc.
 *
 _______________________________________________________________________
 ______________  S I L I C O N   G R A P H I C S   I N C .  ____________
 |
 |   $Revision: 1.1 $
 |
 |   Description:
 |    This file defines the SoColorPacker class.
 |
 |   Author(s)        : Alan Norton, Gavin Bell
 |
 ______________  S I L I C O N   G R A P H I C S   I N C .  ____________
 _______________________________________________________________________
 */

#ifndef  _SO_COLOR_PACKER
#define  _SO_COLOR_PACKER

///////////////////////////////////////////////////////////////////////////
//
// Class: SoColorPacker
// This class is meant to be used by all property nodes that set either
// a diffuse color or transparency in the lazy element.  It maintains
// a cache of the current diffuse color and transparency in a packed
// color array.
//////////////////////////////////////////////////////////////////////////

#include <Inventor/SbBasic.h>
#include <Inventor/SbColor.h>

class INVENTOR_API SoColorPacker {
public:
    //Constructor, makes a colorPacker with NULL packedColor pointer:
    SoColorPacker();

    // destructor, deletes packed color array
    ~SoColorPacker();

    uint32_t* getPackedColors() const
        { return packedColors;}

    SbBool diffuseMatch(uint32_t nodeId) const
        { return (nodeId == diffuseNodeId);}

    SbBool transpMatch(uint32_t nodeId) const
        { return (nodeId == transpNodeId);}

    void setNodeIds(uint32_t diffNodeId, uint32_t tNodeId)
        {diffuseNodeId = diffNodeId; transpNodeId = tNodeId;}

    size_t getSize() const
        { return packedArraySize;}

    //Pack the current diffuse and transparency
    void packColors(const SbColor *diffuseColors, size_t numDiffuseColors,
                    const float *transparencies, size_t numTransparencies);
private:
    void reallocate(size_t size);

    // nodeids are used for testing cache validity
    uint32_t    transpNodeId;
    uint32_t    diffuseNodeId;
    // array of packed colors, or NULL if empty
    uint32_t*    packedColors;
    // size of packed color array (not necessarily number of valid colors)
    size_t    packedArraySize;
};

#endif /* _SO_COLOR_PACKER */
