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

//
// Copyright (C) 1990-93   Silicon Graphics, Inc.
//
//______________________________________________________________________
//_____________  S I L I C O N   G R A P H I C S   I N C .  ____________
//
//   $Revision: 1.1 $
//
//   Description:
//	SoSceneManager manages rendering and event handling operations
//      on a scene graph.
//
//   Author(s): David Mott, Alain Dumesny
//
//_____________  S I L I C O N   G R A P H I C S   I N C .  ____________
//______________________________________________________________________
//

#pragma once

#include <Inventor/SbColor.h>
#include <Inventor/actions/SoGLRenderAction.h>

class SoNodeSensor;
class SoEvent;
class SoHandleEventAction;
class SoNode;
class SoSceneManager;
class SoSensor;
class SoSFTime;
class SoOneShotSensor;

/// callback function prototypes
typedef void SoSceneManagerRenderCB(void *userData, SoSceneManager *mgr);

/// Manages scene graph rendering and event handling.
/// \ingroup Interaction
/// <tt>SoSceneManager</tt> provides Inventor rendering and event handling
/// inside a window provided by the caller.
/// The scene manager is able to render in only a portion of a window if
/// desired. The <tt>SoXtRenderArea</tt> class employs a
/// <tt>SoSceneManager</tt>, and handles most all the details for setting up a
/// window, converting X events to Inventor events, automatically redrawing the
/// scene when necessary, and so on. It is simplest to use a render area when
/// rendering in an entire window. The <tt>SoSceneManager</tt> class is
/// available for programmers not working with the <em>Inventor Xt Component and
/// Utility Library</em>.
/// \sa SoXtRenderArea, SoGLRenderAction, SoHandleEventAction
class INVENTOR_API SoSceneManager {
  public:
    /// Constructor.
    SoSceneManager();

    /// Destructor.
    virtual ~SoSceneManager();

    /// Apply an SoGLRenderAction to the scene graph managed here.
    /// The caller is responsible for setting up a window to render into.
    /// If \a clearWindow is TRUE, this clears the graphics window before
    /// rendering. If \a clearZbuffer is TRUE, the z buffer will be cleared
    /// before rendering.
    virtual void render(SbBool clearWindow = TRUE, SbBool clearZbuffer = TRUE);

    /// Process the passed event by applying an SoHandleEventAction
    /// to the scene graph managed here. Returns TRUE if the event was
    /// handled by a node.
    virtual SbBool processEvent(const SoEvent *event);

    /// Reinitialize graphics. This should be called, for instance, when
    /// there is a new window.
    void reinitialize();

    /// Schedule a redraw for some time in the near future.
    /// If there is no render callback set, or this is not active,
    /// no redraw will be scheduled.
    void scheduleRedraw();

    /// Set the scene graph which is managed here. This is the
    /// Inventor scene which will be traversed for rendering and event
    /// processing.
    virtual void setSceneGraph(SoNode *newScene);

    /// Get the scene graph which is managed here.
    virtual SoNode *getSceneGraph() const;

    /// Set the size of the window in which scene mgr should render.
    /// This size must be set before render() and processEvent() are called.
    void setWindowSize(const SbVec2s &newSize);

    /// Get the size of the window in which scene mgr should render.
    const SbVec2s &getWindowSize() const;

    /// Set the size of the viewport within the window.
    /// Default is to render the entire window region, that is,
    /// origin = (0,0), size = getWindowSize().
    void setSize(const SbVec2s &newSize);

    /// Get the size of the viewport within the window.
    const SbVec2s &getSize() const;

    /// Set the origin of the viewport within the window.
    /// Default is to render the entire window region, that is,
    /// origin = (0,0), size = getWindowSize().
    void setOrigin(const SbVec2s &newOrigin);

    /// Get the origin of the viewport within the window.
    const SbVec2s &getOrigin() const;

    /// Sets current viewport region to use for rendering.
    /// This can be used instead of setting the size and origin separately.
    void setViewportRegion(const SbViewportRegion &newRegion);

    /// Gets current viewport region to use for rendering.
    const SbViewportRegion &getViewportRegion() const;

    /// Set the window background color when in RGB mode.
    /// This is the color the scene manager viewport is cleared to when
    /// render() is called with \a clearWindow set to TRUE.
    void setBackgroundColor(const SbColor &c);

    /// Gets the window background color when in RGB mode.
    /// (default to black (0,0,0));
    const SbColor &getBackgroundColor() const { return bkgColor; }

    /// Sets the window background color when in color index mode.
    void setBackgroundIndex(int index);

    /// Gets the window background color when in color index mode.
    /// (default to black (index 0)).
    int getBackgroundIndex() const { return bkgIndex; }

