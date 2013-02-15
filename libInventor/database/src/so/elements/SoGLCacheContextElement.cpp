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
 |   $Revision: 1.2 $
 |
 |   Classes:
 |	SoGLCacheContextElement
 |
 |   Author(s)		: Gavin Bell
 |
 ______________  S I L I C O N   G R A P H I C S   I N C .  ____________
 _______________________________________________________________________
 */

#include <GL/glew.h>
#include <Inventor/elements/SoGLCacheContextElement.h>
#include <Inventor/caches/SoGLDisplayList.h>
#include <Inventor/errors/SoDebugError.h>
#include <Inventor/misc/SoState.h>
#include <Inventor/SbString.h>
#include <Inventor/lists/SbIntList.h>

#include <map>

SO_ELEMENT_SOURCE(SoGLCacheContextElement);

SbPList		*SoGLCacheContextElement::waitingToBeFreed = NULL;

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Destructor.
//
// Use: private

SoGLCacheContextElement::~SoGLCacheContextElement()
//
////////////////////////////////////////////////////////////////////////
{
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Initializes SoGLCacheContextElement class.
//
// Use: internal

void
SoGLCacheContextElement::initClass()
{
    SO_ELEMENT_INIT_CLASS(SoGLCacheContextElement, SoElement);
    waitingToBeFreed = new SbPList;
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Initializes element
//
// Use: public

void
SoGLCacheContextElement::init(SoState *)
//
////////////////////////////////////////////////////////////////////////
{
    context = 0;
    is2PassTransp = FALSE;
    autoCacheBits = 0;
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Sets cache context
//
// Use: public, static

void
SoGLCacheContextElement::set(SoState *state, int ctx,
                             SbBool is2PassTransparency,
                             SbBool remoteRender)
//
////////////////////////////////////////////////////////////////////////
{
#ifdef DEBUG
    if (state->getDepth() != 1) {
        SoDebugError::post("SoGLCacheContextElement::set",
                           "must not be set during traversal");
    }
#endif

    SoGLCacheContextElement *elt = (SoGLCacheContextElement *)state->getElementNoPush(classStackIndex);

    elt->context = ctx;
    elt->is2PassTransp = is2PassTransparency;
    elt->isRemoteRendering = remoteRender;
    elt->autoCacheBits = remoteRender ? DO_AUTO_CACHE : 0;

    // Look through the list of display lists waiting to be freed, and
    // free any that match the context:
    for (int i = waitingToBeFreed->getLength()-1; i >= 0; i--) {
        SoGLDisplayList *dl = (SoGLDisplayList *)(*waitingToBeFreed)[i];
        if (dl->getContext() == ctx) {
            waitingToBeFreed->remove(i);
            delete dl;
        }
    }
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Gets the cache context.  GL render caches do this when they are
//    opened (they need the context to free themselves properly).
//
// Use: public, static

int
SoGLCacheContextElement::get(SoState *state)
//
////////////////////////////////////////////////////////////////////////
{
    const SoGLCacheContextElement *elt = (const SoGLCacheContextElement *)getConstElement(state, classStackIndex);

    return elt->context;
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Frees up the given display lists right away, if possible.  If
//    not possible (because the state passed in is NULL or has a
//    different cache context than the display lists' state), this
//    adds the given display list/count to the list of display lists
//    that need to be freed the next time the given context is valid.
//    This method is necessary because nodes with caches can be
//    deleted at any time, but we can't necessarily send GL commands
//    to free up a display list at any time.
//
// Use: public, static

void
SoGLCacheContextElement::freeList(SoState *state,
                                  SoGLDisplayList *dl)
//
////////////////////////////////////////////////////////////////////////
{
    if (state != NULL  &&  get(state) == dl->getContext()) {
        delete dl;
    } else {
        waitingToBeFreed->append(dl);
    }
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Given an extension string, return TRUE if that extension is
//    supported in this rendering context.
//
SbBool
SoGLCacheContextElement::extSupported(SoState *state, const char *str)
//
////////////////////////////////////////////////////////////////////////
{
    SoGLCacheContextElement *elt = (SoGLCacheContextElement *)state->getElementNoPush(classStackIndex);
#ifdef DEBUG
    if (elt == NULL) {
	SoDebugError::post("SoGLCacheContextElement::extSupported",
               "No GL cache context found; "
               "This method require a valid OpenGL context");
    }
#endif

    static std::map<int, GLEWContext> s_glewctx;

#ifdef GLEW_MX
#  define glewGetContext() (&s_glewctx[elt->context])
#endif

    if (s_glewctx.find(elt->context) == s_glewctx.end()) {
        GLenum err = glewInit();
        if (GLEW_OK != err)
        {
            /* Problem: glewInit failed, something is seriously wrong. */
#ifndef DEBUG
            SoDebugError::post("SoGLCacheContextElement:", "Error: %s", glewGetErrorString(err));
#endif
        }
    }

    return glewIsSupported(str);
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Overrides this method to compare contexts.
//
// Use: public

SbBool
SoGLCacheContextElement::matches(const SoElement *elt) const
//
////////////////////////////////////////////////////////////////////////
{
    
    const SoGLCacheContextElement *cacheElt = (const SoGLCacheContextElement *) elt;

    return (context       == cacheElt->context &&
            is2PassTransp == cacheElt->is2PassTransp);
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//     Create a copy of this instance suitable for calling matches()
//     on.
//
// Use: protected

SoElement *
SoGLCacheContextElement::copyMatchInfo() const
//
////////////////////////////////////////////////////////////////////////
{
    SoGLCacheContextElement *result =
            (SoGLCacheContextElement *)getTypeId().createInstance();

    result->context = context;
    result->is2PassTransp = is2PassTransp;

    return result;
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Prints element for debugging.
//
// Use: public
//
////////////////////////////////////////////////////////////////////////

#ifdef DEBUG
void
SoGLCacheContextElement::print(FILE *fp) const
{
    SoElement::print(fp);

    fprintf(fp, "\tCache context = %uL, is2PassTransp = %s\n",
            context, is2PassTransp ? "TRUE" : "FALSE");
}
#else  /* DEBUG */
void
SoGLCacheContextElement::print(FILE *) const
{
}
#endif /* DEBUG */

int
SoGLCacheContextElement::getUniqueCacheContext()
{
    static int s_contextIncrement = 1000;
    
    return ++s_contextIncrement;
}
