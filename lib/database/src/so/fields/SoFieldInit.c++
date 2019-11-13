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
 * Copyright (C) 1990,91,92   Silicon Graphics, Inc.
 *
 _______________________________________________________________________
 ______________  S I L I C O N   G R A P H I C S   I N C .  ____________
 |
 |   $Revision: 1.1 $
 |
 |   Class:
 |	call initClasses for all field classes
 |
 |   Author(s)		: Ronen Barzel
 |
 ______________  S I L I C O N   G R A P H I C S   I N C .  ____________
 _______________________________________________________________________
 */

#include <Inventor/fields/SoFields.h>

////////////////////////////////////////////////////////////////////////
//
// Description:
//    This initializes ALL Inventor field classes.
//
// Use: internal

void
SoField::initClasses()
//
////////////////////////////////////////////////////////////////////////
{
    SoField::initClass();
    SoSField::initClass();
    SoMField::initClass();

    SoSFBool::initClass();
    SoSFColor::initClass();
    SoSFEngine::initClass();
    SoSFEnum::initClass();
    SoSFBitMask::initClass(); // Note: derived from SoSFEnum!
    SoSFFloat::initClass();
    SoSFImage::initClass();
    SoSFInt32::initClass();
    SoSFMatrix::initClass();
    SoSFName::initClass();
    SoSFNode::initClass();
    SoSFPath::initClass();
    SoSFPlane::initClass();
    SoSFRotation::initClass();
    SoSFShort::initClass();
    SoSFString::initClass();
    SoSFTime::initClass();
    SoSFTrigger::initClass();
    SoSFUInt32::initClass();
    SoSFUShort::initClass();
    SoSFVec2f::initClass();
    SoSFVec3f::initClass();
    SoSFVec4f::initClass();

    SoMFBool::initClass();
    SoMFColor::initClass();
    SoMFEngine::initClass();
    SoMFEnum::initClass();
    SoMFBitMask::initClass(); // Note: derived from SoMFEnum!
    SoMFFloat::initClass();
    SoMFInt32::initClass();
    SoMFMatrix::initClass();
    SoMFName::initClass();
    SoMFNode::initClass();
    SoMFPath::initClass();
    SoMFPlane::initClass();
    SoMFRotation::initClass();
    SoMFShort::initClass();
    SoMFString::initClass();
    SoMFTime::initClass();
    SoMFUInt32::initClass();
    SoMFUShort::initClass();
    SoMFVec2f::initClass();
    SoMFVec3f::initClass();
    SoMFVec4f::initClass();
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Clean-up.
// Use: public, static

void
SoField::finishClasses()
//
////////////////////////////////////////////////////////////////////////
{}
