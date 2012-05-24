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
 |	This file defines the base class for all interaction nodekits.
 |
 |   Author(s): Paul Isaacs
 |
 ______________  S I L I C O N   G R A P H I C S   I N C .  ____________
 _______________________________________________________________________
 */

#ifndef  _SO_INTERACTION_KIT_
#define  _SO_INTERACTION_KIT_

#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodekits/SoBaseKit.h>
#include <Inventor/fields/SoSFEnum.h>
#include <Inventor/sensors/SoFieldSensor.h>
#include <Inventor/SoNodeKitPath.h>

class SoPath;
class SoPathList;

//////////////////////////////////////////////////////////////////////////////
//
//  Class: SoInteractionKit
//
// NOTE TO DEVELOPERS:
//     For info about the structure of SoInteractionKit:
//     [1] compile: /usr/share/src/Inventor/samples/ivNodeKitStructure
//     [2] type:    ivNodeKitStructure SoInteractionKit.
//     [3] The program prints a diagram of the scene graph and a table with 
//         information about each part.
//
//  Base class for all draggers.
//
//////////////////////////////////////////////////////////////////////////////

/// Base class for all interaction nodekit classes.
/// \ingroup Nodekits
/// This is the base class for all classes of interaction nodekits.  Currently,
/// the only subclass is <tt>SoDragger</tt>,  which reacts to click-drag-release
/// events from the mouse.
///
///
/// This node has four fields corresponding to those of an
/// <tt>SoSeparator</tt>:  They are #renderCaching, #boundingBoxCaching,
/// #renderCulling, and #pickCulling.  They behave the same here as they
/// do for an <tt>SoSeparator</tt>
///
///
/// The #setPartAsPath() method
/// provides support for creating "stand-in" objects for parts in the interaction
/// kit.  The "stand-in", or "surrogate" part, is a path to an object that lies
/// somewhere else in the scene graph.
/// \par Nodekit structure:
/// \code
/// CLASS SoInteractionKit
/// -->"this"
///       "callbackList"
/// -->   "topSeparator"
/// -->      "geomSeparator"
/// \endcode
///
/// \par File format/defaults:
/// \code
/// SoInteractionKit {
///     renderCaching       AUTO
///     boundingBoxCaching  AUTO
///     renderCulling       AUTO
///     pickCulling         AUTO
///     callbackList        NULL
/// }
/// \endcode
/// \sa SoBaseKit,SoInteraction,SoNodeKitDetail,SoNodeKitPath,SoNodekitCatalog, SoDragger,
/// \sa SoCenterballDragger,SoDirectionalLightDragger,SoDragPointDragger,SoHandleBoxDragger,
/// \sa SoJackDragger,SoPointLightDragger,SoRotateCylindricalDragger,SoRotateDiscDragger,
/// \sa SoRotateSphericalDragger,SoScale1Dragger,SoScale2Dragger,SoScale2UniformDragger,
/// \sa SoScaleUniformDragger,SoSpotLightDragger,SoTabBoxDragger,SoTabPlaneDragger,SoTrackballDragger,
/// \sa SoTransformBoxDragger,SoTranslate1Dragger,SoTranslate2Dragger
class INVENTOR_API SoInteractionKit : public SoBaseKit {

    SO_KIT_HEADER(SoInteractionKit);

    SO_KIT_CATALOG_ENTRY_HEADER(topSeparator);
    SO_KIT_CATALOG_ENTRY_HEADER(geomSeparator);

  public:

    /// Sets any public part in the interaction kit as a
    /// "surrogate" path instead.  The object at the end of the path serves as
    /// a stand-in when a pick occurs, and can thus initiate interaction.
    ///
    /// Instead of the usual #setPart(), which
    /// replaces \a partName with a new node, this will
    /// remove the node being used for \a partName from the scene and remember the
    /// \a surrogatePath you give it.  Later, any pick on \a surrogatePath will
    /// be regarded as a pick on \a partName.
    ///
    /// For example, set the \e XRotator part of an \c SoTrackballDragger
    /// to be the path to an object in the scene.  The rest of the
    /// trackball will look the same, but the \e XRotator stripe will disappear.
    /// However, click the mouse on the object at the end of \a surrogatePath and the
    /// ball will start to drag in rotation around its X axis.
    ///
    /// Note that this is different from setting the part to be the node at the end
    /// of the path. When you set the part as a node, a second instance will be
    /// drawn in the local space of the interaction kit.  When you set it as a path,
    /// the object itself is used, not a copy.
    ///
    /// The \a partName may be any part name that follows the nodekit syntax for
    /// parts, such as \a childList[0].shape or \a rotator.rotatorActive.
    /// (See the #getPart() method in the
    /// \c SoBaseKit reference page for a complete description.)
    virtual SbBool  setPartAsPath( const SbName &partName, 
				   SoPath *surrogatePath );

