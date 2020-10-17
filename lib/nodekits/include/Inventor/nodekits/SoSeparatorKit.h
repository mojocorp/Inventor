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
 |      Defines the SoSeparatorKit class.
 |      Organizes an appearance, a transform, a local transform and a
 |      group of child kits
 |
 |   Author(s)          : Paul Isaacs, Thad Beier
 |
 ______________  S I L I C O N   G R A P H I C S   I N C .  ____________
 _______________________________________________________________________
*/

#pragma once

#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodekits/SoBaseKit.h>
#include <Inventor/fields/SoSFEnum.h>
#include <Inventor/sensors/SoFieldSensor.h>

////////////////////////////////////////////////////////////////////
//    Class: SoSeparatorKit
//
// NOTE TO DEVELOPERS:
//     For info about the structure of SoSeparatorKit:
//     [1] compile: /usr/share/src/Inventor/samples/ivNodeKitStructure
//     [2] type:    ivNodeKitStructure SoSeparatorKit.
//     [3] The program prints a diagram of the scene graph and a table with
//         information about each part.
//
//    New nodes in this subclass are:
//         callbackList, topSeparator, pickStyle, appearance,
//          transform, texture2Transform,
//          childList
//
//      A parent node that manages a collection of child nodes
//      into a unit with the following structure:
//
//                            this
//            ------------------|
//            |          "topSeparator"
//         "callbackList"       |
//      ---------------------------------------------------------------
//      |       |       |           |     |                           |
//  "pickStyle" |    "units" "transform"  |                           |
//     "appearance"  "texture2Transform"                              |
//                                                                    |
//                                                                    |
//                                                              "childList"
//
////////////////////////////////////////////////////////////////////

/// Separator nodekit class.
/// \ingroup Nodekits
/// A nodekit that is used for creating nodekit hierarchies.
/// <tt>SoSeparatorKit</tt> contains a <em>transform</em> part, a
/// <em>childList</em> part, and a few others in its catalog. The
/// <em>transform</em> part (an <tt>SoTransform</tt> node) affects all of the
/// children in the <b>childList</b>. Each of these children must be an
/// <tt>SoSeparatorKit</tt> or from a class that is derived from
/// <tt>SoSeparatorKit</tt> (e.g., <tt>SoShapeKit</tt> and
/// <tt>SoWrapperKit</tt>). Since all members of the <em>childList</em> are in
/// turn <tt>SoSeparatorKit</tt>s, and each contains a <em>transform</em>, these
/// nested lists allow you to create a hierarchy of motion, in which each
/// <em>transform</em> affects an entire subgraph of nodekits.
///
///
/// The other parts added to the catalog for the <tt>SoSeparatorKit</tt> are
/// <em>pickStyle</em>, <em>appearance</em>, <em>units</em> and
/// <em>texture2Transform</em>. Furthermore, since <tt>SoSeparator</tt> is
/// derived from <tt>SoBaseKit</tt>, it inherits the <em>callbackList</em> part.
/// This is a list of <tt>SoCallback</tt> and/or <tt>SoEventCallback</tt> nodes
/// which enable the <tt>SoSeparatorKit</tt> to perform special callbacks
/// whenever an action is applied to it.
///
///
/// By creating the <em>pickStyle</em> part, a user can alter the pick style
/// for the entire nodekit hierarchy. The <em>appearance</em> part is an
/// <tt>SoAppearanceKit</tt> nodekit. Note that all parts contained in the
/// <tt>SoAppearanceKit</tt> catalog can be accessed
/// as if they were part of the <tt>SoSeparatorKit</tt>. For example:
/// \code
/// myMtl = mySepKit->getPart("material",TRUE)
/// \endcode
/// and
/// \code
/// mySepKit->setPart("material",myMtl)
/// \endcode
/// See <tt>SoBaseKit</tt> for further explanation.
/// \par Nodekit structure:
/// \code
/// CLASS SoSeparatorKit
/// -->"this"
///       "callbackList"
/// -->   "topSeparator"
/// -->      "pickStyle"
/// -->      "appearance"
/// -->      "units"
/// -->      "transform"
/// -->      "texture2Transform"
/// -->      "childList"
/// \endcode
///
/// \par File format/defaults:
/// \code
/// SoSeparatorKit {
///     renderCaching       AUTO
///     boundingBoxCaching  AUTO
///     renderCulling       AUTO
///     pickCulling         AUTO
///     callbackList        NULL
///     pickStyle           NULL
///     appearance          NULL
///     units               NULL
///     transform           NULL
///     texture2Transform   NULL
///     childList           NULL
/// }
/// \endcode
/// \sa
/// SoAppearanceKit,SoBaseKit,SoCameraKit,SoLightKit,SoNodeKit,SoNodeKitDetail,SoNodeKitListPart,SoNodeKitPath,SoNodekitCatalog,SoSceneKit,SoShapeKit,SoWrapperKit
class INVENTOR_API SoSeparatorKit : public SoBaseKit {

