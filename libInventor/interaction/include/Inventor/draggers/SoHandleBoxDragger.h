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
 |	This file defines the handle box dragger class.
 |
 | NOTE TO DEVELOPERS:
 |     For info about the structure of SoHandleBoxDragger:
 |     [1] compile: /usr/share/src/Inventor/samples/ivNodeKitStructure
 |     [2] type:    ivNodeKitStructure SoHandleBoxDragger.
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
 |  NOTE: for the translators and extruders, 
 |        numbering refers to a cube-face:
 |        1=top,2=bot,3=lft,4=rit,5=fnt,6=bak
 |
 |	handleBoxTranslator1               - translator1
 |	handleBoxTranslator1Active         - translator1Active
 |      (ditto for translator2-translator6)
 |
 |      handleBoxExtruder1                 - extruder1
 |      handleBoxExtruder1Active           - extruder1Active
 |      (ditto for extruder2-extruder6)
 |
 |  
 |  NOTE: for the uniform scalers, 
 |        numbers refer to a corner: 
 |        1=+x+y+z,  2=+x+y-z, 3=+x-y+z, 
 |        4=+x-y-z,  5=-x+y+z,  6=-x+y-z, 
 |        7=-x-y+z, 8=-x-y-z
 |      handleBoxUniform1		   - uniform1
 |      handleBoxUniform1Active            - uniform1Active
 |      (ditto for uniform2-uniform8)
 |
 |  NOTE: for the arrows, numbering refers
 |        to cube-face they point out from:
 |        1=top,2=bot,3=lft,4=rit,5=fnt,6=bak
 |  
 |      handleBoxArrow1                    - arrow1
 |      (ditto for arrow2-arrow6)
 |
 |   Author(s): Paul Isaacs, David Mott
 |
 ______________  S I L I C O N   G R A P H I C S   I N C .  ____________
 _______________________________________________________________________
 */

#ifndef  _SO_HANDLEBOX_DRAGGER_
#define  _SO_HANDLEBOX_DRAGGER_

#include <Inventor/draggers/SoDragger.h>
#include <Inventor/fields/SoSFVec3f.h>
#include <Inventor/sensors/SoSensor.h>

class SbDict;
class SoDrawStyle;
class SbPlaneProjector;
class SbLineProjector;
class SoFieldSensor;

