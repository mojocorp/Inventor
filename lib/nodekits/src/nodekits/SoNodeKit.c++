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
 * Copyright (C) 1990, 1991   Silicon Graphics, Inc.
 *
 _______________________________________________________________________
 ______________  S I L I C O N   G R A P H I C S   I N C .  ____________
 |
 |   $Revision: 1.1 $
 |
 |   Description:
 |      Definition of the SoNodeKit class
 |
 |   Author(s)          : Paul Isaacs
 |
 ______________  S I L I C O N   G R A P H I C S   I N C .  ____________
 _______________________________________________________________________
 */

#include <Inventor/SoDB.h>
#include <Inventor/nodekits/SoNodeKitListPart.h>
#include <Inventor/nodekits/SoNodeKit.h>
#include <Inventor/nodekits/SoAppearanceKit.h>
#include <Inventor/nodekits/SoBaseKit.h>
#include <Inventor/nodekits/SoCameraKit.h>
#include <Inventor/nodekits/SoSeparatorKit.h>
#include <Inventor/nodekits/SoLightKit.h>
#include <Inventor/nodekits/SoSceneKit.h>
#include <Inventor/nodekits/SoShapeKit.h>
#include <Inventor/nodekits/SoWrapperKit.h>
#include <Inventor/details/SoNodeKitDetail.h>
#include <Inventor/misc/upgraders/SoV1NodeKit.h>

SbBool SoNodeKit::initialized = FALSE;

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Initialize all node kit class.
//
// Use: public

void
SoNodeKit::init()
//
////////////////////////////////////////////////////////////////////////
{
    // Only initialize once
    if (!initialized) {

        // Call SoDB::init();
        // Okay if it's already been called.
        SoDB::init();

        //
        // initialize the nodekit catalog class
        //
        SoNodekitCatalog::initClass();

        //
        // initialize all the node classes
        //

        // base class
        SoNodeKitListPart::initClass();

        // base class
        SoBaseKit::initClass();

        SoAppearanceKit::initClass();
        SoSeparatorKit::initClass();
        SoWrapperKit::initClass();
        SoShapeKit::initClass();
        SoLightKit::initClass();
        SoCameraKit::initClass();
        SoSceneKit::initClass();

        // The detail class -- Not a node class, but a detail used by them.
        SoNodeKitDetail::initClass();

        // Initialize the V1.0 to V2.0 upgrader classes
        SoV1NodeKit::init();

        initialized = TRUE;
    }
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Clean-up all node kit class.
//
// Use: public

void
SoNodeKit::finish()
//
////////////////////////////////////////////////////////////////////////
{
    // Only initialize once
    if (initialized) {
        SoV1NodeKit::finish();
        SoNodeKitDetail::finishClass();
        SoSceneKit::finishClass();
        SoCameraKit::finishClass();
        SoLightKit::finishClass();
        SoShapeKit::finishClass();
        SoWrapperKit::finishClass();
        SoSeparatorKit::finishClass();
        SoAppearanceKit::finishClass();
        SoBaseKit::finishClass();

        SoNodeKitListPart::finishClass();
        SoNodekitCatalog::finishClass();

        SoDB::finish();

        initialized = FALSE;
    }
}