    /// Override the default behavior of SoNode
    virtual SbBool affectsState() const;

    /// Possible values for caching
    enum CacheEnabled {
        OFF,                    ///< Never build or use a cache
        ON,                     ///< Always try to build a cache
        AUTO                    ///< Automatic caching
    };

    /// Set render caching mode.  Default is <b>AUTO</b>.
    SoSFEnum renderCaching;

    /// Set bounding box caching mode.
    /// Default is <b>ON</b>.  Setting this value to <b>AUTO</b> is equivalent
    /// to <b>ON</b> - automatic culling is not implemented.
    SoSFEnum boundingBoxCaching;

    /// Set render culling mode.  Default is <b>OFF</b>.
    /// Setting this value to <b>AUTO</b> is equivalent to <b>ON</b> -
    /// automatic culling is not implemented.
    SoSFEnum renderCulling;

    /// Set pick caching mode.  Default is <b>AUTO</b>.
    SoSFEnum pickCulling;

  SoEXTENDER public:

    // Sets the part, then sets the field for that part to default.
    // using partField.setDefault(TRUE).
    // Hence 'newNode' becomes the new default value, and the part
    // only writes if [a] the part is set to a different node.
    // [b] values are changes in the fields of newNode.
    // [c] any child below newNode changes.
    //
    // If the flag 'onlyIfAlready' is TRUE, then this will only change values
    // of parts that are already in a default state.
    //
    // Returns FALSE on failure, TRUE otherwise.
    // First version sets the part to the given node.
    // Second version looks up resourceName in the global dictionary and sets
    // the part with the node found.
    virtual SbBool setPartAsDefault(const SbName &partName, 
		              SoNode *newNode, SbBool onlyIfAlready = TRUE );
    virtual SbBool setPartAsDefault(const SbName &partName, 
		    const SbName &newNodeName, SbBool onlyIfAlready = TRUE);

    // Is the 'inPath' valid as a surrogate path anywhere in this node's
    // subgraph? 
    // Examines the surrogate paths of all InteractionKits from this node down.
    // The first time a surrogate path is found that is contained within 
    // 'inPath', then TRUE is returned.
    // The second version also returns information about the surrogate,
    // only if (fillArgs == TRUE).
    SbBool isPathSurrogateInMySubgraph(const SoPath *pathToCheck);
    SbBool isPathSurrogateInMySubgraph(const SoPath *pathToCheck, 
				       SoPath *&pathToOwner,
				       SbName  &surrogateNameInOwner,
				       SoPath *&surrogatePathInOwner,
				       SbBool  fillArgs = TRUE );

    static void setSwitchValue( SoNode *n, int newVal );

  SoINTERNAL public:
    static void		initClass();	// initialize the class

  protected:
    SoInteractionKit();
    ~SoInteractionKit();

    // Copies the contents of the given instance into this instance
    // This copies the surrogatePartPathList and the surrogatePartNameList
    virtual void	copyContents(const SoFieldContainer *fromFC,
				     SbBool copyConnections);

    // These are overloaded to set the surrogate path to NULL after completing
    // their work.
    virtual SoNode *getAnyPart( const SbName &partName, SbBool makeIfNeeded,
			SbBool leafCheck = FALSE, SbBool publicCheck = FALSE );
    virtual SoNodeKitPath *createPathToAnyPart(const SbName &partName,
			SbBool makeIfNeeded,
			SbBool leafCheck = FALSE, SbBool publicCheck = FALSE,
			const SoPath *pathToExtend = NULL );
    virtual SbBool setAnyPart( const SbName &partName, SoNode *from,
			SbBool anyPart = TRUE );