    SO_KIT_HEADER(SoSeparatorKit);

    // defines fields for the new parts in the catalog
    SO_KIT_CATALOG_ENTRY_HEADER(topSeparator);
    SO_KIT_CATALOG_ENTRY_HEADER(pickStyle);
    SO_KIT_CATALOG_ENTRY_HEADER(appearance);
    SO_KIT_CATALOG_ENTRY_HEADER(units);
    SO_KIT_CATALOG_ENTRY_HEADER(transform);
    SO_KIT_CATALOG_ENTRY_HEADER(texture2Transform);
    SO_KIT_CATALOG_ENTRY_HEADER(childList);

  public:
    enum CacheEnabled { ///< Possible values for caching
        OFF,            ///< Never build or use a cache
        ON,             ///< Always try to build a cache
        AUTO            ///< Decide based on some heuristic
    };

    // Fields
    SoSFEnum renderCaching;      ///< OFF/ON/AUTO (see above)
    SoSFEnum boundingBoxCaching; ///< OFF/ON/AUTO (see above)
    SoSFEnum renderCulling;      ///< OFF/ON/AUTO (see above)
    SoSFEnum pickCulling;        ///< OFF/ON/AUTO (see above)

    /// Default constructor
    SoSeparatorKit();

    /// Overrides default method on SoNode
    virtual SbBool affectsState() const;

    SoINTERNAL
  public:
    static void initClass();

  protected:
    // detach/attach any sensors, callbacks, and/or field connections.
    // Called by:            start/end of SoBaseKit::readInstance
    // and on new copy by:   start/end of SoBaseKit::copy.
    // Classes that redefine must call setUpConnections(TRUE,TRUE)
    // at end of constructor.
    // Returns the state of the node when this was called.
    virtual SbBool setUpConnections(SbBool onOff, SbBool doItAlways = FALSE);

    // Called by the SoBaseKit::write() method. Calls setDefault(TRUE)
    // on the topSeparator. Note that this may be overriden later by basekit
    // if, for example, topSeparator lies on a path that is being written out.
    virtual void setDefaultOnNonWritingFields();

    // This sensor will watch the topSeparator part.  If the part changes to a
    // new node,  then the fields of the old part will be disconnected and
    // the fields of the new part will be connected.
    // Connections are made from/to the renderCaching, boundingBoxCaching,
    // renderCulling and pickCulling fields. This way, the SoSeparatorKit
    // can be treated from the outside just like a regular SoSeparator node.
    // Setting the fields will affect caching and culling, even though the
    // topSeparator takes care of it.
    // oldTopSep keeps track of the part for comparison.
    SoFieldSensor *fieldSensor;
    static void    fieldSensorCB(void *, SoSensor *);
    SoSeparator *  oldTopSep;

    void connectSeparatorFields(SoSeparator *dest, SbBool onOff);

  protected:
    virtual ~SoSeparatorKit();
};
