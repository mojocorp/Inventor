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
 |	SoGLTextureUnitElement
 |
 |   Author(s)		: Morgan Leborgne
 |
 ______________  S I L I C O N   G R A P H I C S   I N C .  ____________
 _______________________________________________________________________
 */

#include <Inventor/misc/SoGL.h>
#include <Inventor/elements/SoGLTextureUnitElement.h>
#include <Inventor/actions/SoGLRenderAction.h>

SO_ELEMENT_SOURCE(SoGLTextureUnitElement);

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Initializes SoGLTextureUnitElement class.
//
// Use: internal

void
SoGLTextureUnitElement::initClass() {
    SO_ELEMENT_INIT_CLASS(SoGLTextureUnitElement, SoTextureUnitElement);
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Destructor.
//
// Use: private

SoGLTextureUnitElement::~SoGLTextureUnitElement()
//
////////////////////////////////////////////////////////////////////////
{}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Initializes element
//
// Use: public

void
SoGLTextureUnitElement::init(SoState *state)
//
////////////////////////////////////////////////////////////////////////
{
    SoTextureUnitElement::init(state);

    copiedFromParent = NULL;
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Pushes element, copying data from previous element
//
// Use: public

void
SoGLTextureUnitElement::push(SoState *state)
//
////////////////////////////////////////////////////////////////////////
{
    const SoGLTextureUnitElement *prevElt =
        (const SoGLTextureUnitElement *)getNextInStack();

    data = prevElt->data;

    copiedFromParent = state;
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Pops element, causing side effects in GL.
//
// Use: public

void
SoGLTextureUnitElement::pop(SoState *state, const SoElement *childElt)
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
    copiedFromParent = NULL;

    // If the previous element didn't have the same value...
    const SoGLTextureUnitElement *child =
        (const SoGLTextureUnitElement *)childElt;

    // Restore previous texture unit
    if (data != child->data)
        send();
}

///////////////////////////////////////////////////////////////////////
//
// Description:
//    Sets texture unit in element.
//
// Use: protected, virtual

void
SoGLTextureUnitElement::setElt(int32_t unit)
//
////////////////////////////////////////////////////////////////////////
{
    // Optimization:  on push, we copy the value from the previous
    // element.  If the element is set to the same value, we don't
    // bother sending it, but do add a cache dependency on the
    // previous element.

    if (data != unit) {
        data = unit;
        send();
        copiedFromParent = NULL;
    } else if (copiedFromParent) {
        SoGLTextureUnitElement *parent =
            (SoGLTextureUnitElement *)getNextInStack();
        parent->capture(copiedFromParent);
    }
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Sends texture unit in element to GL.
//
// Use: private

void
SoGLTextureUnitElement::send()
//
////////////////////////////////////////////////////////////////////////
{
    glActiveTextureARB((unsigned int)GL_TEXTURE0 + data);
}
