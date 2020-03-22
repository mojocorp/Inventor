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
 |   Description:
 |	This file defines the transform box dragger class.
 |	This dragger allows you to perform rotations,uniform scales
 |	and translations, all in one tidy box.
 |
 | NOTE TO DEVELOPERS:
 |     For info about the structure of SoTabBoxDragger:
 |     [1] compile: /usr/share/src/Inventor/samples/ivNodeKitStructure
 |     [2] type:    ivNodeKitStructure SoTabBoxDragger.
 |     [3] The program prints a diagram of the scene graph and a table with
 |         information about each part.
 |
 |  The following parts in this dragger are created at construction time.
 |  'ResourceName' corresponds to the name of the default geometry for the
 |  part. The dragger's constructor gets the scene graph for 'ResourceName'
 |  by querying the global dictionary ( SoDB::getByName("ResourceName"); ).
 |
 |  Resource Name:                           Part Name:
 |
 |  tabBoxBoxGeom                          - boxGeom
 |
 |  tabBoxTranslator                       - tabPlane1.translator
 |  tabBoxScaleTabMaterial                 - tabPlane1.scaleTabMaterial
 |  tabBoxScaleTabHints                    - tabPlane1.scaleTabHints
 |  (ditto for tabPlane2 - tabPlane6)
 |
 |   Author(s): Paul Isaacs, Howard Look
 |
 ______________  S I L I C O N   G R A P H I C S   I N C .  ____________
 _______________________________________________________________________
 */

#ifndef _SO_TAB_BOX_DRAGGER_
#define _SO_TAB_BOX_DRAGGER_

#include <Inventor/draggers/SoDragger.h>
#include <Inventor/fields/SoSFVec3f.h>
#include <Inventor/sensors/SoSensor.h>

class SoFieldSensor;

/// Cubic object you can translate and scale by dragging with the mouse.
/// \ingroup Draggers
/// <tt>SoTabBoxDragger</tt>
/// is a composite dragger shaped like a box.
/// Inside it are six <tt>SoTabPlaneDraggers</tt> which the dragger positions
/// and orients to form a cube.
/// The operations available in an <tt>SoTabPlaneDragger</tt> (translation,
/// 1D scaling and 2D scaling) are available on each face of the cube.
/// Since they each move in their local space, the dragger may be easily
/// translated or scaled in any direction.
///
///
/// As a composite dragger, this class makes sure that when
/// one plane is dragged, the entire box moves together.
///
///
/// As each sub-dragger is moved, the <tt>SoTabBoxDragger</tt> updates
/// its #scaleFactor and #translation fields.
/// As with all draggers, if you change a field the dragger will
/// move to match the new settings.
///
///
/// <em>Remember:</em> This is <em>not</em> an <tt>SoTransform!</tt>.
/// If you want to move other objects with this dragger, you can either:
///
///
/// [a] Use an <tt>SoTabBoxManip</tt>, which is subclassed from
/// <tt>SoTransform</tt>. It creates one of these draggers and uses it as the
/// interface to change its fields. (see the <tt>SoTabBoxManip</tt> man page).
///
///
/// [b] Use field-to-field connections to connect the fields of this dragger to
/// those of any <tt>SoTransformation</tt> node.
///
///
/// You can change the parts in any instance of this dragger using
/// #setPart().  The default part geometries
/// are defined as resources for this <tt>SoTabBoxDragger</tt> class.  They are
/// detailed below in the Dragger Resources section of the online reference
/// page for this class.  You can make your program use different default
/// resources for the parts by copying the file
/// #/usr/share/data/draggerDefaults/tabBoxDragger.iv  into your own
/// directory, editing the file, and then setting the environment variable
/// <b>SO_DRAGGER_DIR</b> to be a path to that directory.
/// \par Nodekit structure:
/// \code
/// CLASS SoTabBoxDragger
/// -->"this"
///       "callbackList"
///       "topSeparator"
///          "motionMatrix"
/// -->      "surroundScale"
/// -->      "tabPlane1Sep"
/// -->         "tabPlane1Xf"
/// -->         "tabPlane1"
/// -->      "tabPlane2Sep"
/// -->         "tabPlane2Xf"
/// -->         "tabPlane2"
/// -->      "tabPlane3Sep"
/// -->         "tabPlane3Xf"
/// -->         "tabPlane3"
/// -->      "tabPlane4Sep"
/// -->         "tabPlane4Xf"
/// -->         "tabPlane4"
/// -->      "tabPlane5Sep"
/// -->         "tabPlane5Xf"
/// -->         "tabPlane5"
/// -->      "tabPlane6Sep"
/// -->         "tabPlane6Xf"
/// -->         "tabPlane6"
///          "geomSeparator"
/// -->         "boxGeom"
/// \endcode
///
/// \par File format/defaults:
/// \code
/// SoTabBoxDragger {
///     renderCaching       AUTO
///     boundingBoxCaching  AUTO
///     renderCulling       AUTO
///     pickCulling         AUTO
///     isActive            FALSE
///     translation         0 0 0
///     scaleFactor         1 1 1
///     callbackList        NULL
///     surroundScale       NULL
///     tabPlane1           TabPlaneDragger {}
///     tabPlane2           TabPlaneDragger {}
///     tabPlane3           TabPlaneDragger {}
///     tabPlane4           TabPlaneDragger {}
///     tabPlane5           TabPlaneDragger {}
///     tabPlane6           TabPlaneDragger {}
///     boxGeom             <tabBoxBoxGeom resource>
/// }
/// \endcode
/// \sa
/// SoInteractionKit,SoDragger,SoCenterballDragger,SoDirectionalLightDragger,
/// \sa SoDragPointDragger,SoHandleBoxDragger,SoJackDragger,SoPointLightDragger,
/// \sa SoRotateCylindricalDragger,SoRotateDiscDragger,SoRotateSphericalDragger,
/// \sa
/// SoScale1Dragger,SoScale2Dragger,SoScale2UniformDragger,SoScaleUniformDragger,
/// \sa
/// SoSpotLightDragger,SoTabPlaneDragger,SoTrackballDragger,SoTransformBoxDragger,
/// \sa SoTransformerDragger,SoTranslate1Dragger,SoTranslate2Dragger
class SoTabBoxDragger : public SoDragger {
    SO_KIT_HEADER(SoTabBoxDragger);

