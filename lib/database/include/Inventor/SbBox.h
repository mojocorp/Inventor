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
 |	This file contains definitions of SbBoxes, 2D/3D boxes. A
 |	box has planes parallel to the major axes and can therefore
 |	be specified by two points on a diagonal.  The points with minimum
 |	and maximum x, y, and z coordinates are used.
 |
 |   Author(s)		: Paul S. Strauss, Nick Thompson, David Mott
 |
 ______________  S I L I C O N   G R A P H I C S   I N C .  ____________
 _______________________________________________________________________
 */

#pragma once

#include <Inventor/SbVec.h>
#include <Inventor/SbPlane.h>
#include <Inventor/SbMatrix.h>

/// 3D box class.
/// \ingroup Basics
/// 3D box which has planes parallel to the major axes and is specified by two
/// points on a diagonal. This class is part of the standard Inventor
/// datatype classes and is used as input and output to geometry operations (see
/// <tt>SoGetBoundingBoxAction</tt>).
/// \sa SbXfBox3f, SbBox2f, SbBox2s, SbVec3f, SbVec2f, SbVec2s, SbMatrix,
/// SoGetBoundingBoxAction
class SbBox3f {
  public:
    /// Default constructor - leaves box totally empty
    SbBox3f() { makeEmpty(); }

    /// Constructor given bounds
    SbBox3f(float xmin, float ymin, float zmin, float xmax, float ymax,
            float zmax) {
        min.setValue(xmin, ymin, zmin);
        max.setValue(xmax, ymax, zmax);
    }

    /// Constructor given minimum and maximum points
    SbBox3f(const SbVec3f &_min, const SbVec3f &_max) {
        min = _min;
        max = _max;
    }

    /// Destructor
    ~SbBox3f() {}

    /// Returns the minimum point of the box.  The minimum point is
    /// the corner of the box with the lowest X, Y, and Z values.
    const SbVec3f &getMin() const { return min; }
    /// Returns the maximum point of the box.  The maximum
    /// point is the corner of the box with the highest X, Y, and Z values.
    const SbVec3f &getMax() const { return max; }
    /// Returns the minimum point of the box.  The minimum point is
    /// the corner of the box with the lowest X, Y, and Z values.
    SbVec3f &getMin() { return min; }
    /// Returns the maximum point of the box.  The maximum
    /// point is the corner of the box with the highest X, Y, and Z values.
    SbVec3f &getMax() { return max; }

    /// Returns the center of a box
    SbVec3f getCenter() const;

    /// Extends Box3f (if necessary) to contain given 3D point
    void extendBy(const SbVec3f &pt);

    /// Extends Box3f (if necessary) to contain given Box3f
    void extendBy(const SbBox3f &bb);

    /// Returns TRUE if intersection of given point and Box3f is not empty
    SbBool intersect(const SbVec3f &pt) const;

    /// Returns TRUE if intersection of given Box3f and Box3f is not empty
    SbBool intersect(const SbBox3f &bb) const;

    /// Returns true if intersection of given Plane and Box3f is not empty
    SbBool intersect(const SbPlane &plane) const;

    /// Returns TRUE if bounding box is completely outside the
    /// view-volume defined by the model+view+projection matrix given.
    /// "cullBits" keeps track of which view-volume clipping planes the
    /// box is completely inside of; if a 'parent' bounding box of this
    /// bounding box was found to be completely inside the left/right
    /// clipping planes, the low bit of cullBits will be set to zero,
    /// and this routine will not test this box against those planes.
    /// When cullBits is zero, it is a waste of time to call this
    /// function!  Before calling this the first time, set cullBits to
    /// 7 (SoGLRenderAction and SoSeparator will handle this
    /// automatically for render culling).
    SbBool outside(const SbMatrix &MVP, int &cullBits) const;

    /// Sets the corners of the box.
    void setBounds(float xmin, float ymin, float zmin, float xmax, float ymax,
                   float zmax) {
        min.setValue(xmin, ymin, zmin);
        max.setValue(xmax, ymax, zmax);
    }

