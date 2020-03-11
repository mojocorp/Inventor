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
 | This file defines the SoGLDisplayList class.
 |
 |   Author(s)		: Gavin Bell
 |
 ______________  S I L I C O N   G R A P H I C S   I N C .  ____________
 _______________________________________________________________________
 */

#ifndef _SO_GL_DISPLAY_LIST
#define _SO_GL_DISPLAY_LIST

#include <Inventor/SbBasic.h>

class SoState;

////////////////////////////////////////////////////////////////////////
//
//  Class SoGLDisplayList:
//
//  A helper class used to store OpenGL display-list-like objects.
//  Currently, it can store either texture objects (which must be
//  treated like display lists; texture objects bound inside a display
//  list must be reference counted, etc) and display lists.
//
////////////////////////////////////////////////////////////////////////

SoEXTENDER
class SoGLDisplayList {

  public:
    //
    // Constructor.  Takes state, type, and number of lists to
    // allocate.  Calls either glGenLists() or glGenTexturesEXT() to
    // create empty objects/display lists.  If texture objects are not
    // supported, the type will revert to DISPLAY_LIST.
    //
    enum Type { DISPLAY_LIST, TEXTURE_OBJECT };
    SoGLDisplayList(SoState *state, Type type, int numToAllocate = 1);

    //
    // Because display lists may contain textures objects or other
    // display lists, they must be reference counted.
    // You can pass a NULL state to unref(); deletion of the display
    // list or texture object will then be delayed until a render
    // action is applied in the correct context.
    void ref();
    void unref(SoState *state = NULL);

    //
    // Open/close a display list.  Display lists are done in
    // COMPILE_AND_EXECUTE mode, so you don't need to call() the
    // display list after close().
    // Opening a texture object binds it; closing it does nothing.
    void open(SoState *state, int index = 0);
    void close(SoState *state);

    //
    // Call a display list, or bind a texture object.  This
    // automatically sets up a dependency if there is another display
    // list open in the state.
    // You can also use the get() methods below and make the OpenGL
    // calls yourself, in which case you should call the
    // addDependency() method to do the correct reference counting...
    void call(SoState *state, int index = 0);
    void addDependency(SoState *state);

    //
    // Get methods
    //
    Type     getType() const { return type; }
    int      getNumAllocated() const { return num; }
    uint32_t getFirstIndex() const { return startIndex; }
    int      getContext() const { return context; }

  private:
    ~SoGLDisplayList();

    Type     type;
    uint32_t startIndex;
    int      num;
    int      refCount;
    int      context;
    friend class SoGLCacheContextElement;
};

#endif /* _SO_GL_DISPLAY_LIST */
