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
 |      Defines the SoWrapperKit class.
 |      A group kit which has a place to attach any inventor subgraph
 |      of any unspecified structure.
 |
 |   Author(s)          : Paul Isaacs, Thad Beier
 |
 ______________  S I L I C O N   G R A P H I C S   I N C .  ____________
 _______________________________________________________________________
*/

#pragma once

#include <Inventor/nodekits/SoSeparatorKit.h>

////////////////////////////////////////////////////////////
// Class: SoWrapperKit
//
// NOTE TO DEVELOPERS:
//     For info about the structure of SoWrapperKit:
//     [1] compile: /usr/share/src/Inventor/samples/ivNodeKitStructure
//     [2] type:    ivNodeKitStructure SoWrapperKit.
//     [3] The program prints a diagram of the scene graph and a table with
//         information about each part.
//
//  A grouping in which a programmer can shove whatever
//  subgraph (s)he wants
//
//    New nodes in this subclass are:
//         contents
//
//  Adds a node before the "childList"
//  from the SoSeparatorKit class.
//
// This results in the following structure:
//
//                            this
//              ----------------|
//              |          "topSeparator"
//          "callbackList"      |
//      ----------------------------------------------------------
//      |       |           |     |          |    |              |
//  "pickStyle" |    "transform"  |          |    |              |
//     "appearance"  "texture2Transform"     |    |              |
//                                           |    |              |
//                                           |    |              |
//                                           |    |          "contents"
//                                           |  "localTransform"
//                                           |
//                                        "childList"
//
////////////////////////////////////////////////////////////
/// Wrapper nodekit class.
/// \ingroup Nodekits
/// <tt>SoWrapperKit</tt> is derived from <tt>SoSeparatorKit</tt>.
/// It adds the capability to wrap an arbitrary scene graph, (non-nodekit),
/// within an <tt>SoSeparatorKit</tt>, so that it may be used along with other
/// shape kits in a hierarchy. There are two additional parts
/// included in <tt>SoWrapperKit</tt>: <em>localTransform</em> and
/// <em>contents</em>.
///
///
/// The part <em>contents</em> is an <tt>SoSeparator</tt> node beneath which
/// any arbitrary scene graph can be added. This is especially useful
/// for importing scene graphs of unknown structure (non-nodekits)
/// into nodekit format.
///
///
/// Since an <tt>SoWrapperKit</tt> is a class descended from
/// <tt>SoSeparatorKit</tt>, it may be put into another
/// <tt>SoSeparatorKit</tt>'s <em>childList</em>.
/// \par Nodekit structure:
/// \code
/// CLASS SoWrapperKit
/// -->"this"
///       "callbackList"
///       "topSeparator"
///          "pickStyle"
///          "appearance"
///          "units"
///          "transform"
///          "texture2Transform"
///          "childList"
/// -->      "localTransform"
/// -->      "contents"
/// \endcode
///
/// \par File format/defaults:
/// \code
/// SoWrapperKit {
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
///     localTransform      NULL
///     contents            NULL
/// }
/// \endcode
/// \sa
/// SoAppearanceKit,SoBaseKit,SoCameraKit,SoLightKit,SoNodeKit,SoNodeKitDetail,
/// \sa
/// SoNodeKitListPart,SoNodeKitPath,SoNodekitCatalog,SoSceneKit,SoSeparatorKit,SoShapeKit
class SoWrapperKit : public SoSeparatorKit {

    SO_KIT_HEADER(SoWrapperKit);

    // defines fields for the new parts in the catalog
    SO_KIT_CATALOG_ENTRY_HEADER(localTransform);
    SO_KIT_CATALOG_ENTRY_HEADER(contents);

  public:
    /// Default constructor
    SoWrapperKit();

    SoINTERNAL
  public:
    static void initClass();

  protected:
    virtual ~SoWrapperKit();
};