    /// Sets the corners of the box.
    void setBounds(const SbVec3f &_min, const SbVec3f &_max) {
        min = _min;
        max = _max;
    }

    /// Gets the corners of the box.
    void getBounds(float &xmin, float &ymin, float &zmin, float &xmax,
                   float &ymax, float &zmax) const {
        min.getValue(xmin, ymin, zmin);
        max.getValue(xmax, ymax, zmax);
    }

    /// Gets the corners of the box.
    void getBounds(SbVec3f &_min, SbVec3f &_max) const {
        _min = min;
        _max = max;
    }

    /// Returns the closest point on the box to the given point.
    /// (Returns the point on the center of the Z face if passed the center.)
    SbVec3f getClosestPoint(const SbVec3f &point);

    /// Returns origin (minimum point) of box
    void getOrigin(float &originX, float &originY, float &originZ) const {
        originX = min[0];
        originY = min[1];
        originZ = min[2];
    }

    /// Returns size of box
    void getSize(float &sizeX, float &sizeY, float &sizeZ) const {
        sizeX = max[0] - min[0];
        sizeY = max[1] - min[1];
        sizeZ = max[2] - min[2];
    }

    /// Returns size of box
    SbVec3f getSize() const { return max - min; }

    /// Sets Box3f to contain nothing
    void makeEmpty();

    /// Checks if the box is empty (degenerate)
    /// note that this relies on boxes being completely degenerate if
    /// they are degenerate at all.  All member functions preserve this
    /// invariant.
    SbBool isEmpty() const { return max[0] < min[0]; }

    /// Checks if the box has volume; i.e., all three dimensions have
    /// positive size
    SbBool hasVolume() const {
        return (max[0] > min[0] && max[1] > min[1] && max[2] > min[2]);
    }

    /// Finds the span of a box along a specified direction.  The span is
    /// the total distance the box occupies along a given direction.  The total
    /// distance is returned in the form of a minimum and maximum distance from
    /// the origin of each of the corners of the box along the given direction.
    /// The difference between these two values is the span.
    void getSpan(const SbVec3f &direction, float &dMin, float &dMax) const;

    /// Transforms Box3f by matrix, enlarging Box3f to contain result
    void transform(const SbMatrix &m);

    /// Gives the volume of the box (0 for an empty box)
    float getVolume() const;

    /// Equality comparisons
    friend int operator==(const SbBox3f &b1, const SbBox3f &b2);
    friend int operator!=(const SbBox3f &b1, const SbBox3f &b2) {
        return !(b1 == b2);
    }

  private:
    // Minimum and maximum points
    SbVec3f min, max;
};

/// 3D box with an associated transformation matrix.
/// \ingroup Basics
/// A 3D box with an arbitrary transformation applied.  This class is useful
/// when a box will be transformed frequently; if an <tt>SbBox3f</tt> is used
/// for this purpose it will expand each time it is transformed in order to keep
/// itself axis-aligned.  Transformations can be accumulated on an
/// <tt>SbXfBox3f</tt> without expanding the box, and after all transformations
/// have been done, the box can be expanded to an axis-aligned box if necessary.
/// \sa SbBox3f, SbBox2f, SbBox2s, SbVec3f, SbVec2f, SbVec2s,     SbMatrix,
/// SoGetBoundingBoxAction
class SbXfBox3f : private SbBox3f {
  public:
    /// Default constructor - leaves box totally empty
    SbXfBox3f();

    /// Constructor given minimum and maximum points
    SbXfBox3f(const SbVec3f &_min, const SbVec3f &_max);

    /// Constructor given Box3f
    SbXfBox3f(const SbBox3f &box);

    /// Destructor
    ~SbXfBox3f() {}

    /// Set the transformation on the box
    void setTransform(const SbMatrix &m);

    /// Get the transformation on the box.
    const SbMatrix &getTransform() const { return xform; }

    /// Get the inverse transformation on the box.
    const SbMatrix &getInverse() const { return xformInv; }

    /// Returns the center of the box
    SbVec3f getCenter() const;

