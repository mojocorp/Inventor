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
 | This file contains the implementation of the
 |      various composition and decomposition engines
 |
 |   Classes:
 |       SoComposeVec2f
 |       SoComposeVec3f
 |       SoComposeVec4f
 |       SoComposeRotation
 |       SoComposeRotationFromTo
 |       SoDecomposeVec2f
 |       SoDecomposeVec3f
 |       SoDecomposeVec4f
 |       SoDecomposeRotation
 |
 |   Author(s)  : Ronen Barzel
 |
 ______________  S I L I C O N   G R A P H I C S   I N C .  ____________
 _______________________________________________________________________
 */

#ifndef  _SO_COMPOSE_
#define  _SO_COMPOSE_

#include <Inventor/engines/SoSubEngine.h>
#include <Inventor/fields/SoMFFloat.h>
#include <Inventor/fields/SoMFMatrix.h>
#include <Inventor/fields/SoMFRotation.h>
#include <Inventor/fields/SoMFVec2f.h>
#include <Inventor/fields/SoMFVec3f.h>
#include <Inventor/fields/SoMFVec4f.h>

// This internal macro defines the standard methods for the composition
// engines.  These are just basic engine declarations, nothing fancy.
// The macro leaves the class header in "public:" state so it can be
// followed immediately with the composition input and output declarations.
#define SO_COMPOSE__HEADER(Name)  \
     SO_ENGINE_HEADER(Name);  \
   private:    \
     ~Name();    \
     virtual void evaluate();  \
   public:    \
     Name();    \
     static void initClass()

/// Composes 2D vectors from floating-point values.
/// \ingroup Engines
/// This engine takes two floating-point inputs and composes a
/// 2D floating-point vector.
///
///
/// The input fields can have multiple values, allowing the engine to
/// compose several vectors in parallel.
/// One of the inputs may have more values than others.  In such cases,
/// the last value of the shorter input will be repeated as necessary.
///
/// \par File format/defaults:
/// \code
/// SoComposeVec2f {
///    x    0
///    y    0
/// }
/// \endcode
/// \sa SoDecomposeVec2f, SoEngineOutput
class INVENTOR_API SoComposeVec2f : public SoEngine {
    SO_COMPOSE__HEADER(SoComposeVec2f);
    SoMFFloat      x; ///< Input
    SoMFFloat      y; ///< Input
    SoEngineOutput vector; ///< Output (SoMFVec2f)
};


/// Composes 2D vectors from floating-point values.
/// \ingroup Engines
/// This engine takes two floating-point inputs and composes a
/// 2D floating-point vector.
///
///
/// The input fields can have multiple values, allowing the engine to
/// compose several vectors in parallel.
/// One of the inputs may have more values than others.  In such cases,
/// the last value of the shorter input will be repeated as necessary.
///
/// \par File format/defaults:
/// \code
/// SoComposeVec2f {
///    x    0
///    y    0
/// }
/// \endcode
/// \sa SoDecomposeVec2f, SoEngineOutput
class INVENTOR_API SoComposeVec3f : public SoEngine {
    SO_COMPOSE__HEADER(SoComposeVec3f);
    SoMFFloat      x; ///< Input
    SoMFFloat      y; ///< Input
    SoMFFloat      z; ///< Input
    SoEngineOutput vector; ///< Output (SoMFVec3f)
};

/// Composes 4D vectors from floating-point values.
/// \ingroup Engines
/// This engine takes four floating-point inputs and composes a
/// 4D vector.
///
///
/// The input fields can have multiple values, allowing the engine to
/// compose several vectors in parallel.
/// Some inputs may have more values than others.  In such cases,
/// the last value of the shorter inputs will be repeated as necessary.
///
/// \par File format/defaults:
/// \code
/// SoComposeVec4f {
///    x    0
///    y    0
///    z    0
///    w    0
/// }
/// \endcode
/// \sa SoDecomposeVec4f, SoEngineOutput
class INVENTOR_API SoComposeVec4f : public SoEngine {
    SO_COMPOSE__HEADER(SoComposeVec4f);
    SoMFFloat      x; ///< Input
    SoMFFloat      y; ///< Input
    SoMFFloat      z; ///< Input
    SoMFFloat      w; ///< Input
    SoEngineOutput vector; ///< Output (SoMFVec4f)
};