/// Box you can scale, stretch and translate by dragging with the mouse.
/// \ingroup Draggers
/// <tt>SoHandleBoxDragger</tt>
/// is a dragger shaped like a wireframe box with small <em>corner cubes</em>
/// mounted on each corner.
/// Click and drag any of these cubes to scale the box uniformly.
/// Six other <em>center cubes</em> are centered on the sides of the box; white
/// lines connect them to the center of the dragger.
/// Drag one of the center cubes along its line to stretch the box
/// in that direction.
/// Dragging a face of the box translates the dragger within that plane.
///
///
/// While you drag a face of the box, purple <em>feedback arrows</em> display the
/// possible directions of motion.  Press the <b><Shift></b> key to
/// <em>constrain</em> the motion to one of the two major directions in the
/// plane. The constraint direction is chosen based on the next user
/// gesture.  Press the <b><Control></b> key and the dragger will translate
/// <em>perpendicular</em> to that plane.  The #translation field is modified
/// as the face is dragged.
///
///
/// By default, dragging any of the small cubes scales  about the center of
/// the object.  Pressing the <b><Control></b> key changes this:  A corner cube
/// will scale about its opposite corner.  A center cube will scale about
/// the center of its opposite face.  Dragging one of the small cubes will
/// usually result in changes to both the #scaleFactor and
/// #translation fields. This is because any scale about a point other
/// than the origin has a translation element.
///
///
/// As with all draggers, if you change the fields the dragger will
/// move to match the new settings.
///
///
/// <em>Remember:</em> This is <em>not</em> an <tt>SoTransform!</tt>.
/// If you want to move other objects with this dragger, you can either:
///
///
/// [a] Use an <tt>SoHandleBoxManip</tt>, which is subclassed from <tt>SoTransform</tt>.
/// It creates one of
/// these draggers and uses it as the interface to change its fields.
/// (see the <tt>SoHandleBoxManip</tt> reference page).
///
///
/// [b] Use field-to-field connections to connect the fields of this dragger to
/// those of any <tt>SoTransformation</tt> node.
///
///
/// You can change the parts in any instance of this dragger using
/// #setPart().
/// The default part geometries are defined as resources for this
/// <tt>SoHandleBoxDragger</tt> class.  They are detailed in the
/// Dragger Resources section of the online reference page for this class.
/// You can make your program use different default resources for the parts
/// by copying the file
/// #/usr/share/data/draggerDefaults/handleBoxDragger.iv
/// into your own directory, editing the file, and then
/// setting the environment variable <b>SO_DRAGGER_DIR</b> to be a path to that directory.
/// \par Notes:
/// Unlike most multi-function draggers,  <tt>SoHandleBoxDragger</tt> is
/// not a compound dragger made up of other draggers that
/// perform its smaller tasks.  This is
/// not because it was inappropriate, but because
/// was written before implementation of the methods that
/// synchronize multiple child draggers. The younger
/// <tt>SoTransformBoxDragger</tt> has similarities to the handle box dragger, but
/// the transform box dragger <em>is</em> a compound dragger.
/// \par Nodekit structure:
/// \code
/// CLASS SoHandleBoxDragger
/// -->"this"
///       "callbackList"
///       "topSeparator"
///          "motionMatrix"
/// -->      "surroundScale"
///          "geomSeparator"
/// -->         "drawStyle"
/// -->         "translator1Switch"
/// -->            "translator1"
/// -->            "translator1Active"
/// -->         "translator2Switch"
/// -->            "translator2"
/// -->            "translator2Active"
/// -->         "translator3Switch"
/// -->            "translator3"
/// -->            "translator3Active"
/// -->         "translator4Switch"
/// -->            "translator4"
/// -->            "translator4Active"
/// -->         "translator5Switch"
/// -->            "translator5"
/// -->            "translator5Active"
/// -->         "translator6Switch"
/// -->            "translator6"
/// -->            "translator6Active"
/// -->         "extruder1Switch"
/// -->            "extruder1"
/// -->            "extruder1Active"
/// -->         "extruder2Switch"
/// -->            "extruder2"
/// -->            "extruder2Active"
/// -->         "extruder3Switch"
/// -->            "extruder3"
/// -->            "extruder3Active"
/// -->         "extruder4Switch"
/// -->            "extruder4"
/// -->            "extruder4Active"
/// -->         "extruder5Switch"
/// -->            "extruder5"
/// -->            "extruder5Active"
/// -->         "extruder6Switch"
/// -->            "extruder6"
/// -->            "extruder6Active"
/// -->         "uniform1Switch"
/// -->            "uniform1"
/// -->            "uniform1Active"
/// -->         "uniform2Switch"
/// -->            "uniform2"
/// -->            "uniform2Active"
/// -->         "uniform3Switch"
/// -->            "uniform3"
/// -->            "uniform3Active"
/// -->         "uniform4Switch"
/// -->            "uniform4"
/// -->            "uniform4Active"
/// -->         "uniform5Switch"
/// -->            "uniform5"
/// -->            "uniform5Active"
/// -->         "uniform6Switch"
/// -->            "uniform6"
/// -->            "uniform6Active"
/// -->         "uniform7Switch"
/// -->            "uniform7"
/// -->            "uniform7Active"
/// -->         "uniform8Switch"
/// -->            "uniform8"
/// -->            "uniform8Active"
/// -->         "arrowTranslation"
/// -->         "arrow1Switch"
/// -->            "arrow1"
/// -->         "arrow2Switch"
/// -->            "arrow2"
/// -->         "arrow3Switch"
/// -->            "arrow3"
/// -->         "arrow4Switch"
/// -->            "arrow4"
/// -->         "arrow5Switch"
/// -->            "arrow5"
/// -->         "arrow6Switch"
/// -->            "arrow6"
/// \endcode
///
/// \par File format/defaults:
/// \code
/// SoHandleBoxDragger {
///     renderCaching       AUTO
///     boundingBoxCaching  AUTO
///     renderCulling       AUTO
///     pickCulling         AUTO
///     isActive            FALSE
///     translation         0 0 0
///     scaleFactor         1 1 1
///     callbackList        NULL
///     surroundScale       NULL
///     translator1         <handleBoxTranslator1 resource>
///     translator1Active   <handleBoxTranslator1Active resource>
///     translator2         <handleBoxTranslator2 resource>
///     translator2Active   <handleBoxTranslator2Active resource>
///     translator3         <handleBoxTranslator3 resource>
///     translator3Active   <handleBoxTranslator3Active resource>
///     translator4         <handleBoxTranslator4 resource>
///     translator4Active   <handleBoxTranslator4Active resource>
///     translator5         <handleBoxTranslator5 resource>
///     translator5Active   <handleBoxTranslator5Active resource>
///     translator6         <handleBoxTranslator6 resource>
///     translator6Active   <handleBoxTranslator6Active resource>
///     extruder1           <handleBoxExtruder1 resource>
///     extruder1Active     <handleBoxExtruder1Active resource>
///     extruder2           <handleBoxExtruder2 resource>
///     extruder2Active     <handleBoxExtruder2Active resource>
///     extruder3           <handleBoxExtruder3 resource>
///     extruder3Active     <handleBoxExtruder3Active resource>
///     extruder4           <handleBoxExtruder4 resource>
///     extruder4Active     <handleBoxExtruder4Active resource>
///     extruder5           <handleBoxExtruder5 resource>
///     extruder5Active     <handleBoxExtruder5Active resource>
///     extruder6           <handleBoxExtruder6 resource>
///     extruder6Active     <handleBoxExtruder6Active resource>
///     uniform1            <handleBoxUniform1 resource>
///     uniform1Active      <handleBoxUniform1Active resource>
///     uniform2            <handleBoxUniform2 resource>
///     uniform2Active      <handleBoxUniform2Active resource>
///     uniform3            <handleBoxUniform3 resource>
///     uniform3Active      <handleBoxUniform3Active resource>
///     uniform4            <handleBoxUniform4 resource>
///     uniform4Active      <handleBoxUniform4Active resource>
///     uniform5            <handleBoxUniform5 resource>
///     uniform5Active      <handleBoxUniform5Active resource>
///     uniform6            <handleBoxUniform6 resource>
///     uniform6Active      <handleBoxUniform6Active resource>
///     uniform7            <handleBoxUniform7 resource>
///     uniform7Active      <handleBoxUniform7Active resource>
///     uniform8            <handleBoxUniform8 resource>
///     uniform8Active      <handleBoxUniform8Active resource>
///     arrow1              <handleBoxArrow1 resource>
///     arrow2              <handleBoxArrow2 resource>
///     arrow3              <handleBoxArrow3 resource>
///     arrow4              <handleBoxArrow4 resource>
///     arrow5              <handleBoxArrow5 resource>
///     arrow6              <handleBoxArrow6 resource>
/// }
/// \endcode
/// \sa SoInteractionKit,SoDragger,SoCenterballDragger,SoDirectionalLightDragger,SoDragPointDragger,SoJackDragger,SoPointLightDragger,SoRotateCylindricalDragger,SoRotateDiscDragger,SoRotateSphericalDragger,SoScale1Dragger,SoScale2Dragger,SoScale2UniformDragger,SoScaleUniformDragger,SoSpotLightDragger,SoTabBoxDragger,SoTabPlaneDragger,SoTrackballDragger,SoTransformBoxDragger,SoTransformerDragger,SoTranslate1Dragger,SoTranslate2Dragger
class INVENTOR_API SoHandleBoxDragger : public SoDragger {