    // For making the dragger surround what lies above it.
    SO_KIT_CATALOG_ENTRY_HEADER(surroundScale);

    SO_KIT_CATALOG_ENTRY_HEADER(tabPlane1Sep);
    SO_KIT_CATALOG_ENTRY_HEADER(tabPlane1Xf);
    SO_KIT_CATALOG_ENTRY_HEADER(tabPlane1);
    SO_KIT_CATALOG_ENTRY_HEADER(tabPlane2Sep);
    SO_KIT_CATALOG_ENTRY_HEADER(tabPlane2Xf);
    SO_KIT_CATALOG_ENTRY_HEADER(tabPlane2);
    SO_KIT_CATALOG_ENTRY_HEADER(tabPlane3Sep);
    SO_KIT_CATALOG_ENTRY_HEADER(tabPlane3Xf);
    SO_KIT_CATALOG_ENTRY_HEADER(tabPlane3);
    SO_KIT_CATALOG_ENTRY_HEADER(tabPlane4Sep);
    SO_KIT_CATALOG_ENTRY_HEADER(tabPlane4Xf);
    SO_KIT_CATALOG_ENTRY_HEADER(tabPlane4);
    SO_KIT_CATALOG_ENTRY_HEADER(tabPlane5Sep);
    SO_KIT_CATALOG_ENTRY_HEADER(tabPlane5Xf);
    SO_KIT_CATALOG_ENTRY_HEADER(tabPlane5);
    SO_KIT_CATALOG_ENTRY_HEADER(tabPlane6Sep);
    SO_KIT_CATALOG_ENTRY_HEADER(tabPlane6Xf);
    SO_KIT_CATALOG_ENTRY_HEADER(tabPlane6);

    SO_KIT_CATALOG_ENTRY_HEADER(boxGeom);

  public:
    /// Constructor.
    SoTabBoxDragger();

    SoSFVec3f translation; ///< Position of the dragger.
    SoSFVec3f scaleFactor; ///< Scale of the dragger.

    /// Cause the scale tab sizes to be re-adjusted on all 6
    /// <tt>SoTabPlaneDragger</tt>s so that they remain a near constant screen
    /// space size. This happens automatically upon dragger finish. Call this to
    /// adjust the scale tab sizes at other times, for instance after the camera
    /// has changed in a viewer finish callback.
    void adjustScaleTabSize();

    SoINTERNAL
  public:
    static void initClass(); // initialize the class

  protected:
    SoFieldSensor *translFieldSensor;
    SoFieldSensor *scaleFieldSensor;
    static void    fieldSensorCB(void *, SoSensor *);
    static void    valueChangedCB(void *, SoDragger *);

    static void invalidateSurroundScaleCB(void *, SoDragger *);
    static void adjustScaleTabSizeCB(void *, SoDragger *);

    // detach/attach any sensors, callbacks, and/or field connections.
    // Also set geometry of childDraggers to be our default instead of their
    // regular default, using our resources.
    // Called by:            start/end of SoBaseKit::readInstance
    // and on new copy by:   start/end of SoBaseKit::copy.
    // Classes that redefine must call setUpConnections(TRUE,TRUE)
    // at end of constructor.
    // Returns the state of the node when this was called.
    virtual SbBool setUpConnections(SbBool onOff, SbBool doItAlways = FALSE);

    virtual void setDefaultOnNonWritingFields();

    virtual ~SoTabBoxDragger();

  private:
    static const unsigned char geomBuffer[];
};

#endif /* _SO_TAB_BOX_DRAGGER_ */