    // This allows you to set any part in the nodekit as a surrogate path.
    // Note: The node corresponding to 'partName' will become empty.
    //       If 'partName' is not a leaf, the subgraph below it will disappear.
    virtual SbBool  setAnyPartAsPath( const SbName &partName, 
				      SoPath *surrogatePath,
				      SbBool leafCheck = FALSE, 
				      SbBool publicCheck = FALSE );

    // Reads in from file. Takes care of checking values in 
    // the topSeparator fields and the field of this node.
    virtual SbBool readInstance(SoInput *in, unsigned short flags);

    static void readDefaultParts( const char *fileName,        // file to read
		                  const char defaultBuffer[],   // if no file,
				    int defBufSize );	     // read from buffer

    // Protected versions of these methods that allow you to set non-leaf
    // and/or private parts.
    virtual SbBool setAnyPartAsDefault(const SbName &partName, 
		       SoNode *newNode, SbBool anyPart = TRUE, 
		       SbBool onlyIfAlready = TRUE );
    virtual SbBool setAnyPartAsDefault(const SbName &partName, 
		       const SbName &newNodeName, SbBool anyPart = TRUE, 
		       SbBool onlyIfAlready = TRUE );

    SbBool setAnySurrogatePath( const SbName &inName, SoPath *inPath,
			SbBool leafCheck = FALSE, SbBool publicCheck = FALSE );

    // detach/attach any sensors, callbacks, and/or field connections.
    // Called by:            start/end of SoBaseKit::readInstance
    // and on new copy by:   start/end of SoBaseKit::copy.
    // Classes that redefine must call setUpConnections(TRUE,TRUE) 
    // at end of constructor.
    // Returns the state of the node when this was called.
    virtual SbBool setUpConnections( SbBool onOff, SbBool doItAlways = FALSE );

    // Called by the SoBaseKit::write() method. 
    //
    // InteractionKits don't want to write fields if they have default vals.
    //
    // Calls setDefault(TRUE) on caching fields if default and not 
    // connected.
    //
    // Calls setDefault(TRUE) on the topSeparator. 
    // Calls setDefault(TRUE) on the geomSeparator. 
    // 
    // Calls setDefault(TRUE) on any non-leaf part of type exactly == SoSwitch.
    // Subclasses can override this if they like, since the function is virtual.
    // But interactionKits use non-leaf switches to turn parts on and off, and 
    // this does not need to be written to file.
    //
    // Lastly, calls same method on SoBaseKit.
    //
    // NOTE: Parts which are set to default may still wind up writing to file 
    //       if, for example, they lie on a path.
    virtual void setDefaultOnNonWritingFields();

    // This sensor will watch the topSeparator part.  If the part changes to a 
    // new node,  then the fields of the old part will be disconnected and
    // the fields of the new part will be connected.
    // Connections are made from/to the renderCaching, boundingBoxCaching,
    // renderCulling and pickCulling fields. This way, the SoInteractionKit
    // can be treated from the outside just like a regular SoSeparator node.
    // Setting the fields will affect caching and culling, even though the
    // topSeparator takes care of it.
    // oldTopSep keeps track of the part for comparison.
    SoFieldSensor *fieldSensor;
    static void fieldSensorCB( void *, SoSensor *);
    SoSeparator *oldTopSep;

    void connectSeparatorFields( SoSeparator *dest, SbBool onOff );

  private:
  
    // This list stores all paths that represent surrogate parts.
    // These special parts do not have geometry in the dragger subgraph.
    // Rather, picking one of these paths is regarded as tantamount to 
    // picking a particular part.
    SoPathList    *surrogatePartPathList;
    SbPList       *surrogatePartNameList;

    void   setMySurrogatePath( const SbName &name, SoPath *newPath );
    int    indexOfSurrogateInMyList(const SoPath *pathToCheck);

    // used to read information by 'readDefaultParts'
    static SoGroup *readFromFile(const char *fileName);
    static SoGroup *readFromBuffer( const char defaultBuffer[], int defBufSize);
};

#endif /* _SO_INTERACTION_KIT_ */

