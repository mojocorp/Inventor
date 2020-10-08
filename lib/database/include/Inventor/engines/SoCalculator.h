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
 |	This file contains the declaration of the Calculator engine
 |
 |   Classes:
 |	SoCalculator
 |
 |   Author(s)		: Ronen Barzel
 |
 ______________  S I L I C O N   G R A P H I C S   I N C .  ____________
 _______________________________________________________________________
 */

#pragma once

#include <Inventor/engines/SoSubEngine.h>
#include <Inventor/fields/SoMFFloat.h>
#include <Inventor/fields/SoMFString.h>
#include <Inventor/fields/SoMFVec3f.h>

class SoCalcParser;
/// A general-purpose calculator.
/// \ingroup Engines
/// This engine is a general-purpose calculator.
/// The calculator operates on floating-point values and 3D
/// floating-point vectors.
/// The engine takes up to eight inputs of each type
/// (<tt>SoMFFloat</tt> and <tt>SoMFVec3f</tt>),
/// and produces up to four outputs of each type.
///
///
/// Each input field (#a-#h, #A-#H) can have multiple values,
/// allowing the engine to
/// evaluate the expression with different values in parallel.
/// Some inputs may have more values than others.  In such cases,
/// the last value of the shorter inputs will be repeated as necessary.
///
///
/// The #expression input string specifies the expression to be evaluated.
/// An expression can consist of multiple subexpressions.
/// Several subexpressions can be specified in one string,
/// separated by semicolons (;).
/// Alternatively, the subexpressions can be stored in separate strings
/// in the multiple-valued input field.
///
///
/// Each subexpression is of the form:
///
/// \<lhs\> = \<rhs\>
///
/// The <b>\<lhs\></b> can be any one of the outputs or a temporary variable.
/// The engine provides 8 temporary floating-point variables
/// (ta, tb, tc, td, te, tf, tg, and th),
/// and 8 temporary vector variables (tA, tB, tC, tD, tE, tF, tG, and tH).
/// You can assign a value to one component of a vector output (#A-#H)
/// or a vector variable (tA-tH) by using the [\|] operator.
/// For example, oA[0] = \<rhs\>, will evaluate the right hand side and assign
/// the value to the first component of the output vector #oA.
///
///
/// The <b>\<rhs\></b> supports arithmetic, logical and conditional operators.
/// They are:
/// \code
/// (unary)   !, -
/// (binary)  +, -, *, /, %, <, > <=, >=, ==, !=, &&, ||
/// (ternary) ? :
/// \endcode
/// The ternary operator is a conditional operator.  For example,
/// <b>a ? b : c</b> evaluates to b if a != 0, and to c if a==0.
///
///
/// Valid operands for the <b>\<rhs\></b> include the inputs, outputs, temporary
/// variables, and their components (e.g. oA[0]).
/// Operands can also be numeric constants (e.g. 1.0),
/// pre-defined named constants, or pre-defined functions.
///
///
/// The named constants are:
/// \code
/// MAXFLOAT
/// MINFLOAT
/// M_E
/// M_LOG2E
/// M_LOG10E
/// M_LN2
/// M_LN10
/// M_PI
/// M_SQRT2  = sqrt(2)
/// M_SQRT1_2 = sqrt(1/2)
/// \endcode
/// Most of the pre-defined functions come from the math library:
/// \code
/// cos, sin, tan,
/// acos, asin, atan, atan2,
/// cosh, sinh, tanh,
/// sqrt, pow, exp, log, log10,
/// ceil, floor, fabs, fmod.
/// \endcode
/// Other functions are defined by <tt>SoCalculator</tt>. They are:
/// \code
/// rand(f) - Random number generator
/// cross(v1, v2) - Vector cross product
/// dot(v1, v2) - Vector dot product
/// length(v) - Vector length
/// normalize(v) - Normalize vector
/// vec3f(f1, f2, f3) - Generate a vector from 3 floats
/// \endcode
/// The subexpressions are evaluated in order, so a variable
/// set in the \<lhs\> of an earlier expression may be used in the
/// \<rhs\> of a later expression.
///
///
/// Note, when the input has multiple values, all the
/// subexpressions specified in the #expression are applied
/// to all the multiple input values.
/// This is unlike the <tt>SoBoolOperation</tt> engine, where each
/// operation is applied only to the corresponding entries
/// of the input data.
/// Note also, that even though the inputs and outputs can have multiple
/// values the [\|] operator is only for indexing into the values of a
/// single vector.
/// It does not index into the multiple values of a field.
/// For example, if the floating-point input field #a has two values:
/// 1.0, and 2.0, then the expression
/// \code
/// "oA[0]=a; oA[1]=a; oA[2]=0.0"
/// \endcode
/// will produce two output vectors in #oA: (1.0, 1.0, 0.0) and
/// (2.0, 2.0, 0.0).
///
///
/// Examples of expressions:
/// \code
/// "ta = oA[0]*floor(a)"
/// "tb = (a+b)*sin(M_PI)"
/// "oA = vec3f(ta, tb, ta+tb)"
/// "oB = normalize(oA)"
/// "ta = a; tb = sin(ta); oA = vec3f(ta, tb, 0)"
/// \endcode
/// \par File format/defaults:
/// \code
/// SoCalculator {
///    a            0
///    b            0
///    c            0
///    d            0
///    e            0
///    f            0
///    g            0
///    h            0
///    A            0 0 0
///    B            0 0 0
///    C            0 0 0
///    D            0 0 0
///    E            0 0 0
///    F            0 0 0
///    G            0 0 0
///    H            0 0 0
///    expression   ""
/// }
/// \endcode
/// \sa SoEngineOutput, SoBoolOperation
class SoCalculator : public SoEngine {

    SO_ENGINE_HEADER(SoCalculator);

  public:
    // Inputs
    SoMFFloat  a, b, c, d, e, f, g, h;
    SoMFVec3f  A, B, C, D, E, F, G, H;
    SoMFString expression;

    // Outputs
    SoEngineOutput oa, ob, oc, od; // (SoMFfloat)
    SoEngineOutput oA, oB, oC, oD; // (SoMFVec3f)

    // Constructor
    SoCalculator();

    SoINTERNAL
  public:
    static void initClass();

  protected:
    virtual void inputChanged(SoField *whichInput);

  private:
    // Destructor
    ~SoCalculator();

    // Evaluation method
    virtual void evaluate();

    SoCalcParser *parser;
    SbBool        reparse;

    // working storage for the evaluation
    float           va, vb, vc, vd, ve, vf, vg, vh, ova, ovb, ovc, ovd;
    float           ta, tb, tc, td, te, tf, tg, th;
    SbVec3f         vA, vB, vC, vD, vE, vF, vG, vH, ovA, ovB, ovC, ovD;
    SbVec3f         tA, tB, tC, tD, tE, tF, tG, tH;
    static float *  lookupFloat(void *, const char *);
    static SbVec3f *lookupVec3f(void *, const char *);
};

