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
 |	This file defines the SoFont node class.
 |
 |   Author(s)		: Thad Beier, Gavin Bell
 |
 ______________  S I L I C O N   G R A P H I C S   I N C .  ____________
 _______________________________________________________________________
 */

#ifndef  _SO_FONT_
#define  _SO_FONT_

#include <Inventor/fields/SoSFFloat.h>
#include <Inventor/fields/SoSFName.h>
#include <Inventor/nodes/SoSubNode.h>

#include <map>

/// Node that defines font type and size for text.
/// \ingroup Nodes
/// This node defines the current font type and point size for all
/// subsequent text shapes in the scene graph.  Fonts are specified with
/// PostScript names, except for the default font. The default font is
/// called <b>"defaultFont"</b> and is the standard SGI graphics font for 2D
/// text. <b>"Utopia"</b> is the standard Inventor font for 3D text.
///
/// \par Action behavior:
/// <b>SoGLRenderAction, SoCallbackAction, SoGetBoundingBoxAction, SoRayPickAction</b>
/// Sets the font name and size in the current traversal state.
///
/// \par File format/defaults:
/// \code
/// SoFont {
///    name	"defaultFont"
///    size	10
/// }
/// \endcode
/// \sa SoText2, SoText3
class INVENTOR_API SoFont : public SoNode {

    SO_NODE_HEADER(SoFont);

  public:
    /// This field defines the font name as a PostScript name. For example,
    /// Times Roman would be specified as <b>Times-Roman</b>.  Multiple fonts
    /// can be specified by separating the font names with semicolons.  When
    /// multiple fonts are specified, the first font in the list that contains a
    /// given character is used to render that character.
    SoSFName		name;

    /// This field defines the font size. The value is in points for
    /// 2D text and is in the current units for 3D text.
    SoSFFloat		size;

    /// Creates a font node with default settings.
    SoFont();

    /// Returns the full font path
    static SbString getFontFileName(const SbName & fontName);

    /// Register a font with corresponding path.
    static void addFontFileName(const SbName & fontName, const SbString & fontPath);

  SoEXTENDER public:
    virtual void	doAction(SoAction *action);
    virtual void	GLRender(SoGLRenderAction *action);
    virtual void	callback(SoCallbackAction *action);
    virtual void	pick(SoPickAction *action);
    virtual void	getBoundingBox(SoGetBoundingBoxAction *action);

  SoINTERNAL public:
    static void	initClass();
    static void	finishClass();

  protected:
    virtual ~SoFont();

    static std::map<SbName, SbString> s_font_map;
};

#endif /* _SO_FONT_ */
