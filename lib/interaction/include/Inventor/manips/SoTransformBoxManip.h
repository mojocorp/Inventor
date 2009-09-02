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
 * Copyright (C) 1990,91,92   Silicon Graphics, Inc.
 *
 _______________________________________________________________________
 ______________  S I L I C O N   G R A P H I C S   I N C .  ____________
 |
 |   $Revision: 1.1 $
 |
 |   Author(s): Paul Isaacs
 |
 ______________  S I L I C O N   G R A P H I C S   I N C .  ____________
 _______________________________________________________________________
 */

#ifndef  _SO_TRANSFORM_BOX_MANIP_
#define  _SO_TRANSFORM_BOX_MANIP_

#include <Inventor/manips/SoTransformManip.h>

/// Transform node with 3D interface for changing scaling, rotation, and translation.
/// \ingroup Manips
/// <tt>SoTransformBoxManip</tt>
/// is derived from <tt>SoTransform</tt> (by way of <tt>SoTransformManip</tt>).
/// When its fields
/// change, nodes following it in the scene graph rotate, scale, and/or translate.
///
/// As a subclass of <tt>SoTransformManip</tt>, this manipulator
/// also has a 3D interface to edit some of its fields.
/// In this case, the interface edits the #scaleFactor, #rotation
/// and #translation fields.
///
/// A manipulator differs from a dragger. When you move a dragger,
/// no other nodes are affected.  When you move an <tt>SoTransformManip</tt>,
/// other nodes move along with it.
/// (See the reference page for <tt>SoTransformManip</tt>).
///
/// The interface for an <tt>SoTransformBoxManip</tt> is exactly the same as that
/// of the <tt>SoTransformBoxDragger</tt>.
/// To find out more about the interface, see the reference page
/// for <tt>SoTransformBoxDragger</tt>.  To find out how the manipulator uses a
/// dragger to provide its interface, see the reference page for
/// <tt>SoTransformManip</tt>.
///
/// On screen, this manipulator will surround the objects influenced by its
/// motion.  This is because it turns on the <em>surroundScale</em> part of the
/// dragger (See the reference page for <tt>SoSurroundScale</tt>)
///
/// \par File format/defaults:
/// \code
/// SoTransformBoxManip {
///    translation      0 0 0
///    rotation         0 0 1  0
///    scaleFactor      1 1 1
///    scaleOrientation 0 0 1  0
///    center           0 0 0
/// }
/// \endcode
/// \sa SoCenterballManip,SoHandleBoxManip,SoJackManip,SoTabBoxManip,SoTrackballManip,SoTransform,SoTransformBoxDragger,SoTransformerManip,SoTransformManip
class INVENTOR_API SoTransformBoxManip : public SoTransformManip
{
    SO_NODE_HEADER(SoTransformBoxManip);

  public:
    /// Constructor.
    SoTransformBoxManip();

  SoINTERNAL public:
    static void initClass();  // initialize the class

  protected:
    virtual ~SoTransformBoxManip();
};    

#endif  /* _SO_TRANSFORM_BOX_MANIP_ */
