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
 |   $Revision: 1.3 $
 |
 |   Description:
 |	This file contains definitions of various linear algebra classes,
 |	such as vectors, coordinates, etc..
 |
 |   Classes:
 |	SbViewVolume
 |
 |   Author(s)		: Paul S. Strauss, Nick Thompson,
 |			  David Mott, Alain Dumesny
 |
 ______________  S I L I C O N   G R A P H I C S   I N C .  ____________
 _______________________________________________________________________
 */

#ifndef _SB_VIEW_VOLUME_
#define _SB_VIEW_VOLUME_

#include <Inventor/SbBasic.h>
#include <Inventor/SbVec.h>

class SbBox3f;
class SbLine;
class SbMatrix;
class SbPlane;
class SbRotation;
class SbVec2f;

/// 3D viewing volume class.
/// \ingroup Basics
/// Class used to represent a 3D viewing volume. This class is used
/// to represent viewing frusta and picking volumes.  For perspective
/// projection, the view volume is a frustum. For orthographic (parallel)
/// projection, the view volume is a rectangular prism. \sa SbVec3f, SbVec2f,
/// SbBox3f, SbMatrix, SbRotation
class SbViewVolume {
  public:
    /// Default constructor
    SbViewVolume();

    /// Destructor
    ~SbViewVolume() {}

    /// Returns two matrices corresponding to the view volume.  The
    /// first is a viewing matrix, which is guaranteed to be an affine
    /// transformation.  The second is suitable for use as a projection
    /// matrix in GL.
    void getMatrices(SbMatrix &affine, SbMatrix &proj) const;

    /// Like the method above, but returns the affine and projection parts
    /// together in one matrix (i.e. affine.multRight( proj ) ).
    SbMatrix getMatrix() const;

    /// Returns a matrix that transforms the view volume into camera
    /// space: it translates the view volume so the view point is at
    /// the origin, and rotates it so the view direction is along the
    /// negative z axis.
    SbMatrix getCameraSpaceMatrix() const;

    /// Maps a 2d point (in 0 <= x,y <= 1) to a 3d line.
    void projectPointToLine(const SbVec2f &pt, SbLine &line) const;

    /// Maps a 2d point (in 0 <= x,y <= 1) to a 3d line.
    void projectPointToLine(const SbVec2f &pt, SbVec3f &line0,
                            SbVec3f &line1) const;

    /// Maps the 3d point in world coordinates to a 2d point in
    /// normalized screen coordinates (0 <= x,y,z <= 1, 0 <= z <= 1).
    /// The z-screen coordinate represents the homogonized z coordinate
    /// which goes (non-linearly) from 0 at the near clipping plane to
    /// 1 at the far clipping plane.
    void projectToScreen(const SbVec3f &src, SbVec3f &dst) const;

    /// Returns a plane parallel to the near (or far) plane of the view
    /// volume at a given distance from the projection point (eye)
    SbPlane getPlane(float distFromEye) const;

    /// Returns the point along the line of sight at the given distance
    /// from the projection point (eye)
    SbVec3f getSightPoint(float distFromEye) const;

    /// Returns the projection of a given point in normalized screen
    /// coords (see projectToScreen()) onto the plane parallel to the
    /// near plane that is at distFromEye units from the eye
    SbVec3f getPlanePoint(float distFromEye, const SbVec2f &normPoint) const;

    /// Returns a rotation that would align a viewed object so that
    /// its positive x-axis (of its object space) is to the right in
    /// the view and it's positive y-axis is up. If rightAngleOnly is
    /// TRUE, it will come as close as it can to this goal by using
    /// only 90 degree rotations.
    SbRotation getAlignRotation(SbBool rightAngleOnly = FALSE) const;

    /// Returns a scale factor that would scale a unit sphere centered
    /// at worldCenter so that it would appear to have the given radius
    /// in normalized screen coordinates when projected onto the near plane
    float getWorldToScreenScale(const SbVec3f &worldCenter,
                                float          normRadius) const;

    /// Projects the given 3D bounding box onto the near plane and
    /// returns the size (in normalized screen coords) of the
    /// rectangular region that encloses it
    SbVec2f projectBox(const SbBox3f &box) const;

    /// Given a view volume, this narrows the view to the given sub-rectangle
    /// of the near plane. The coordinates of the rectangle are between
    /// 0 and 1, where (0,0) is the lower-left corner of the near plane
    /// and (1,1) is the upper-right corner.
    SbViewVolume narrow(float left, float bottom, float right, float top) const;

    /// Narrow a view volume by the given box.  The box must lie inside
    /// the unit cube, and the view will be shrunk according to the
    /// size of the box.
    SbViewVolume narrow(const SbBox3f &box) const;