    SO_KIT_HEADER(SoHandleBoxDragger);

    SO_KIT_CATALOG_ENTRY_HEADER(drawStyle);

    // For making the dragger surround what lies above it.
    SO_KIT_CATALOG_ENTRY_HEADER(surroundScale);

    SO_KIT_CATALOG_ENTRY_HEADER(translator1Switch);
    SO_KIT_CATALOG_ENTRY_HEADER(translator1);
    SO_KIT_CATALOG_ENTRY_HEADER(translator1Active);
    SO_KIT_CATALOG_ENTRY_HEADER(translator2Switch);
    SO_KIT_CATALOG_ENTRY_HEADER(translator2);
    SO_KIT_CATALOG_ENTRY_HEADER(translator2Active);
    SO_KIT_CATALOG_ENTRY_HEADER(translator3Switch);
    SO_KIT_CATALOG_ENTRY_HEADER(translator3);
    SO_KIT_CATALOG_ENTRY_HEADER(translator3Active);
    SO_KIT_CATALOG_ENTRY_HEADER(translator4Switch);
    SO_KIT_CATALOG_ENTRY_HEADER(translator4);
    SO_KIT_CATALOG_ENTRY_HEADER(translator4Active);
    SO_KIT_CATALOG_ENTRY_HEADER(translator5Switch);
    SO_KIT_CATALOG_ENTRY_HEADER(translator5);
    SO_KIT_CATALOG_ENTRY_HEADER(translator5Active);
    SO_KIT_CATALOG_ENTRY_HEADER(translator6Switch);
    SO_KIT_CATALOG_ENTRY_HEADER(translator6);
    SO_KIT_CATALOG_ENTRY_HEADER(translator6Active);


