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
 | This file defines the base SoEngine class, and the
 |      SoEngineOutput class.
 |
 |   Author(s)  : Gavin Bell
 |
 ______________  S I L I C O N   G R A P H I C S   I N C .  ____________
 _______________________________________________________________________
 */

#ifndef  _SO_ENGINE_
#define  _SO_ENGINE_

#include <Inventor/fields/SoFieldContainer.h>
#include <Inventor/engines/SoEngineOutput.h>
#include <Inventor/lists/SoEngineList.h>
#include <Inventor/lists/SoEngineOutputList.h>
#include <Inventor/SbString.h>
#include <Inventor/SoType.h>

class SoFieldData;
class SoEngineOutput;
class SoEngineOutputData;
class SoField;
class SoOutput;
class SoInput;

/// Base class for all engines.
/// \ingroup Engines
/// <tt>SoEngine</tt> is the abstract base class for all engines.
/// Engines are objects used for animation and behavior.
/// They are lightweight objects that are connected between
/// nodes, the clock, and other engines to form interesting
/// behaviorial objects (e.g., a spinning windmill).
///
///
/// Engines are used to animate parts of a scene and/or to constrain
/// one part of a scene in relation to some other part of the scene.
/// An engine receives a number of input values, performs some operation
/// on them, and then copies the results into one or more
/// output fields.  Both the inputs and the outputs can be connected
/// to other fields or engines in the scene graph.  When an engine's
/// output values change, those new values are sent to any fields or
/// engines connected to them.
/// \par File format/defaults:
/// This is an abstract class. See the reference page of a derived class for the format and default values.
/// \sa SoBoolOperation, SoCalculator, SoComposeMatrix, SoComposeRotation, SoComposeRotationFromTo,SoComposeVec2f,
/// \sa SoComposeVec3f, SoComposeVec4f, SoComputeBoundingBox, SoConcatenate, SoCounter, SoDecomposeMatrix,
/// \sa SoDecomposeRotation, SoDecomposeVec2f, SoDecomposeVec3f,SoDecomposeVec4f, SoElapsedTime, SoGate,
/// \sa SoInterpolate, SoOnOff, SoOneShot, SoSelectOne,SoTimeCounter, SoTransformVec3f, SoTriggerAny
class INVENTOR_API SoEngine : public SoFieldContainer {

public:

    /// Returns the type identifier for the SoEngine class.
    static SoType getClassTypeId() {
        return classTypeId;
    }

    /// Returns a list of outputs in this engine.
    /// Use #getOutputName() to get the names of the outputs,
    /// and use #SoEngineOutput::getConnectionType() to determine their types.
    virtual int getOutputs(SoEngineOutputList &list) const;

    /// Returns a pointer to the engine output with the given name.
    /// If no such output exists, NULL is returned.
    SoEngineOutput * getOutput(const SbName &outputName) const;

    /// Returns (in \a outputName) the name of the engine
    /// output (\a output).  Returns FALSE if the engine output is
    /// not contained within the engine instance.
    SbBool getOutputName(const SoEngineOutput *output,
                         SbName &outputName) const;

    /// Creates and returns an exact copy of the engine.
    /// All connections to inputs are copied as is (without
    /// copying what's at the other end).
    SoEngine * copy() const;

    /// \name Look up engine(s) by name.
    /// A engine can be given a name using setName() (which is a
    /// method on SoBase).  These methods allow enginess to be looked
    /// up by name.
    /// @{
    static SoEngine * getByName(const SbName &name);
    static int getByName(const SbName &name, SoEngineList &list);
    /// @}
protected:

    // User-written evaluation engine.
    virtual void evaluate() = 0;

SoINTERNAL public:

    // wrapper for the user's evaluate() engine
    void evaluateWrapper();

    // Propagates modification notification through an instance.
    virtual void notify(SoNotList *list);

    // Initializes base engine class
    static void initClass();

    // Info on outputs:
    virtual const SoEngineOutputData *getOutputData() const = 0;

    // Initialize ALL Inventor engine classes
    static void initClasses();
    static void finishClasses();


    // Writes instance to SoOutput. (Used only for last stage of writing)
    virtual void writeInstance(SoOutput *out);

    // Copies an instance that is encountered through a field connection
    virtual SoFieldContainer * copyThroughConnection() const;

    // Recursive procedure that determines if this engine should be
    // copied during a copy operation, or just referenced as is
    SbBool shouldCopy() const;

    // A very annoying double notification occurs with engines
    // that enable their outputs during inputChanged; this flag
    // prevents that:
    SbBool isNotifying() const {
        return notifying;
    }

protected:

    // Constructor, destructor
    SoEngine();
    ~SoEngine();

    // Reads stuff into instance. Returns FALSE on error
    virtual SbBool readInstance(SoInput *in, unsigned short flags);

    // This is called whenever the value of an input is changed. The
    // default method does nothing. Subclasses can override this to
    // detect when a specific field is changed.
    virtual void inputChanged(SoField *whichField);

    // This is used by the input&output inheritence mechanism, hidden in
    // the SoSubEngine macros
    static const SoFieldData  **getInputDataPtr()  {
        return NULL;
    }
    static const SoEngineOutputData **getOutputDataPtr() {
        return NULL;
    }

    // Write output type information for engines that are not built-in.
    void writeOutputTypes(SoOutput *out);

private:
    static SoType classTypeId;  // Type identifier

    SbBool  needsEvaluation; // Dirty bit
    SbBool  notifying;
    friend class SoEngineOutput;
};
#endif /* _SO_ENGINE_ */

