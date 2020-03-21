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
 |	SoGLDepthBufferElement
 |
 ______________  S I L I C O N   G R A P H I C S   I N C .  ____________
 _______________________________________________________________________
 */

#include <glad/gl.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/elements/SoGLDepthBufferElement.h>
#include <Inventor/misc/SoState.h>

SO_ELEMENT_SOURCE(SoGLDepthBufferElement);

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Destructor.
//
// Use: private

SoGLDepthBufferElement::~SoGLDepthBufferElement()
//
////////////////////////////////////////////////////////////////////////
{}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Initializes SoGLDepthBufferElement class.
//
// Use: internal

void
SoGLDepthBufferElement::initClass() {
    SO_ELEMENT_INIT_CLASS(SoGLDepthBufferElement, SoDepthBufferElement);
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Initializes element.
//
// Use: public

void
SoGLDepthBufferElement::init(SoState *_state)
//
////////////////////////////////////////////////////////////////////////
{
    // Initialize base class stuff
    SoDepthBufferElement::init(_state);
    whatChanged = 0;
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Pushes the element, copying values from previous element in
//    state:
//
// Use: public, virtual

void
SoGLDepthBufferElement::push(SoState *)
//
////////////////////////////////////////////////////////////////////////
{
    const SoGLDepthBufferElement *prevElt =
        (const SoGLDepthBufferElement *)getNextInStack();

    test = prevElt->test;
    write = prevElt->write;
    function = prevElt->function;
    range = prevElt->range;
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Pops element, causing side effects in GL.
//
// Use: public

void
SoGLDepthBufferElement::pop(SoState *state, const SoElement *childElt)
//
////////////////////////////////////////////////////////////////////////
{
    // Since popping this element has GL side effects, make sure any
    // open caches capture it.  We may not send any GL commands, but
    // the cache dependency must exist even if we don't send any GL
    // commands, because if the element changes, the _lack_ of GL
    // commands here is a bug (remember, GL commands issued here are
    // put inside the cache).
    capture(state);

    // If the previous element didn't have the same value...
    const SoGLDepthBufferElement *child =
        (const SoGLDepthBufferElement *)childElt;

    // Restore previous values
    if (test != child->test)
        whatChanged |= TEST_MASK;
    if (write != child->write)
        whatChanged |= WRITE_MASK;
    if (function != child->function)
        whatChanged |= FUNCT_MASK;
    if (range != child->range)
        whatChanged |= RANGE_MASK;

    if (whatChanged) {
        send();
    }
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Sets depth params in element.
//
// Use: protected, virtual

void
SoGLDepthBufferElement::setElt(SbBool _test, SbBool _write,
                               DepthWriteFunction _function,
                               const SbVec2f &    _range)
//
////////////////////////////////////////////////////////////////////////
{
    whatChanged = 0;

    if (test != _test)
        whatChanged |= TEST_MASK;
    if (write != _write)
        whatChanged |= WRITE_MASK;
    if (function != _function)
        whatChanged |= FUNCT_MASK;
    if (range != _range)
        whatChanged |= RANGE_MASK;

    if (whatChanged) {
        SoDepthBufferElement::setElt(_test, _write, _function, _range);

        send();
    }
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Sends depth params in element to GL.
//
// Use: private

void
SoGLDepthBufferElement::send()
//
////////////////////////////////////////////////////////////////////////
{
    if (whatChanged & TEST_MASK) {
        if (test) {
            glEnable(GL_DEPTH_TEST);
        } else {
            glDisable(GL_DEPTH_TEST);
        }
    }

    if (whatChanged & WRITE_MASK) {
        glDepthMask(write ? GL_TRUE : GL_FALSE);
    }

    if (whatChanged & FUNCT_MASK) {
        switch (function) {
        case NEVER:
            glDepthFunc(GL_NEVER);
            break;
        case ALWAYS:
            glDepthFunc(GL_ALWAYS);
            break;
        case LESS:
            glDepthFunc(GL_LESS);
            break;
        case LEQUAL:
            glDepthFunc(GL_LEQUAL);
            break;
        case EQUAL:
            glDepthFunc(GL_EQUAL);
            break;
        case GEQUAL:
            glDepthFunc(GL_GEQUAL);
            break;
        case GREATER:
            glDepthFunc(GL_GREATER);
            break;
        case NOTEQUAL:
            glDepthFunc(GL_NOTEQUAL);
            break;
        default: // Just to shut CC up
            break;
        }
    }

    if (whatChanged & RANGE_MASK) {
        glDepthRange(range[0], range[1]);
    }
}