    SO_KIT_CATALOG_ENTRY_HEADER(extruder1Switch);
    SO_KIT_CATALOG_ENTRY_HEADER(extruder1);
    SO_KIT_CATALOG_ENTRY_HEADER(extruder1Active);
    SO_KIT_CATALOG_ENTRY_HEADER(extruder2Switch);
    SO_KIT_CATALOG_ENTRY_HEADER(extruder2);
    SO_KIT_CATALOG_ENTRY_HEADER(extruder2Active);
    SO_KIT_CATALOG_ENTRY_HEADER(extruder3Switch);
    SO_KIT_CATALOG_ENTRY_HEADER(extruder3);
    SO_KIT_CATALOG_ENTRY_HEADER(extruder3Active);
    SO_KIT_CATALOG_ENTRY_HEADER(extruder4Switch);
    SO_KIT_CATALOG_ENTRY_HEADER(extruder4);
    SO_KIT_CATALOG_ENTRY_HEADER(extruder4Active);
    SO_KIT_CATALOG_ENTRY_HEADER(extruder5Switch);
    SO_KIT_CATALOG_ENTRY_HEADER(extruder5);
    SO_KIT_CATALOG_ENTRY_HEADER(extruder5Active);
    SO_KIT_CATALOG_ENTRY_HEADER(extruder6Switch);
    SO_KIT_CATALOG_ENTRY_HEADER(extruder6);
    SO_KIT_CATALOG_ENTRY_HEADER(extruder6Active);

    SO_KIT_CATALOG_ENTRY_HEADER(uniform1Switch);
    SO_KIT_CATALOG_ENTRY_HEADER(uniform1);
    SO_KIT_CATALOG_ENTRY_HEADER(uniform1Active);
    SO_KIT_CATALOG_ENTRY_HEADER(uniform2Switch);
    SO_KIT_CATALOG_ENTRY_HEADER(uniform2);
    SO_KIT_CATALOG_ENTRY_HEADER(uniform2Active);
    SO_KIT_CATALOG_ENTRY_HEADER(uniform3Switch);
    SO_KIT_CATALOG_ENTRY_HEADER(uniform3);
    SO_KIT_CATALOG_ENTRY_HEADER(uniform3Active);
    SO_KIT_CATALOG_ENTRY_HEADER(uniform4Switch);
    SO_KIT_CATALOG_ENTRY_HEADER(uniform4);
    SO_KIT_CATALOG_ENTRY_HEADER(uniform4Active);
    SO_KIT_CATALOG_ENTRY_HEADER(uniform5Switch);
    SO_KIT_CATALOG_ENTRY_HEADER(uniform5);
    SO_KIT_CATALOG_ENTRY_HEADER(uniform5Active);
    SO_KIT_CATALOG_ENTRY_HEADER(uniform6Switch);
    SO_KIT_CATALOG_ENTRY_HEADER(uniform6);
    SO_KIT_CATALOG_ENTRY_HEADER(uniform6Active);
    SO_KIT_CATALOG_ENTRY_HEADER(uniform7Switch);
    SO_KIT_CATALOG_ENTRY_HEADER(uniform7);
    SO_KIT_CATALOG_ENTRY_HEADER(uniform7Active);
    SO_KIT_CATALOG_ENTRY_HEADER(uniform8Switch);
    SO_KIT_CATALOG_ENTRY_HEADER(uniform8);
    SO_KIT_CATALOG_ENTRY_HEADER(uniform8Active);

    SO_KIT_CATALOG_ENTRY_HEADER(arrowTranslation);

