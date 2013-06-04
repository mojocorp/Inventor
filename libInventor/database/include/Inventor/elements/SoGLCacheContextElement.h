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
 | This file defines the SoGLCacheContextElement class.
 |
 |   Author(s)  : Gavin Bell
 |
 ______________  S I L I C O N   G R A P H I C S   I N C .  ____________
 _______________________________________________________________________
 */

#ifndef  _SO_CACHE_CONTEXT_ELEMENT
#define  _SO_CACHE_CONTEXT_ELEMENT

#include <Inventor/elements/SoSubElement.h>

#include <list>

//////////////////////////////////////////////////////////////////////////////
//
//  Class: SoCacheContextElement
//
//  Element that stores the cache context.  There is a method on the
//  render action to set this; if you are not rendering onto multiple
//  displays, you will not need to set this (assuming that caches are
//  shareable between different windows on the same display, which is
//  true of GLX).
//
//  This element should be set before traversal starts, and must not
//  be changed during traversal (you'll get a debug error if a node
//  tries to set the cache context during traversal).
//
//////////////////////////////////////////////////////////////////////////////

class SoGLDisplayList;

SoEXTENDER class INVENTOR_API SoGLCacheContextElement : public SoElement {

    SO_ELEMENT_HEADER(SoGLCacheContextElement);

public:
    // Initializes element
    virtual void init(SoState *state);

    // Sets the current context.  This is done by the renderAction;
    // theoretically, a node that redirected the GL rendering context
    // to another X server would also set this.  This also frees up
    // any display lists that are waiting for the context to become
    // valid. The third argument indicates whether 2-pass
    // transparency is in effect (as for DELAYED or SORTED blending),
    // which means that caches containing transparent objects may not
    // be valid.  The fourth argument should be TRUE if remote
    // rendering is being done; the SoSeparator auto-caching algorithm
    // uses this information to help decide whether or not to build
    // caches.
    static void set(SoState *state, int context,
                    SbBool is2PassTransparency,
                    SbBool isRemoteRendering);

    // Gets the current context
    static int get(SoState *state);

    // Methods used to improve auto-caching algorithm:

    // Two bits are stored.  Nodes that should be cached will set the
    // DO_AUTO_CACHE bit, nodes that should NOT be cached will set the
    // DONT_AUTO_CACHE bit.  By default, DO_AUTO_CACHE is FALSE unless
    // remote rendering is being done.  DONT_AUTO_CACHE is FALSE by
    // default.  Separators will auto cache if DO_AUTO_CACHE is TRUE
    // and DONT_AUTO_CACHE is FALSE, otherwise they won't auto-cache.
    enum AutoCache {
        DO_AUTO_CACHE = 1,   // Hack warning: I rely on TRUE==DO_AUTO_CACHE
        DONT_AUTO_CACHE = 2
    };

    // Called by nodes to say that they should/shouldn't be
    // auto-cached (pass TRUE if should, FALSE if shouldn't, don't
    // call this method at all if the node doesn't care):
    static void shouldAutoCache(SoState *state, AutoCache bits) {
        SoGLCacheContextElement *elt = (SoGLCacheContextElement *)state->getElementNoPush(classStackIndex);
        elt->autoCacheBits |= (int)bits;
    }

    // Used by Separators to set/reset the auto-caching bits:
    static void setAutoCacheBits(SoState *state, int bits) {
        ((SoGLCacheContextElement *)state->getElementNoPush(classStackIndex))->autoCacheBits = bits;
    }

    static int resetAutoCacheBits(SoState *state) {
        SoGLCacheContextElement *elt = (SoGLCacheContextElement *)state->getElementNoPush(classStackIndex);
        int result = elt->autoCacheBits;
        // Hack warning: I rely on TRUE==DO_AUTO_CACHE
        elt->autoCacheBits = elt->isRemoteRendering;

        return result;
    }

    static SbBool getIsRemoteRendering(SoState *state) {
        const SoGLCacheContextElement *elt = (const SoGLCacheContextElement *)state->getConstElement(classStackIndex);
        return elt->isRemoteRendering;
    }

    // Standard element stuff:

    // Returns TRUE if the widths match in both elements
    virtual SbBool matches(const SoElement *elt) const;

    // Create and return a copy of this element
    virtual SoElement *copyMatchInfo() const;

    // Prints element (for debugging)
    virtual void print(FILE *fp) const;

    // Returns an unique cache context id.
    static int getUniqueCacheContext();

SoINTERNAL public:
    static void initClass();

protected:
    int      context;
    SbBool   is2PassTransp;
    SbBool   isRemoteRendering;
    int      autoCacheBits;

    virtual ~SoGLCacheContextElement();

private:
    // Frees up the given display lists right away, if possible.  If
    // not possible (because the state passed in is NULL or has a
    // different cache context than the display lists' state), this
    // adds the given display list/count to the list of display lists
    // that need to be freed the next time the given context is valid.
    // This method is necessary because nodes with caches can be
    // deleted at any time, but we can't necessarily send GL commands
    // to free up a display list at any time.
    static void freeList(SoState *state, SoGLDisplayList *list);

    static std::list<SoGLDisplayList*> waitingToBeFreed;

    friend class SoGLDisplayList;
};

#endif /* _SO_CACHE_CONTEXT_ELEMENT */