    /// Sets up an orthographic view volume with the given sides.
    /// The parameters are the same as for the GL ortho() routine.
    void ortho(float left, float right, float bottom, float top, float near,
               float far);

    /// Sets up a perspective view volume with the given field of view
    /// and aspect ratio. The parameters are the same as for the GL
    /// perspective() routine, except that the field of view angle is
    /// specified in radians.
    void perspective(float fovy, float aspect, float near, float far);

    /// Set up the frustum for perspective projection.
    /// It has the same arguments and functionality as the corresponding OpenGL
    /// glFrustum() function.
    void frustum(float left, float right, float bottom, float top,
                 float nearval, float farval);

    /// Rotate the camera view direction.  Note that this accomplishes
    /// the reverse of doing a GL rotate() command after defining a
    /// camera, which rotates the scene viewed by the camera.
    void rotateCamera(const SbRotation &q);

    /// Translate the camera viewpoint.  Note that this accomplishes
    /// the reverse of doing a GL translate() command after defining a
    /// camera, which translates the scene viewed by the camera.
    void translateCamera(const SbVec3f &v);

    /// Returns the positive z axis in eye space.  In this coordinate
    /// system, the z value of the near plane should be GREATER than the
    /// z value of the far plane.
    SbVec3f zVector() const;

    /// Returns a narrowed view volume which contains as tightly as
    /// possible the given interval on the z axis (in eye space).  The
    /// returned view volume will never be larger than the current volume,
    /// however.  Near and far are given in terms of zVector(): this
    /// means that near > far must hold.
    SbViewVolume zNarrow(float near, float far) const;

    /// Scales width and height of view volume by given factor
    void scale(float factor);

    /// Scales view volume to be the given ratio of its current width
    /// , leaving the resulting view volume centered about the
    /// same point (in the near plane) as the current one.
    void scaleWidth(float ratio);

    /// Scales view volume to be the given ratio of its current
    /// height, leaving the resulting view volume centered about the
    /// same point (in the near plane) as the current one.
    void scaleHeight(float ratio);

    // Projection type
    enum ProjectionType {
        ORTHOGRAPHIC, ///< Orthographic projection
        PERSPECTIVE   ///< Perspective projection
    };

    /// Returns projection information.
    ProjectionType getProjectionType() const { return type; }

    /// Returns projection information.
    const SbVec3f &getProjectionPoint() const { return projPoint; }

    /// Returns projection information.
    const SbVec3f &getProjectionDirection() const { return projDir; }

    /// Returns distance from projection point to near plane
    float getNearDist() const { return nearDist; }

    /// Returns width of viewing frustum in the projection plane.
    float getWidth() const { return (lrfO - llfO).length(); }

    /// Returns height of viewing frustum in the projection plane.
    float getHeight() const { return (ulfO - llfO).length(); }

    /// Returns depth of viewing frustum, i.e. the distance from the near
    /// clipping plane to the far clipping plane.
    float getDepth() const { return nearToFar; }

    SoINTERNAL
  public:
    ProjectionType type;

    // Note that there is redundant info in this data structure and its
    // elements should not be changed by hand.
    SbVec3f projPoint; // must be (0,0,0) for ortho
    SbVec3f projDir;
    float   nearDist;  // distance to near plane
    float   nearToFar; // distance between z clips
    SbVec3f llf;
    SbVec3f lrf;
    SbVec3f ulf;

    // Transforms the view volume by the given matrix. NOTE: if the
    // matrix performs a scale and a rotation, angles between the
    // transformed projection direction and the sides of the view
    // volume may not be preserved.
    void transform(const SbMatrix &matrix);

    // Returns TRUE if view volume contains point
    SbBool intersect(const SbVec3f &point) const;

    // Returns TRUE if line segment between 2 points may intersect
    // volume. Returns closest point on line to center ray of volume
    // if intersection is found.
    SbBool intersect(const SbVec3f &p0, const SbVec3f &p1,
                     SbVec3f &closestPoint) const;

    // Returns TRUE if bounding box may intersect volume
    SbBool intersect(const SbBox3f &box) const;

    // Returns TRUE if the bounding box defined by min,max is totally
    // outside plane p.
    SbBool outsideTest(const SbPlane &p, const SbVec3f &min,
                       const SbVec3f &max) const;

  private:
    // Points on the near clipping plane.  Add in the projPoint to
    // figure out where they are in world space:
    SbVec3f llfO; // x = -w, y = -w, z = -w
    SbVec3f lrfO; // x =  w, y = -w, z = -w
    SbVec3f ulfO; // x = -w, y =  w, z = -w
};

#endif /* _SB_VIEW_VOLUME_ */
