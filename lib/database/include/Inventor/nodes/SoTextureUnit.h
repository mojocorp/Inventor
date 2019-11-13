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
 |	This file defines the SoTextureUnit node class.
 |
 |   Author(s)		: Morgan Leborgne
 |
 ______________  S I L I C O N   G R A P H I C S   I N C .  ____________
 _______________________________________________________________________
 */

#ifndef _SO_TEXTURE_UNIT_H_
#define _SO_TEXTURE_UNIT_H_

#include <Inventor/nodes/SoSubNode.h>
#include <Inventor/fields/SoSFInt32.h>

class SoState;

class SoTextureUnit : public SoNode {

    SO_NODE_HEADER(SoTextureUnit);

public:
    // Fields:
    //
    SoSFInt32 unit;

    /**
     * Creates a texture unit node with default settings.
     */
    SoTextureUnit();

    /**
     * Returns the maximum supported texture image units for the current context.
     * Units can be used to access texture maps from the fragment shader. 
     * The value must be at least 2.
     */
    static uint32_t getMaxTextureUnit(SoState *state);

SoINTERNAL public:
    // Initializes this class for use in scene graphs. This
    // should be called after database initialization and before
    // any instance of this node is constructed.
    static void    initClass();

SoEXTENDER public:
    // Implement render action
    virtual void        doAction(SoAction *action);
    virtual void        callback(SoCallbackAction *action);
    virtual void        GLRender(SoGLRenderAction *action);
    virtual void        pick(SoPickAction *action);

protected:
    // Destructor. Protected to keep people from trying to delete
    // nodes, rather than using the reference count mechanism.
    virtual ~SoTextureUnit();

private:
};

#endif /* _SO_TEXTURE_UNIT_H_ */