    SO_KIT_CATALOG_ENTRY_HEADER(arrow1Switch);
    SO_KIT_CATALOG_ENTRY_HEADER(arrow1);
    SO_KIT_CATALOG_ENTRY_HEADER(arrow2Switch);
    SO_KIT_CATALOG_ENTRY_HEADER(arrow2);
    SO_KIT_CATALOG_ENTRY_HEADER(arrow3Switch);
    SO_KIT_CATALOG_ENTRY_HEADER(arrow3);
    SO_KIT_CATALOG_ENTRY_HEADER(arrow4Switch);
    SO_KIT_CATALOG_ENTRY_HEADER(arrow4);
    SO_KIT_CATALOG_ENTRY_HEADER(arrow5Switch);
    SO_KIT_CATALOG_ENTRY_HEADER(arrow5);
    SO_KIT_CATALOG_ENTRY_HEADER(arrow6Switch);
    SO_KIT_CATALOG_ENTRY_HEADER(arrow6);

  public:
    /// Constructor.
    SoHandleBoxDragger();
    
    SoSFVec3f    translation; ///< Scale of the dragger.
    SoSFVec3f    scaleFactor; ///< Position of the dragger.

  SoINTERNAL public:
    static void		initClass();	// initialize the class

  protected:

    // Callbacks for drag start, motion, and finish
    static void startCB( void *, SoDragger * );
    static void motionCB( void *, SoDragger * );
    static void finishCB( void *, SoDragger * );
    
    SoFieldSensor *translFieldSensor;
    SoFieldSensor *scaleFieldSensor;
    static void   fieldSensorCB( void *, SoSensor * );
    static void valueChangedCB( void *, SoDragger * );

    // Callbacks for pressing and releasing the meta keys
    static void	metaKeyChangeCB( void *, SoDragger *);

    // These really do the work during startCB, motionCB, and finishCB
    void	dragStart();
    void	drag();
    void	dragFinish();

    void setAllPartsActive( SbBool onOrOff );

    // detach/attach any sensors, callbacks, and/or field connections.
    // Called by:            start/end of SoBaseKit::readInstance
    // and on new copy by:   start/end of SoBaseKit::copy.
    // Classes that redefine must call setUpConnections(TRUE,TRUE) 
    // at end of constructor.
    // Returns the state of the node when this was called.
    virtual SbBool setUpConnections( SbBool onOff, SbBool doItAlways = FALSE );

    virtual void setDefaultOnNonWritingFields();

    virtual ~SoHandleBoxDragger();

  private:

    enum WhichAxis 
        { POSX, POSY, POSZ, NEGX, NEGY, NEGZ };
    void   highlightAxisForScale( WhichAxis axis );

    enum State
	{ INACTIVE, 
	  SCALE_GEOM_ONLY,
	   RIT_X_SCALE,  TOP_Y_SCALE,  FNT_Z_SCALE, 
	   LFT_X_SCALE,  BOT_Y_SCALE,  BAK_Z_SCALE, 

	  PX_PY_PZ_3D_SCALE, PX_PY_NZ_3D_SCALE, PX_NY_PZ_3D_SCALE, 
	  PX_NY_NZ_3D_SCALE, NX_PY_PZ_3D_SCALE, NX_PY_NZ_3D_SCALE, 
	  NX_NY_PZ_3D_SCALE, NX_NY_NZ_3D_SCALE, 

	  RIT_TRANSLATE,  TOP_TRANSLATE,  FNT_TRANSLATE,
	  LFT_TRANSLATE,  BOT_TRANSLATE,  BAK_TRANSLATE };

    State		currentState;     
    int                 translateDir;   // Used for 1-D translation,
				        // this variable records the current
				        // axis being used.

    SbPlaneProjector    *motionPlaneProj;
    SbLineProjector     *motionLineProj;

    State	    restartState;     // used by meta callback. 
    SbVec3f         worldRestartPt; // used during interaction

    SbBool          altDown, ctlDown, shftDown;  // used to keep track of
						 // which meta keys were down.

    // functions which do all the work
    void	setHighlights();
    void	setFeedbackArrows();

    SbBool		translateInit();
    SbBool		faceScaleInit();
    SbBool		uniformScaleInit();
    SbBool		scaleGeomSizeOnlyInit();

    SbBool		translateDrag();
    SbBool		scaleDrag();
    SbBool		scaleGeomSizeOnlyDrag();

    // character strings from which the shared geometry is read
    static const unsigned char geomBuffer[];
};

#endif /* _SO_HANDLEBOX_DRAGGER_ */