/// Decomposes 2D vectors into floating-point values.
/// \ingroup Engines
/// This engine takes as input a 2D vector,
/// and decomposes it into two single floating-point values.
///
///
/// The input can have multiple values, allowing the engine to decompose
/// several vectors in parallel.
///
/// \par File format/defaults:
/// \code
/// SoDecomposeVec2f {
///    vector 0 0
/// }
/// \endcode
/// \sa SoComposeVec2f, SoEngineOutput
class INVENTOR_API SoDecomposeVec2f : public SoEngine {
    SO_COMPOSE__HEADER(SoDecomposeVec2f);
    SoMFVec2f      vector; // Input
    SoEngineOutput x; // Output (SoMFFloat)
    SoEngineOutput y; // Output (SoMFFloat)
};

/// Decomposes 3D vectors into floating-point values.
/// \ingroup Engines
/// This engine takes as input a 3D vector,
/// and decomposes it into three single floating-point values.
///
///
/// The input can have multiple values, allowing the engine to decompose
/// several vectors in parallel.
///
/// \par File format/defaults:
/// \code
/// SoDecomposeVec3f {
///    vector 0 0 0
/// }
/// \endcode
/// \sa SoComposeVec3f, SoEngineOutput
class INVENTOR_API SoDecomposeVec3f : public SoEngine {
    SO_COMPOSE__HEADER(SoDecomposeVec3f);
    SoMFVec3f      vector; // Input
    SoEngineOutput x; // Output (SoMFFloat)
    SoEngineOutput y; // Output (SoMFFloat)
    SoEngineOutput z; // Output (SoMFFloat)
};

/// Decomposes 4D vectors into floating-point values.
/// \ingroup Engines
/// This engine takes as input a 4D vector,
/// and decomposes it into four single floating-point values.
///
///
/// The input can have multiple values, allowing the engine to decompose
/// several vectors in parallel.
///
/// \par File format/defaults:
/// \code
/// SoDecomposeVec4f {
///    vector 0 0 0 0
/// }
/// \endcode
/// \sa SoComposeVec4f, SoEngineOutput
class INVENTOR_API SoDecomposeVec4f : public SoEngine {
    SO_COMPOSE__HEADER(SoDecomposeVec4f);
    SoMFVec4f      vector; // Input
    SoEngineOutput x; // Output (SoMFFloat)
    SoEngineOutput y; // Output (SoMFFloat)
    SoEngineOutput z; // Output (SoMFFloat)
    SoEngineOutput w; // Output (SoMFFloat)
};

/// Composes a rotation from axis and angle values.
/// \ingroup Engines
/// This engine has two inputs, representing an #axis of rotation
/// and a rotation #angle in radians.  As output, the engine composes the
/// inputs into a rotation field.
///
///
/// The input fields can have multiple values, allowing the engine to
/// compose several rotations in parallel.
/// Some inputs may have more values than others.  In such cases,
/// the last value of the shorter inputs will be repeated as necessary.
///
/// \par File format/defaults:
/// \code
/// SoComposeRotation {
///    axis     0 0 1
///    angle    0
/// }
/// \endcode
/// \sa SoComposeRotationFromTo, SoDecomposeRotation, SoEngineOutput
class INVENTOR_API SoComposeRotation : public SoEngine {
    SO_COMPOSE__HEADER(SoComposeRotation);
    SoMFVec3f      axis;  // Input
    SoMFFloat      angle;  // Input
    SoEngineOutput rotation; // Output (SoMFRotation)
};

