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

/*
 * Copyright (C) 1990,91   Silicon Graphics, Inc.
 *
 _______________________________________________________________________
 ______________  S I L I C O N   G R A P H I C S   I N C .  ____________
 |
 |   $Revision: 1.1 $
 |
 |   Classes:
 |	SoSFImage
 |
 |   Author(s)		: Gavin Bell
 |
 ______________  S I L I C O N   G R A P H I C S   I N C .  ____________
 _______________________________________________________________________
 */

#include <Inventor/fields/SoSFImage.h>

#include <cstring>

//////////////////////////////////////////////////////////////////////////////
//
// SoSFImage class
//
//////////////////////////////////////////////////////////////////////////////

// Use standard definitions of all basic methods
SO_SFIELD_SOURCE(SoSFImage, SbImage, const SbImage &);

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Initializes the SoSFImage class.
//
// Use: internal

void
SoSFImage::initClass()
//
////////////////////////////////////////////////////////////////////////
{
    SO__FIELD_INIT_CLASS(SoSFImage, "SFImage", SoSField);
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Sets value, given image dimensions and bytes...
//
// Use: public

void
SoSFImage::setValue(const SbVec2s &s, int nc, const unsigned char *b)
//
////////////////////////////////////////////////////////////////////////
{
    SbImage::Format fmt = SbImage::Format_Invalid;

    switch(nc) {
    case 1: fmt = SbImage::Format_Luminance; break;
    case 2: fmt = SbImage::Format_Luminance_Alpha; break;
    case 3: fmt = SbImage::Format_RGB24; break;
    case 4: fmt = SbImage::Format_RGBA32; break;
    default:
#ifdef DEBUG
    SoDebugError::postInfo("SoSFImage::setValue",
               "Unsupported number of components %d, should be 1,2,3 or 4",
               nc);
#endif
        break;
    }

    setValue(SbImage(s, fmt, s[0]*s[1]*nc, b));
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Gets value, which is the image's dimensions and bytes.
//
// Use: public

const unsigned char *
SoSFImage::getValue(SbVec2s &s, int &nc) const
//
////////////////////////////////////////////////////////////////////////
{
    s = SbVec2s(value.getSize().getValue());
    nc = value.getNumComponents();

    return value.getConstBytes();
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Edit the values of the image
//
// Use: public

unsigned char *
SoSFImage::startEditing(SbVec2s &s, int &nc)
//
////////////////////////////////////////////////////////////////////////
{
    s = SbVec2s(value.getSize().getValue());
    nc = value.getNumComponents();
    return value.getBytes();
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Must be called sometime after editValue to let node notify.
//
// Use: public

void
SoSFImage::finishEditing()
//
////////////////////////////////////////////////////////////////////////
{
    valueChanged();
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Reads value from file. Returns FALSE on error.
//
// Use: private

SbBool
SoSFImage::readValue(SoInput *in)
//
////////////////////////////////////////////////////////////////////////
{
    SbVec2s size;
    int numComponents;
    if (!in->read(size[0])  ||
        !in->read(size[1]) ||
        !in->read(numComponents))
        return FALSE;

    // Ignore empty images.
    if (size[0] == 0 || size[1] == 0 || numComponents == 0)
        return TRUE;

    SbImage::Format fmt = SbImage::Format_Invalid;

    switch(numComponents) {
    case 1: fmt = SbImage::Format_Luminance; break;
    case 2: fmt = SbImage::Format_Luminance_Alpha; break;
    case 3: fmt = SbImage::Format_RGB24; break;
    case 4: fmt = SbImage::Format_RGBA32; break;
    default:
#ifdef DEBUG
    SoDebugError::postInfo("SoSFImage::readValue",
               "Unsupported number of components %d, should be 1,2,3 or 4",
               numComponents);
#endif
    return FALSE;
    }

    SbImage image(size, fmt, size[0]*size[1]*numComponents, NULL);
    unsigned char *bytes = image.getBytes();

    int byte = 0;
    if (in->isBinary()) {
	// Inventor version 2.1 and later binary file
	if (in->getIVVersion() > 2.0) {
	    int numBytes = size[0]*size[1]*numComponents;
	    if (! in->readBinaryArray(bytes, numBytes))
		return FALSE;
	}

	// Pre version 2.1 Inventor binary files
	else {
	    for (int i = 0; i < size[0]*size[1]; i++) {
		uint32_t l;

		if (!in->read(l)) return FALSE;
		for (int j = 0; j < numComponents; j++) {
		    bytes[byte++] = (unsigned char)(
			(l >> (8*(numComponents-j-1))) & 0xFF);
		}
	    }
	}
    } else {
        for (int i = 0; i < size[0]*size[1]; i++) {
            uint32_t l;
    
	    if (!in->readHex(l)) return FALSE;
	    for (int j = 0; j < numComponents; j++) {
		bytes[byte++] = (unsigned char)(
		    (l >> (8*(numComponents-j-1))) & 0xFF);
	    }
	}
    }

    setValue(image);

    return TRUE;
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Writes value of field to file.
//
// Use: private

void
SoSFImage::writeValue(SoOutput *out) const
//
////////////////////////////////////////////////////////////////////////
{
    const SbVec3s & size = value.getSize();
    int numComponents = value.getNumComponents();
    const unsigned char * bytes = value.getConstBytes();

    out->write(size[0]);

    if (! out->isBinary())
	out->write(' ');

    out->write(size[1]);

    if (! out->isBinary())
	out->write(' ');

    out->write(numComponents);

    if (out->isBinary()) {
	int numBytes = size[0] * size[1] * numComponents;
        out->writeBinaryArray(bytes, numBytes);
    } else {
	int byte = 0;
	for (int i = 0; i < size[0]*size[1]; i++) {
	    uint32_t l = 0;
	    for (int j = 0; j < numComponents; j++) {
		l = (l<<8) | bytes[byte++];
	    }
	    out->write(' ');
	    out->write(l);
	}
    }
}