    /// Set the color mode (TRUE \(em RGB mode, FALSE \(em color map mode).
    /// Default is RGB mode. Only a subset of Inventor nodes will render
    /// correctly in color map mode. Basically, when in color index mode,
    /// lighting should be turned off (the model field of
    /// SoLightModel should be set to \e BASE_COLOR), and the SoColorIndex
    /// node should be used to specify colors.
    void setRGBMode(SbBool onOrOff);

    /// Get the color mode (TRUE \(em RGB mode, FALSE \(em color map mode).
    /// Default is RGB mode.
    SbBool isRGBMode() const { return rgbMode; }

    /// Activate the scene manager.
    /// The scene manager will only employ sensors for automatic redraw
    /// while it is active. Typically, the scene manager should be activated
    /// whenever its window is visible on the screen, and deactivated
    /// when its window is closed or iconified.
    virtual void activate();

    /// Deactivate the scene manager.
    virtual void deactivate();

    /// Render callback - the scene manager employs a sensor to redraw
    /// the scene when something changes. This callback is invoked for that
    /// to happen. The callback should set up its graphics window, then
    /// call the scene manager render() method. If the callback is set to NULL
    /// (the default), auto-redraw is turned off.
    void setRenderCallback(SoSceneManagerRenderCB *f, void *userData = NULL);

    /// Returns TRUE if there is currently a render callback registered.
    SbBool isAutoRedraw() const { return (renderCB != NULL); }

    /// Set the priority of the redraw sensor. Sensors are
    /// processed based on priority, with priority values of 0
    /// processed immediately. The default priority for the scene
    /// manager redraw sensor is 10000.
    void setRedrawPriority(uint32_t priority);

    /// Get the priority of the redraw sensor.
    uint32_t getRedrawPriority() const;

    /// Returns the default priority of the redraw sensor
    static uint32_t getDefaultRedrawPriority() { return 10000; }

    /// Enable/Disable the realTime global field update which normally
    /// happen right after a redraw.
    static void enableRealTimeUpdate(SbBool flag);

    /// Returns TRUE if realTime global field update is enabled.
    static SbBool isRealTimeUpdateEnabled() { return updateRealTime; }

    /// Set the antialiasing for rendering. There are two kinds of
    /// antialiasing available: smoothing and multipass antialiasing.
    /// If \a smoothing is set to TRUE, smoothing is enabled.
    /// Smoothing uses OpenGL's line- and point-smoothing features
    /// to provide cheap antialiasing of lines and points.
    /// The value of \a numPasses controls multipass antialiasing. Each time
    /// a render action is applied, Inventor renders the scene \a numPasses
    /// times from slightly different camera positions, averaging the results.
    /// \a numPasses can be from one to 255, inclusive. Setting \a numPasses to
    /// one disables multipass antialiasing. You can use either, both, or
    /// neither of these antialiasing techniques. By default, both smoothing and
    /// multipass antialiasing are disabled.
    void setAntialiasing(SbBool smoothing, int numPasses);

    /// Returns TRUE if antialiasing is enabled.
    void getAntialiasing(SbBool &smoothing, int &numPasses) const;

    SoEXTENDER
  public:
    /// User supplied render action. Highlights fall into this category.
    /// SceneManager will never delete a render action passed to this method.
    void              setGLRenderAction(SoGLRenderAction *ra);
    SoGLRenderAction *getGLRenderAction() const { return renderAction; }

    /// User supplied handle event action. This should not be done in
    /// the middle of event handling. Passing NULL turns event handling off.
    /// SceneManager will never delete a handle event action passed to
    /// this method.
    void                 setHandleEventAction(SoHandleEventAction *hea);
    SoHandleEventAction *getHandleEventAction() const {
        return handleEventAction;
    }

  protected:
    SoSceneManagerRenderCB *renderCB;
    void *                  renderCBData;
    SbBool                  isActive() const { return active; }

    // This tells the app it is time to redraw by invoking the renderCB.
    void redraw();

  private:
    SbBool               raCreatedHere;
    SbBool               heaCreatedHere;
    SoGLRenderAction *   renderAction;
    SoHandleEventAction *handleEventAction;
    SoNode *             scene;
    SoNodeSensor *       sceneSensor; // attached to root for auto-redraw
    SbBool               active;      // set TRUE if we are active
    SbBool               needZbuffer;
    SbBool needToSendVP; // TRUE when glViewport() needs to be called

    SbColor bkgColor;
    int     bkgIndex;
    SbBool  graphicsInitNeeded;
    SbBool  rgbMode;

    // static callbacks
    static void antialiasingCallback(void *);
    static void sceneSensorCallback(SoSceneManager *, SoSensor *);

    // realTime field update vars and callbacks
    static SbBool           updateRealTime;
    static SoSFTime *       realTime;
    static SoOneShotSensor *realTimeSensor; // touches realTime global field
    static void             realTimeSensorCB();
};

