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
 |	This file contains the definition of the SbSphereSectionProjector
 |	class. This projector projects the mouse position onto the
 |	section of a sphere that has been sliced by a plane.
 |
 |	The tolerance slice can be specified as a fraction of the radius
 |	of the sphere. The projection point will not extend beyond
 |	the sliced portion of the sphere.
 |
 |	This projector is good for trackballs that only do
 |	pure roll when the mouse is off the sliced portion of
 |	the sphere.
 |
 |   Author(s)		: Howard Look, Paul Isaacs
 |
 ______________  S I L I C O N   G R A P H I C S   I N C .  ____________
 _______________________________________________________________________
 */

#ifndef _SB_SPHERE_SECTION_PROJECTOR_
#define _SB_SPHERE_SECTION_PROJECTOR_

#include <Inventor/SbPlane.h>
#include <Inventor/projectors/SbSphereProjector.h>

/// Sphere-section projector.
/// \ingroup Projectors
/// <tt>SbSphereSectionProjector</tt>
/// projects a window space point (usually based on the mouse location)
/// onto the section of a sphere that has been sliced by a plane.
/// Two projected points can produce a rotation
/// about the sphere's center.
/// The tolerance slice can be specified as a fraction of the radius
/// of the sphere. The projection point will not extend beyond the
/// sliced portion of the sphere.
///
/// Incremental changes (delta rotation) can be computed during
/// interactive sessions. Sphere projectors are typically used to write
/// interactive 3D manipulators and viewers.
/// \sa
/// SbCylinderProjector,SbCylinderSectionProjector,SbCylinderPlaneProjector,SbCylinderSheetProjector,
/// \sa
/// SbLineProjector,SbPlaneProjector,SbSpherePlaneProjector,SbSphereSheetProjector
class SbSphereSectionProjector : public SbSphereProjector {
  public:
    /// Default constructor.
    /// The default view volume is undefined.
    /// The default working space is identity (world space).
    /// The default sphere to use has a radius of 1.0 and is centered at
    /// (0,0,0). The default edge tolerance is .9. The default eye orientation
    /// is TRUE.
    SbSphereSectionProjector(float edgeTol = .9, SbBool orientToEye = TRUE);

    /// Constructor taking the sphere.
    /// The position of the plane which slices the sphere into
    /// a section is specified as a fraction of the sphere radius
    /// with the parameter \a edgeTol.
    /// A tolerance value of 1.0 positions the plane
    /// down the center of the sphere. A tolerance value of 0.5 defines
    /// the longitudinal plane halfway between the center and the outside
    /// edge of the sphere.  The default value is .9, so that almost
    /// half the sphere is in front of the plane.
    /// The \a orientToEye
    /// parameter determines whether the plane is perpendicular to the
    /// eye, or perpendicular to the sphere's Z axis.  Setting that parameter to
    /// TRUE (the default) specifies that the plane be perpendicular to the eye,
    /// which is most often the desired behavior.
    ///
    /// The default view volume is undefined, and the working space is identity.
    SbSphereSectionProjector(const SbSphere &sph, float edgeTol = .9,
                             SbBool orientToEye = TRUE);

    /// Destructor
    ~SbSphereSectionProjector() {}

    /// Returns an instance that is a copy of this instance. The caller
    /// is responsible for deleting the copy when done.
    virtual SbProjector *copy() const;

    /// Apply the projector using the given point, returning the
    /// point in three dimensions that it projects to.
    /// The point should be normalized from 0-1, with (0,0) at the lower-left.
    virtual SbVec3f project(const SbVec2f &point);

    /// Computes a rotation based on two points on this projector.
    virtual SbRotation getRotation(const SbVec3f &point1,
                                   const SbVec3f &point2);

    /// Set the edge tolerance as a fraction of the
    /// radius of the sphere. If this is 1.0, the projector is a
    /// hemisphere. If this is .1, the projector is a slice of
    /// the sphere with radius .1*radius.  Default is .9.
    void setTolerance(float edgeTol);

    /// Get the edge tolerance as a fraction of the
    /// radius of the sphere. If this is 1.0, the projector is a
    /// hemisphere. If this is .1, the projector is a slice of
    /// the sphere with radius .1*radius.  Default is .9.
    float getTolerance() const { return tolerance; }

    /// Set the radial rotation factor.
    /// When the mouse is dragged off of the edge of the sphere, the mouse
    /// motion can be classified as either tangential (moving in a circle
    /// around the sphere) or radial (moving toward or away from the center).
    /// The tangential motion will always map to a rotation around the center,
    /// (like the hands of a clock).
    /// The radial motion, by default, has no effect. But if you set the
    /// radialFactor to be > 0.0, this motion will make the sphere rotate
    /// as if the mouse is pulling the top of the sphere out toward the
    /// mouse.
    /// If radialFactor = 1.0, then pulling has a 'normal' feel. (that is, the
    /// mouse motion causes the same amount of rotation as if you had rotated
    /// by hitting the actual surface of the sphere, instead of moving off the
    /// Default is 0.0
    void setRadialFactor(float rad = 0.0f) { radialFactor = rad; }

    /// Get the radial rotation factor.
    /// Default is 0.0
    /// \sa setRadialFactor
    float getRadialFactor() const { return radialFactor; }

    /// Find whether this point on the sphere or tolerance
    /// plane is within tolerance.
    SbBool isWithinTolerance(const SbVec3f &point);

  protected:
    // Sets up the tolerance slice.
    virtual void setupTolerance();

    // Information about the slice tolerance.
    float tolerance; // the edge tolerance
    float tolDist;   // dist from planePoint to tolerance slice

    float radialFactor;

    // Information about the plane used for intersection testing.
    SbVec3f planePoint; // point on plane
    SbVec3f planeDir;   // normal direction
    float   planeDist;  // distance from sphere center
    SbPlane tolPlane;   // the plane itself
};

#endif /* _SB_SPHERE_SECTION_PROJECTOR_ */