    /// Extends XfBox3f (if necessary) to contain given 3D point.
    /// If the box has had a non-identity transformation applied using the
    /// setTransform() method, the point is assumed to be in the transformed
    /// space.  For example, the following code sequence:
    ///
    /// SbXfBox3f bbox; bbox.extendBy(SbVec3f(0,0,0));
    /// SbMatrix trans; trans.setTranslate(SbVec3f(1,1,1));
    /// bbox.setTransform(trans);
    /// bbox.extendBy(SbVec3f(0,0,0));
    ///
    /// will result in a bounding box extending from (-1,-1,-1) to
    /// (0,0,0) in bbox'es local (untransformed) space.
    void extendBy(const SbVec3f &pt);

    /// Extends XfBox3f (if necessary) to contain given Box3f.
    /// If the box has had a non-identity transformation applied using the
    /// setTransform() method, the given SbBox3f is assumed to be in the
    /// transformed space.
    void extendBy(const SbBox3f &bb) { extendBy(SbXfBox3f(bb)); }

    /// Extends XfBox3f (if necessary) to contain given XfBox3f
    void extendBy(const SbXfBox3f &bb);

    /// Returns TRUE if intersection of given point and Box3f is not empty
    SbBool intersect(const SbVec3f &pt) const;

    /// Returns TRUE if intersection of given XfBox3f and Box3f is not empty
    SbBool intersect(const SbBox3f &bb) const {
        return project().intersect(bb);
    }

    /// Set the bounds of the box.
    void setBounds(float xmin, float ymin, float zmin, float xmax, float ymax,
                   float zmax) {
        SbBox3f::setBounds(xmin, ymin, zmin, xmax, ymax, zmax);
    }

    /// Set the bounds of the box.
    void setBounds(const SbVec3f &_min, const SbVec3f &_max) {
        SbBox3f::setBounds(_min, _max);
    }

    /// Get the bounds of the box.
    void getBounds(float &xmin, float &ymin, float &zmin, float &xmax,
                   float &ymax, float &zmax) const {
        SbBox3f::getBounds(xmin, ymin, zmin, xmax, ymax, zmax);
    }

    /// Get the bounds of the box.
    void getBounds(SbVec3f &_min, SbVec3f &_max) const {
        SbBox3f::getBounds(_min, _max);
    }

    /// Returns origin (minimum point) of box
    void getOrigin(float &originX, float &originY, float &originZ) {
        SbBox3f::getOrigin(originX, originY, originZ);
    }

    /// Returns size of box
    void getSize(float &sizeX, float &sizeY, float &sizeZ) {
        SbBox3f::getSize(sizeX, sizeY, sizeZ);
    }

    /// Returns size of box
    SbVec3f getSize() const { return SbBox3f::getSize(); }

    /// Gives the volume of the box (0 for an empty box)
    float getVolume() const;

    /// Sets Box3f to contain nothing
    void makeEmpty() { SbBox3f::makeEmpty(); }

    /// Checks if the box is empty (degenerate)
    /// note that this relies on boxes being completely degenerate if
    /// they are degenerate at all.  All member functions preserve this
    /// invariant.
    SbBool isEmpty() const { return SbBox3f::isEmpty(); }

    /// Checks if the box has volume; i.e., all three dimensions have
    /// positive size
    SbBool hasVolume() const { return SbBox3f::hasVolume(); }

    /// Finds the extent of a box along a particular direction
    void getSpan(const SbVec3f &direction, float &dMin, float &dMax) const {
        project().getSpan(direction, dMin, dMax);
    }

    /// Transforms Box3f by matrix
    void transform(const SbMatrix &m);

    /// Projects an SbXfBox3f to an SbBox3f
    SbBox3f project() const;

    /// Equality comparisons
    friend int operator==(const SbXfBox3f &b1, const SbXfBox3f &b2);
    friend int operator!=(const SbXfBox3f &b1, const SbXfBox3f &b2) {
        return !(b1 == b2);
    }

  private:
    // These are incorrect for SbXfBox3f, so we hide them
    const SbVec3f &getMin() const { return SbBox3f::getMin(); }
    const SbVec3f &getMax() const { return SbBox3f::getMax(); }

