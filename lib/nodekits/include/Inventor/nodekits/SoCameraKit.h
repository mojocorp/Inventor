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
 |      Defines the SoCameraKit class.
 |      Subclassed off of SoBaseKit, it is the base class for all camera kits.
 |
 |   Author(s)          : Paul Isaacs
 |
 ______________  S I L I C O N   G R A P H I C S   I N C .  ____________
 _______________________________________________________________________
*/

#ifndef _SO_CAMERA_KIT_
#define _SO_CAMERA_KIT_

#include <Inventor/nodekits/SoBaseKit.h>

////////////////////////////////////////////////////////////////////
//    Class: SoCameraKit
//
// NOTE TO DEVELOPERS:
//     For info about the structure of SoCameraKit:
//     [1] compile: /usr/share/src/Inventor/samples/ivNodeKitStructure
//     [2] type:    ivNodeKitStructure SoCameraKit.
//     [3] The program prints a diagram of the scene graph and a table with
//         information about each part.
//
//      A parent node that manages a collection of child nodes
//      into a unit with the following structure:
//
//                            this
//                              |
//        ------------------------
//        |                      |
//     "callbackList"       "transformGroup"
//                               |
//                      ----------------
//                      |              |
//                    "transform" "camera"
//
////////////////////////////////////////////////////////////////////

/// Camera nodekit class.
/// \ingroup Nodekits
/// This nodekit class is used to create camera nodes that have
/// a local transformation.
/// <tt>SoCameraKit</tt> adds two public parts to the basic nodekit:
/// <em>transform</em> and <em>camera</em>.
///
///
/// The <em>camera</em> part is created by default as an
/// <tt>SoPerspectiveCamera</tt> node, but may later be changed to any subclass
/// of <tt>SoCamera</tt>.
///
///
/// You can move the camera relative to the rest of the scene by creating
/// and editing the <em>transform</em> part.
///
///
/// <tt>SoCameraKit</tt> also adds a private part, <em>transformGroup</em>,
/// which is of type <tt>SoTransformSeparator</tt>. The kit uses this part to
/// contain the effect of <em>transform</em> to move only the <em>camera</em>,
/// while allowing the <em>camera</em> to affect the rest of the scene.
///
///
/// <tt>SoCameraKit</tt> is derived from <tt>SoBaseKit</tt>
/// and thus also includes a <em>callbackList</em> part for adding callback
/// nodes.
///
/// \par File format/defaults:
/// \code
/// CameraKit {
///     callbackList    NULL
///     transform       NULL
///     camera          PerspectiveCamera {}
/// }
/// \endcode
/// \sa
/// SoAppearanceKit,SoBaseKit,SoLightKit,SoNodeKit,SoNodeKitDetail,SoNodeKitListPart,SoNodeKitPath,SoNodekitCatalog,SoSceneKit,SoSeparatorKit,SoShapeKit,SoWrapperKit
class SoCameraKit : public SoBaseKit {

    SO_KIT_HEADER(SoCameraKit);

    // defines fields for the new parts in the catalog
    SO_KIT_CATALOG_ENTRY_HEADER(transformGroup);
    SO_KIT_CATALOG_ENTRY_HEADER(transform);
    SO_KIT_CATALOG_ENTRY_HEADER(camera);

  public:
    /// Default constructor
    SoCameraKit();

    SoINTERNAL
  public:
    static void initClass();

  protected:
    virtual ~SoCameraKit();
};
#endif /* _SO_CAMERA_KIT_ */
