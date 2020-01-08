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
 |      Defines the SoSceneKit class.
 |      Organizes four lists into a scene:
 |         cameraList       -- a list of SoCameraKits and its subclasses.
 |         lightList        -- a list of SoLightKits and its subclasses.
 |         childList        -- a list of SoSeparatorKit and its subclasses
 |
 |   Author(s)          : Paul Isaacs
 |
 ______________  S I L I C O N   G R A P H I C S   I N C .  ____________
 _______________________________________________________________________
*/

#ifndef _SO_SCENE_KIT_
#define _SO_SCENE_KIT_

#include <Inventor/nodekits/SoBaseKit.h>

////////////////////////////////////////////////////////////////////
//    Class: SoSceneKit
//
// NOTE TO DEVELOPERS:
//     For info about the structure of SoSceneKit:
//     [1] compile: /usr/share/src/Inventor/samples/ivNodeKitStructure
//     [2] type:    ivNodeKitStructure SoSceneKit.
//     [3] The program prints a diagram of the scene graph and a table with
//         information about each part.
//
//    New nodes in this subclass are:
//         cameraList, lightList, and childList
//
//      A parent node that manages a collection of child nodes
//      into a unit with the following structure:
//
//                            this
//                              |
//              -----------------
//              |               |
//           "callbackList" "topSeparator"
//                              |
//           --------------------------------
//           |               |              |
//      "cameraList"    "lightList"   "childList"
//
////////////////////////////////////////////////////////////////////
/// Scene nodekit class.
/// \ingroup Nodekits
/// This nodekit is used to organize camera, (<tt>SoCameraKit</tt>),
/// light, (<tt>SoLightKit</tt>), and object, (<tt>SoShapeKit</tt>,
/// <tt>SoSeparatorKit</tt>, and <tt>SoWrapperKit</tt>) nodekits into a scene.
/// A scene is composed of a list of cameras, a list of lights, and a list of
/// children. There are three parts created by this nodekit:
/// <em>cameraList</em>, <em>lightList</em>, and <em>childList</em>.
///
///
/// The <em>cameraList</em> part is a <em>list part</em> of <tt>SoCameraKit</tt>
/// nodes. The list itself is an <tt>SoNodeKitListPart</tt>, and since only one
/// camera can be active at a time, the <em>container</em> of the <em>list
/// part</em> is an <tt>SoSwitch</tt> node. Use #setCameraNumber(), and the
/// scene kit will set the switch to make that camera active.
///
///
/// The <em>lightList</em> part is a list of <tt>SoLightKit</tt> nodes.
/// The <em>lightList</em> is used to illuminate the objects
/// contained in the <em>childList</em> part.
///
///
/// The <em>childList</em> part contains a set of <tt>SoSeparatorKit</tt> nodes.
/// You can add any kind of <tt>SoSeparatorKit</tt> to this list,
/// including <tt>SoShapeKit</tt> and <tt>SoWrapperKit</tt>.  Since each
/// <tt>SoSeparatorKit</tt> in turn contains a <em>childList</em>, this part is
/// used to describe a hierarchical scene. (See the reference page for
/// <tt>SoSeparatorKit</tt>). All members of <em>childList</em> are lit by the
/// lights in <em>lightList</em> and rendered by the active camera in
/// <em>cameraList</em>.
/// \par Nodekit structure:
/// \code CLASS SoSceneKit
/// -->"this"
///       "callbackList"
/// -->   "topSeparator"
/// -->      "cameraList"
/// -->      "lightList"
/// -->      "childList"
/// \endcode
///
/// \par File format/defaults:
/// \code
/// SoSceneKit {
///     callbackList    NULL
///     cameraList      NodeKitListPart {
///                         containerTypeName   "Switch"
///                         childTypeNames      "CameraKit"
///                         containerNode       Switch {
///                                                 whichChild 0
///                                                 CameraKit {
///                                                     camera PerspectiveCamera
///                                                     { }
///                                                 }
///                                             }
///                     }
///     lightList       NULL
///     childList       NULL
/// }
/// \endcode
/// \sa
/// SoAppearanceKit,SoBaseKit,SoCameraKit,SoLightKit,SoNodeKit,SoNodeKitDetail,SoNodeKitListPart,SoNodeKitPath,SoNodekitCatalog,SoSeparatorKit,SoShapeKit,SoWrapperKit
class SoSceneKit : public SoBaseKit {

    SO_KIT_HEADER(SoSceneKit);

    // defines fields for the new parts in the catalog
    SO_KIT_CATALOG_ENTRY_HEADER(topSeparator);
    SO_KIT_CATALOG_ENTRY_HEADER(cameraList);
    SO_KIT_CATALOG_ENTRY_HEADER(lightList);
    SO_KIT_CATALOG_ENTRY_HEADER(childList);

  public:
    /// Default constructor
    SoSceneKit();

    /// Sets the switch node in 'cameraList' to be the number given
    int  getCameraNumber();
    void setCameraNumber(int camNum);

    /// Overrides default method on SoNode
    virtual SbBool affectsState() const;

    SoINTERNAL
  public:
    static void initClass();

  protected:
    virtual ~SoSceneKit();
};
#endif /* _SO_SCENE_KIT_ */
