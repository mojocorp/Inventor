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
 |   $Revision: 1.2 $
 |
 |   Description:
 |	This file defines the SoPackedColor node class.
 |
 |   Author(s)		: Paul S. Strauss
 |
 ______________  S I L I C O N   G R A P H I C S   I N C .  ____________
 _______________________________________________________________________
 */

#ifndef  _SO_PACKED_COLOR_
#define  _SO_PACKED_COLOR_

#include <Inventor/fields/SoMFUInt32.h>
#include <Inventor/nodes/SoSubNode.h>

/// Node that defines base colors using packed representation.
/// \ingroup Nodes
/// <tt>SoPackedColor</tt> is similar to <tt>SoBaseColor</tt> in that it sets the
/// diffuse color component of the current material. However, it also
/// changes the transparency component. The color and transparency
/// information is packed into unsigned 32-bit integers: <b>0xrrggbbaa</b>,
/// where <b>aa</b> represents the alpha (<b>0x00</b> = fully transparent,
/// <b>0xff</b> = opaque), and <b>rr</b>, <b>gg</b>, and <b>bb</b> represent the
/// red, blue, and green components of the color, respectively.
/// Note that the order (r,g,b,a) of these components is reversed from
/// the ordering in releases of Inventor prior to 2.1.
///
/// If the
/// transparency type is <tt>SoGLRenderAction::SCREEN_DOOR</tt>, only the
/// first transparency value will be used.  With other transparency types,
/// multiple transparencies will be used.
///
/// <tt>SoPackedColor</tt> uses less memory than <tt>SoBaseColor</tt> or
/// <tt>SoMaterial</tt>  to store multiple color and transparency values. It
/// can be used in cases where space is critical.
///
/// \par Action behavior:
/// <b>SoGLRenderAction, SoCallbackAction</b>
/// Sets the current base (diffuse) color(s) in the state.
///
/// \par File format/defaults:
/// \code
/// SoPackedColor {
///    orderedRGBA	0xccccccff
/// }
/// \endcode
/// \sa SoBaseColor, SoMaterial
class INVENTOR_API SoPackedColor : public SoNode {

    SO_NODE_HEADER(SoPackedColor);

  public:
    // Fields
    SoMFUInt32		orderedRGBA;	///< Defines the packed colors.

    /// Creates a packed color node with default settings.
    SoPackedColor();

  SoEXTENDER public:
    virtual void	doAction(SoAction *action);
    virtual void	GLRender(SoGLRenderAction *action);
    virtual void	callback(SoCallbackAction *action);
    SbBool		isTransparent() const
	{return transparent;}

  SoINTERNAL public:
    static void		initClass();

  protected:
    virtual ~SoPackedColor();
    
  private:
    // check for transparency when field changes.
    virtual void	notify(SoNotList *list);
    // store whether transparent or not
    SbBool transparent;
    
};

#endif /* _SO_PACKED_COLOR_ */