    // The box is transformed by this xform
    SbMatrix xform;
    SbMatrix xformInv;
};

/// 2D box class.
/// \ingroup Basics
/// 2D box which has planes parallel to the major axes and is specified by
/// two points (specified as floating point) on a diagonal. This class is part
/// of the standard Inventor datatype classes and is used as input and output
/// to geometry operations.
/// \sa SbBox3f, SbXfBox3f, SbBox2s, SbVec3f, SbVec2f, SbVec2s, SbMatrix
class SbBox2f {
  public:
    /// Default constructor - leaves box totally empty
    SbBox2f() { makeEmpty(); };

    /// Constructor given bounds
    /// \a xmin, \a ymin, \a xmax, and \a ymax are the bounds of the box.
    SbBox2f(float xmin, float ymin, float xmax, float ymax) {
        min.setValue(xmin, ymin);
        max.setValue(xmax, ymax);
    }

    /// Constructor given minimum and maximum points
    /// \a min and \a max are the corners of the diagonal that define the box.
    SbBox2f(const SbVec2f &_min, const SbVec2f &_max) {
        min = _min;
        max = _max;
    }

    /// Destructor
    ~SbBox2f() {}

    /// Returns the minimum point of the box.  The minimum point is
    /// the corner of the box with the lowest X, Y, and Z values.
    const SbVec2f &getMin() const { return min; }

    /// Returns the maximum point of the box.  The maximum
    /// point is the corner of the box with the highest X, Y, and Z values.
    const SbVec2f &getMax() const { return max; }

    /// Returns the center of the box
    SbVec2f getCenter() const;

    /// Extends Box2f (if necessary) to contain given 2D point
    void extendBy(const SbVec2f &pt);

    /// Extends Box2f (if necessary) to contain given Box2f
    void extendBy(const SbBox2f &r);

    /// Returns TRUE if intersection of given point and Box2f is not empty
    SbBool intersect(const SbVec2f &pt) const;

    /// Returns TRUE if intersection of given Box2f and Box2f is not empty
    SbBool intersect(const SbBox2f &bb) const;

    /// Sets the corners of the box.
    void setBounds(float xmin, float ymin, float xmax, float ymax) {
        min.setValue(xmin, ymin);
        max.setValue(xmax, ymax);
    }

    /// Sets the corners of the box.
    void setBounds(const SbVec2f &_min, const SbVec2f &_max) {
        min = _min;
        max = _max;
    }

    /// Gets the corners of the box.
    void getBounds(float &xmin, float &ymin, float &xmax, float &ymax) const {
        min.getValue(xmin, ymin);
        max.getValue(xmax, ymax);
    }

    /// Gets the corners of the box.
    void getBounds(SbVec2f &_min, SbVec2f &_max) const {
        _min = min;
        _max = max;
    }

    /// Returns the closest point on the box to the given point.
    /// (Returns the point on the center of the X side if passed the center.)
    SbVec2f getClosestPoint(const SbVec2f &point);

    /// Returns origin (minimum point) of box
    void getOrigin(float &originX, float &originY) const {
        originX = min[0];
        originY = min[1];
    }

    /// Returns size of box
    void getSize(float &sizeX, float &sizeY) const {
        sizeX = max[0] - min[0];
        sizeY = max[1] - min[1];
    }

    /// Returns size of box
    SbVec2f getSize() const { return max - min; }

    /// Returns aspect ratio (ratio of width to height) of box
    float getAspectRatio() const {
        return (max[0] - min[0]) / (max[1] - min[1]);
    }

    /// Sets box to contain nothing
    void makeEmpty();

    /// Checks if the box is empty (degenerate)
    /// note that this relies on boxes being completely degenerate if
    /// they are degenerate at all.  All member functions preserve this
    /// invariant.
    SbBool isEmpty() const { return max[0] < min[0]; }

    /// Checks if the box has area; i.e., both dimensions have positive size
    SbBool hasArea() const { return (max[0] > min[0] && max[1] > min[1]); }

    /// Equality comparison
    friend int operator==(const SbBox2f &b1, const SbBox2f &b2);