/// Composes a rotation that rotates from one vector into another.
/// \ingroup Engines
/// This engine takes two inputs, representing a vector before
/// and after a rotation has been applied.
/// As output, it produces the rotation value that would cause the first
/// vector to transform into the other.
///
///
/// The input fields can have multiple values, allowing the engine to
/// compose several rotations in parallel.
/// Some inputs may have more values than others.  In such cases,
/// the last value of the shorter inputs will be repeated as necessary.
///
/// \par File format/defaults:
/// \code
/// SoComposeRotationFromTo {
///    from     0 0 1
///    to       0 0 1
/// }
/// \endcode
/// \sa SoComposeRotation, SoDecomposeRotationFromTo, SoEngineOutput
class INVENTOR_API SoComposeRotationFromTo : public SoEngine {
    SO_COMPOSE__HEADER(SoComposeRotationFromTo);
    SoMFVec3f      from;  // Input
    SoMFVec3f      to;  // Input
    SoEngineOutput rotation; // Output (SoMFRotation)
};


/// Decomposes rotation values.
/// \ingroup Engines
/// This engine takes as input a rotation, and
/// decomposes it into an axis value and a rotation angle (in radians).
///
///
/// The input can have multiple values, allowing the engine to decompose
/// several rotations in parallel.
///
/// \par File format/defaults:
/// \code
/// SoDecomposeRotation {
///    rotation 0 0 1  0
/// }
/// \endcode
/// \sa SoDecomposeRotationFromTo, SoComposeRotation, SoEngineOutput
class INVENTOR_API SoDecomposeRotation : public SoEngine {
    SO_COMPOSE__HEADER(SoDecomposeRotation);
    SoMFRotation   rotation; // Input
    SoEngineOutput axis;  // Output (SoMFVec3f)
    SoEngineOutput angle;  // Output (SoMFFloat)
};


/// Composes a transformation matrix.
/// \ingroup Engines
/// This engine has inputs that specify values for translation, rotation,
/// scale, and center of transformation.
/// As output, it produces a transformation matrix that
/// transforms objects into the space specified by the scale, rotation,
/// and translation inputs (in that order).
///
///
/// The input fields can have multiple values, allowing the engine to
/// compose several matrices in parallel.
/// Some inputs may have more values than others.  In such cases,
/// the last value of the shorter inputs will be repeated as necessary.
///
/// \par File format/defaults:
/// \code
/// SoComposeMatrix {
///    translation      0 0 0
///    rotation         0 0 1  0
///    scaleFactor      1 1 1
///    scaleOrientation 0 0 1  0
///    center           0 0 0
/// }
/// \endcode
/// \sa SoDecomposeMatrix, SoEngineOutput
class INVENTOR_API SoComposeMatrix : public SoEngine {
    SO_COMPOSE__HEADER(SoComposeMatrix);
    SoMFVec3f      translation; // Input
    SoMFRotation   rotation; // Input
    SoMFVec3f      scaleFactor; // Input
    SoMFRotation   scaleOrientation;//Input
    SoMFVec3f      center;  // Input
    SoEngineOutput matrix;  // Output (SoMFMatrix)
};


/// Decomposes transformation matrices into values for translation, rotation, and scale.
/// \ingroup Engines
/// This engine takes as input a transformation matrix and a center of transformation.
/// As output the engine produces the translation, rotation and scale values
/// derived from the matrix.
///
///
/// The input fields can have multiple values, allowing the engine to
/// decompose several matrices in parallel.
/// One of the inputs may have more values than the other.  In that case,
/// the last value of the shorter input will be repeated as necessary.
///
/// \par File format/defaults:
/// \code
/// SoDecomposeMatrix {
///    matrix   1 0 0 0
///             0 1 0 0
///             0 0 1 0
///             0 0 0 1
///    center   0 0 0
/// }
/// \endcode
/// \sa SoComposeMatrix, SoEngineOutput
class INVENTOR_API SoDecomposeMatrix : public SoEngine {
    SO_COMPOSE__HEADER(SoDecomposeMatrix);
    SoMFMatrix     matrix;  // Input
    SoMFVec3f      center;  // Input
    SoEngineOutput translation; // Output (SoMFVec3f)
    SoEngineOutput rotation; // Output (SoMFRotation)
    SoEngineOutput scaleFactor; // Output (SoMFVec3f)
    SoEngineOutput scaleOrientation;//Output (SoMFRotation)
};

#endif  /* _SO_COMPOSE_ */
