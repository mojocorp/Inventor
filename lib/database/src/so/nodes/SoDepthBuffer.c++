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
 |      SoDepthBuffer
 |
 ______________  S I L I C O N   G R A P H I C S   I N C .  ____________
 _______________________________________________________________________
 */

#include <Inventor/nodes/SoDepthBuffer.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/elements/SoGLDepthBufferElement.h>

SO_NODE_SOURCE(SoDepthBuffer)

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Constructor
//
// Use: public

SoDepthBuffer::SoDepthBuffer()
//
////////////////////////////////////////////////////////////////////////
{
    SO_NODE_CONSTRUCTOR(SoDepthBuffer);

    SO_NODE_ADD_FIELD(test, (SoDepthBufferElement::getDefaultTest()));
    SO_NODE_ADD_FIELD(write, (SoDepthBufferElement::getDefaultWrite()));
    SO_NODE_ADD_FIELD(function, (SoDepthBufferElement::getDefaultFunction()));
    SO_NODE_ADD_FIELD(range, (SoDepthBufferElement::getDefaultRange()));

    //
    // Set up static info for enumerated type fields
    //
    SO_NODE_DEFINE_ENUM_VALUE(DepthWriteFunction, NEVER);
    SO_NODE_DEFINE_ENUM_VALUE(DepthWriteFunction, ALWAYS);
    SO_NODE_DEFINE_ENUM_VALUE(DepthWriteFunction, LESS);
    SO_NODE_DEFINE_ENUM_VALUE(DepthWriteFunction, LEQUAL);
    SO_NODE_DEFINE_ENUM_VALUE(DepthWriteFunction, EQUAL);
    SO_NODE_DEFINE_ENUM_VALUE(DepthWriteFunction, GEQUAL);
    SO_NODE_DEFINE_ENUM_VALUE(DepthWriteFunction, GREATER);
    SO_NODE_DEFINE_ENUM_VALUE(DepthWriteFunction, NOTEQUAL);

    SO_NODE_SET_SF_ENUM_TYPE(function, DepthWriteFunction);

    isBuiltIn = TRUE;
}

////////////////////////////////////////////////////////////////////////
//
// Destructor
//

SoDepthBuffer::~SoDepthBuffer()
//
////////////////////////////////////////////////////////////////////////
{}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    This initializes the SoDepthBuffer class.
//
// Use: internal

void
SoDepthBuffer::initClass()
//
////////////////////////////////////////////////////////////////////////
{
    SO__NODE_INIT_CLASS(SoDepthBuffer, "DepthBuffer", SoNode);

    SO_ENABLE(SoGLRenderAction, SoGLDepthBufferElement);
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Traversal for GL rendering
//
// Use: extender

void
SoDepthBuffer::GLRender(SoGLRenderAction *action)
//
////////////////////////////////////////////////////////////////////////
{
    SoState *state = action->getState();

    SbBool                                   curtest;
    SbBool                                   curwrite;
    SoDepthBufferElement::DepthWriteFunction curfunc;
    SbVec2f                                  currange;

    SoDepthBufferElement::get(state, curtest, curwrite, curfunc, currange);

    if (!test.isIgnored())
        curtest = test.getValue();
    if (!write.isIgnored())
        curwrite = write.getValue();
    if (!function.isIgnored())
        curfunc = (SoDepthBufferElement::DepthWriteFunction)function.getValue();
    if (!range.isIgnored())
        currange = range.getValue();

    SoDepthBufferElement::set(state, curtest, curwrite, curfunc, currange);
}