    /// Inequality comparison
    friend int operator!=(const SbBox2f &b1, const SbBox2f &b2) {
        return !(b1 == b2);
    }

  private:
    // Minimum and maximum points
    SbVec2f min, max;
};

/// 2D box class.
/// \ingroup Basics
/// 2D box which has planes parallel to the major axes and is specified by two
/// points (specified with short integers) on a diagonal. This class is part of
/// the standard Inventor datatype classes and is used as input and output to
/// geometry operations.
/// \sa SbBox3f, SbXfBox3f, SbBox2f, SbVec3f, SbVec2f, SbVec2s, SbMatrix
class SbBox2s {
  public:
    /// Default constructor - leaves box totally empty
    SbBox2s() { makeEmpty(); };

    /// Constructor given bounds.
    /// \a xmin, \a ymin, \a xmax, and \a ymax are the bounds of the box.
    SbBox2s(short xmin, short ymin, short xmax, short ymax) {
        min.setValue(xmin, ymin);
        max.setValue(xmax, ymax);
    }

    /// Constructor given minimum and maximum points.
    /// \a min and \a max are the corners of the diagonal that define the box.
    SbBox2s(const SbVec2s &_min, const SbVec2s &_max) {
        min = _min;
        max = _max;
    }

    /// Destructor
    ~SbBox2s() {}

    /// Returns the minimum point of the box.  The minimum point is
    /// the corner of the box with the lowest X, Y, and Z values.
    const SbVec2s &getMin() const { return min; }

    /// Returns the maximum point of the box.  The maximum
    /// point is the corner of the box with the highest X, Y, and Z values.
    const SbVec2s &getMax() const { return max; }

    /// Extends Box2s (if necessary) to contain given 2D point
    void extendBy(const SbVec2s &pt);

    /// Extends Box2s (if necessary) to contain given Box2s
    void extendBy(const SbBox2s &r);

    /// Returns TRUE if intersection of given point and Box2s is not empty
    SbBool intersect(const SbVec2s &pt) const;

    /// Returns TRUE if intersection of given Box2s and Box2s is not empty
    SbBool intersect(const SbBox2s &bb) const;

    /// Sets the corners of the box.
    void setBounds(short xmin, short ymin, short xmax, short ymax) {
        min.setValue(xmin, ymin);
        max.setValue(xmax, ymax);
    }

    /// Sets the corners of the box.
    void setBounds(const SbVec2s &_min, const SbVec2s &_max) {
        min = _min;
        max = _max;
    }

    /// Gets the corners of the box.
    void getBounds(short &xmin, short &ymin, short &xmax, short &ymax) const {
        min.getValue(xmin, ymin);
        max.getValue(xmax, ymax);
    }

    /// Gets the corners of the box.
    void getBounds(SbVec2s &_min, SbVec2s &_max) const {
        _min = min;
        _max = max;
    }

    /// Returns origin (minimum point) of box
    void getOrigin(short &originX, short &originY) const {
        originX = min[0];
        originY = min[1];
    }

    /// Returns size of box
    void getSize(short &sizeX, short &sizeY) const {
        sizeX = max[0] - min[0];
        sizeY = max[1] - min[1];
    }

    /// Returns size of box
    SbVec2s getSize() const { return max - min; }

    /// Returns aspect ratio (ratio of width to height) of box
    float getAspectRatio() const {
        return float(max[0] - min[0]) / float(max[1] - min[1]);
    }

    /// Sets box to contain nothing
    void makeEmpty();

    /// Checks if the box is empty (degenerate)
    /// note that this relies on boxes being completely degenerate if
    /// they are degenerate at all.  All member functions preserve this
    /// invariant.
    SbBool isEmpty() const { return max[0] < min[0]; }

    /// Equality comparisons
    friend int operator==(const SbBox2s &b1, const SbBox2s &b2);

    /// Inequality comparison
    friend int operator!=(const SbBox2s &b1, const SbBox2s &b2) {
        return !(b1 == b2);
    }

  private:
    // Minimum and maximum points
    SbVec2s min, max;
};

