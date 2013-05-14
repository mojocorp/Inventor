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
 |	This file defines the SoSeparator node class.
 |
 |   Author(s)		: Paul S. Strauss, Nick Thompson
 |
 ______________  S I L I C O N   G R A P H I C S   I N C .  ____________
 _______________________________________________________________________
 */

#ifndef  _SO_SEPARATOR_
#define  _SO_SEPARATOR_

#include <Inventor/nodes/SoGroup.h>
#include <Inventor/fields/SoSFEnum.h>
#include <Inventor/fields/SoSFBitMask.h>

class SoBoundingBoxCache;
class SoGLCacheList;

/// Group node that saves and restores traversal state.
/// \ingroup Nodes
/// This group node performs a push (save) of the traversal state before
/// traversing its children and a pop (restore) after traversing them.
/// This isolates the separator's children from the rest of the scene
/// graph. A separator can include lights, cameras, coordinates, normals,
/// bindings, and all other properties. Separators are relatively
/// inexpensive, so they can be used freely within scenes.
///
/// The <tt>SoSeparator</tt> node provides caching of state during rendering
/// and bounding box computation. This feature can be enabled by setting
/// the #renderCaching and #boundingBoxCaching fields. By default,
/// these are set to <b>AUTO</b>, which means that Inventor decides whether
/// to build a cache based on internal heuristics.
///
/// Separators can also perform culling during rendering and picking.
/// Culling skips over traversal of the separator's children if they are
/// not going to be rendered or picked, based on the comparison of the
/// separator's bounding box with the current view volume. Culling is
/// controlled by the #renderCulling and #pickCulling fields.  These
/// are also set to <b>AUTO</b> by default; however, render culling can be
/// expensive (and can interfere with render caching), so the <b>AUTO</b>
/// heuristics leave it disabled unless specified otherwise.
///
/// \par Action behavior:
/// <b>SoGLRenderAction, SoCallbackAction, SoGetBoundingBoxAction,
/// SoGetMatrixAction, SoHandleEventAction, SoRayPickAction, SoSearchAction</b>
/// Saves the current traversal state, traverses all children, and
/// restores the previous traversal state.
///
/// \par File format/defaults:
/// \code
/// SoSeparator {
///    renderCaching        AUTO
///    boundingBoxCaching   AUTO
///    renderCulling        AUTO
///    pickCulling          AUTO
/// }
/// \endcode
/// \sa SoSelection, SoTransformSeparator
class INVENTOR_API SoSeparator : public SoGroup {

    SO_NODE_HEADER(SoSeparator);

  public:

    /// Possible values for caching
    enum CacheEnabled {
        OFF,			///< Never build or use a cache
        ON,			///< Always try to build a cache
        AUTO			///< Decide whether to cache based on some heuristic
    };

    // Fields
    SoSFEnum renderCaching;         ///< Whether to cache during rendering traversal.
    SoSFEnum boundingBoxCaching;    ///< Whether to cache during bounding box traversal.
    SoSFEnum renderCulling;         ///< Whether to cull during rendering traversal.
    SoSFEnum pickCulling;           ///< Whether to cull during picking traversal.

    /// Creates a separator node with default settings.
    SoSeparator();

    /// Constructor that takes approximate number of children.
    SoSeparator(int nChildren);

    /// Overrides default method on SoNode
    virtual SbBool	affectsState() const;

    /// By default, each separator node maintains 2 render caches. (This is to
    /// allow two different representations, such as filled and wire-frame, to
    /// both be cached.) The #setNumRenderCaches() method sets the number of
    /// render caches each separator will have. Each render
    /// cache uses memory, so increasing this number may increase the memory
    /// requirements of the application. This method affects only separators
    /// that are created after it is called, not separators that were created
    /// before. Setting the number of caches to 0 turns off render caching
    /// globally from then on.
    static void		setNumRenderCaches(int howMany);

    /// Returns the current number of render caches.
    static int		getNumRenderCaches();

  SoEXTENDER public:
    // Implement actions
    virtual void	doAction(SoAction *action);
    virtual void	callback(SoCallbackAction *action);
    virtual void	getBoundingBox(SoGetBoundingBoxAction *action);
    virtual void	getMatrix(SoGetMatrixAction *action);
    virtual void	GLRender(SoGLRenderAction *action);
    virtual void	handleEvent(SoHandleEventAction *action);
    virtual void	rayPick(SoRayPickAction *action);
    virtual void	search(SoSearchAction *action);

    // These methods make render traversal faster by implementing
    // different rendering paths corresponding to different action
    // path codes.
    virtual void	GLRenderBelowPath(SoGLRenderAction *action);
    virtual void	GLRenderInPath(SoGLRenderAction *action);
    virtual void	GLRenderOffPath(SoGLRenderAction *action);

  SoINTERNAL public:
    static void		initClass();

    virtual void	notify(SoNotList *list);

  protected:
    // Returns TRUE if this separator can be culled because it is
    // outside the cull volume.  The cullResults flags come from the
    // GLRenderAction->getCullTestResults() method, and track the
    // results of previous cull tests to save work when we know we're
    // completely inside the view volume.
    virtual SbBool	cullTest(SoGLRenderAction *action, int &cullResults);

    virtual SbBool	readInstance(SoInput *in, unsigned short flags);

    virtual ~SoSeparator();

  private:
    // Each separator that is created will contain up to this many
    // render caches.
    static int		numRenderCaches;

    SoBoundingBoxCache	*bboxCache;	// Cache for bounding boxes
    SoGLCacheList	*cacheList;	// Caches for GL rendering
};

#endif /* _SO_SEPARATOR_ */
