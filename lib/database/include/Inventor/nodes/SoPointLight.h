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
 |	This file defines the SoPointLight node class.
 |
 |   Author(s)		: Paul S. Strauss
 |
 ______________  S I L I C O N   G R A P H I C S   I N C .  ____________
 _______________________________________________________________________
 */

#pragma once

#include <Inventor/fields/SoSFVec3f.h>
#include <Inventor/nodes/SoLight.h>

/// Node representing a point light source.
/// \ingroup Nodes
/// This node defines a point light source at a fixed 3D location.  A
/// point source illuminates equally in all directions; that is, it is
/// omni-directional.
///
/// \par Action behavior:
/// <b>SoGLRenderAction</b>
/// Activates this light (if so specified) during traversal. All shape
/// nodes that come after this light in the scene graph are illuminated by
/// this light. The light's location is affected by the current
/// transformation.
///
/// \par File format/defaults:
/// \code
/// SoPointLight {
///    on           TRUE
///    intensity    1
///    color        1 1 1
///    location     0 0 1
/// }
/// \endcode
/// \sa SoDirectionalLight, SoSpotLight
class SoPointLight : public SoLight {

    SO_NODE_HEADER(SoPointLight);

  public:
    // Fields (in addition to those in SoLight):
    SoSFVec3f location; ///< Location of the source.

    /// Creates a point light source node with default settings.
    SoPointLight();

    SoEXTENDER
  public:
    // Creates a light source during rendering
    virtual void GLRender(SoGLRenderAction *action);

    SoINTERNAL
  public:
    static void initClass();

  protected:
    virtual ~SoPointLight();
};

