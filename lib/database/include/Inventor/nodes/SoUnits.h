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
 |	This file defines the SoUnits node class.
 |
 |   Author(s)		: Paul S. Strauss
 |
 ______________  S I L I C O N   G R A P H I C S   I N C .  ____________
 _______________________________________________________________________
 */

#ifndef _SO_UNITS_
#define _SO_UNITS_

#include <Inventor/fields/SoSFEnum.h>
#include <Inventor/nodes/SoTransformation.h>

/// Node that scales to convert units of length.
/// \ingroup Nodes
/// This node defines a uniform 3D scale about the origin relative to the
/// previously defined units. The default units for all data are meters.
/// Adding a units node with the value <b>INCHES</b> will have the same
/// effect as adding an <tt>SoScale</tt> node with the <b>scaleFactor</b> of
/// (.0254, .0254, .0254). Any subsequent <tt>SoUnits</tt> node will take the
/// previous units into account. When building a composite object out of
/// a bunch of pieces, it would be a good practice to add an <tt>SoUnits</tt>
/// node at the beginning of each of the pieces, under an <tt>SoSeparator</tt>
/// node, to make sure all the pieces fit together with the same scale.
///
/// \par Action behavior:
/// <b>SoGLRenderAction, SoCallbackAction, SoGetBoundingBoxAction,
/// SoRayPickAction</b> Accumulates the scale that is the ratio of the size from
/// the previous unit to the current unit into the current transformation.
/// <b>SoGetMatrixAction</b>
/// Returns the matrix corresponding to the units scaling.
///
/// \par File format/defaults:
/// \code
/// SoUnits {
///    units	METERS
/// }
/// \endcode
/// \sa SoScale, SoTransform
class SoUnits : public SoTransformation {

    SO_NODE_HEADER(SoUnits);

  public:
    enum Units {
        METERS,
        CENTIMETERS,
        MILLIMETERS,
        MICROMETERS,
        MICRONS,
        NANOMETERS,
        ANGSTROMS,
        KILOMETERS,
        FEET,
        INCHES,
        POINTS,
        YARDS,
        MILES,
        NAUTICAL_MILES
    };

    // Fields
    SoSFEnum units; ///< Defines the current unit.

    /// Creates a unit conversion node with default settings.
    SoUnits();

    SoEXTENDER
  public:
    virtual void doAction(SoAction *action);
    virtual void callback(SoCallbackAction *action);
    virtual void getBoundingBox(SoGetBoundingBoxAction *action);
    virtual void getMatrix(SoGetMatrixAction *action);
    virtual void GLRender(SoGLRenderAction *action);
    virtual void pick(SoPickAction *action);

    SoINTERNAL
  public:
    static void initClass();

  protected:
    virtual ~SoUnits();

  private:
    static const float factor[14];
};

#endif /* _SO_UNITS_ */
