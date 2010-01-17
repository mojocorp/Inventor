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
 |	SoGLDisplayList
 |
 |   Author(s)		: Gavin Bell
 |
 ______________  S I L I C O N   G R A P H I C S   I N C .  ____________
 _______________________________________________________________________
 */

#include <Inventor/elements/SoGLDisplayList.h>
#include <Inventor/elements/SoGLCacheContextElement.h>
#include <Inventor/caches/SoGLRenderCache.h>
#include <Inventor/elements/SoCacheElement.h>

#include <stdlib.h>

int SoGLDisplayList::texture_object_extensionID = -1;

//
// Workaround for nasty OpenGL bug:  we don't use the texture object
// extension if the user sets the IV_NO_TEXTURE_OBJECT environment
// variable, because texture objects are not correctly shared between
// contexts on RE and impact so multi-windowed applications will
// display textures in only one window.
// -- gavin 10/3/95
//
static int never_use_texture_object = -1;  // "don't know" initial value

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Constructor
//
// Use: public

SoGLDisplayList::SoGLDisplayList(SoState *state, Type _type,
				 int numToAllocate)
//
////////////////////////////////////////////////////////////////////////
{
    refCount = 0;

    num = numToAllocate;

    // We must depend on the GL cache context; we can't assume that a
    // cache is valid between any two render actions, since the render
    // actions could be directed at different X servers on different
    // machines (with different ideas about which display lists have
    // been created).
    context = SoGLCacheContextElement::get(state);

#ifdef GL_VERSION_1_1
    int texObjSupported = TRUE;

    if (never_use_texture_object == -1) {
	if (getenv("IV_NO_TEXTURE_OBJECT"))
	    never_use_texture_object = 1;
	else
	    never_use_texture_object = 0;
    }	
    if (never_use_texture_object) texObjSupported = 0;

#else
    int texObjSupported = FALSE;
#endif

    // Always use display lists if texture objects are not supported:
    if (_type == TEXTURE_OBJECT && !texObjSupported)
	type = DISPLAY_LIST;
    else
	type = _type;

    if (type == TEXTURE_OBJECT) {
	glGenTextures(1, &startIndex);
#ifdef DEBUG
	if (num != 1)
	    SoDebugError::post("SoGLDisplayList", "Sorry, can only "
			       "construct 1 texture object at a time");
#endif
    } else {
	startIndex = glGenLists(num);
    }
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//
//
// Use: public

void
SoGLDisplayList::ref()
//
////////////////////////////////////////////////////////////////////////
{
    ++refCount;
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//
//
// Use: public

void
SoGLDisplayList::unref(SoState *state)
//
////////////////////////////////////////////////////////////////////////
{
    --refCount;
    if (refCount <= 0) {
	// Let the CacheContextElement delete us:
	SoGLCacheContextElement::freeList(state, this);
    }
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//
//
// Use: public

void
SoGLDisplayList::open(SoState *, int index)
//
////////////////////////////////////////////////////////////////////////
{
    if (type == TEXTURE_OBJECT) {
	glBindTexture(GL_TEXTURE_2D, startIndex+index);
    } else {
	glNewList(startIndex+index, GL_COMPILE_AND_EXECUTE);
    }
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//
//
// Use: public

void
SoGLDisplayList::close(SoState *)
//
////////////////////////////////////////////////////////////////////////
{
    if (type != TEXTURE_OBJECT) {
	glEndList();
    }
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//
//
// Use: public

void
SoGLDisplayList::call(SoState *state, int index)
//
////////////////////////////////////////////////////////////////////////
{
    if (type == TEXTURE_OBJECT) {
	glBindTexture(GL_TEXTURE_2D, startIndex+index);
    } else {
	glCallList(startIndex+index);
    }
    addDependency(state);
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//
//
// Use: public

void
SoGLDisplayList::addDependency(SoState *state)
//
////////////////////////////////////////////////////////////////////////
{
    if (state->isCacheOpen()) {
	SoGLRenderCache *c = (SoGLRenderCache *)
	    SoCacheElement::getCurrentCache(state);
	c->addNestedCache(this);
    }
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//
//
// Use: public

SoGLDisplayList::~SoGLDisplayList()
//
////////////////////////////////////////////////////////////////////////
{
    if (type == TEXTURE_OBJECT) {
	glDeleteTextures(1, &startIndex);
    } else {
	glDeleteLists(startIndex, num);
    }
}

