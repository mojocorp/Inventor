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
 |        This file defines some macros that implement things common to
 |        many subclasses of SoField (actually, subclasses of SoSField
 |        and SoMField). They may be used to make subclassing easier.
 |        In the macros, the following parameters are used consistently:
 |
 |                className = name of new field subclass
 |
 |                valueType = type of field value (e.g., float, SbVec3f)
 |
 |                valueRef  = type of field value that can be passed or
 |                            returned. For simple types like float or
 |                            int, it is the same as valueType. For
 |                            aggregates such as SbVec3f, use a const
 |                            reference, e.g.: const SbVec3f &
 |
 |   Macros for single-value (SoSField) field subclasses:
 |
 |        Within class header:
 |
 |                SO_SFIELD_REQUIRED_HEADER()
 |                SO_SFIELD_CONSTRUCTOR_HEADER()
 |                SO_SFIELD_VALUE_HEADER()
 |                SO_SFIELD_DERIVED_VALUE_HEADER()
 |
 |                SO_SFIELD_HEADER()
 |                        [includes REQUIRED, CONSTRUCTOR, and VALUE]
 |                SO_SFIELD_DERIVED_HEADER()
 |                        [includes REQUIRED, CONSTRUCTOR, and DERIVED_VALUE]
 |
 |        Within class source:
 |
 |                SO_SFIELD_INIT_CLASS()
 |                SO_SFIELD_REQUIRED_SOURCE()
 |                SO_SFIELD_CONSTRUCTOR_SOURCE()
 |                SO_SFIELD_VALUE_SOURCE()
 |
 |                SO_SFIELD_SOURCE()
 |                        [includes REQUIRED, CONSTRUCTOR, and VALUE]
 |                SO_SFIELD_DERIVED_SOURCE()
 |                        [includes REQUIRED and CONSTRUCTOR]
 |
 |   Macros for multiple-value (SoMField) field subclasses:
 |
 |        Within class header:
 |
 |                SO_MFIELD_REQUIRED_HEADER()
 |                SO_MFIELD_CONSTRUCTOR_HEADER()
 |                SO_MFIELD_VALUE_HEADER()
 |                SO_MFIELD_DERIVED_VALUE_HEADER()
 |
 |                SO_MFIELD_HEADER()
 |                        [includes REQUIRED, CONSTRUCTOR, and VALUE]
 |                SO_MFIELD_DERIVED_HEADER()
 |                        [includes REQUIRED and DERIVED_VALUE ]
 |
 |        Within class source:
 |
 |                SO_MFIELD_INIT_CLASS()
 |                SO_MFIELD_REQUIRED_SOURCE()
 |                SO_MFIELD_CONSTRUCTOR_SOURCE()
 |                SO_MFIELD_DERIVED_CONSTRUCTOR_SOURCE()
 |                SO_MFIELD_VALUE_SOURCE()
 |                SO_MFIELD_ALLOC_SOURCE()
 |                SO_MFIELD_MALLOC_SOURCE()
 |
 |                SO_MFIELD_SOURCE()
 |                        [includes REQUIRED, CONSTRUCTOR, VALUE, and ALLOC]
 |                SO_MFIELD_SOURCE_MALLOC()
 |                        [includes REQUIRED, CONSTRUCTOR, VALUE, and MALLOC]
 |                SO_MFIELD_DERIVED_SOURCE()
 |                        [includes REQUIRED and DERIVED_CONSTRUCTOR]
 |
 |   Author(s)                : Paul S. Strauss
 |
 ______________  S I L I C O N   G R A P H I C S   I N C .  ____________
 _______________________________________________________________________
 */

#ifndef  _SO_SUB_FIELD_
#define  _SO_SUB_FIELD_

#include <Inventor/fields/SoField.h>
#include <Inventor/fields/SoSField.h>
#include <Inventor/fields/SoMField.h>
#include <Inventor/SoInput.h>
#include <Inventor/SoOutput.h>


//===========================================================================
//
// These are all internal macros used by some of the public ones.
//
//===========================================================================

#define SO__FIELD_HEADER(className)                                             \
    public:                                                                     \
    virtual SoType              getTypeId() const;                              \
    static SoType               getClassTypeId() { return classTypeId; }        \
                                                                                \
    /** Copy from another field of same type */                                 \
    const className &                operator =(const className &f);            \
                                                                                \
    SoINTERNAL public:                                                          \
    /** Copy from another field of unknown type (assumed to be same type) */    \
    virtual void                copyFrom(const SoField &f);                     \
                                                                                \
    static void *                createInstance();        /* for SoType  */     \
                                                                                \
    /** Returns TRUE if fields are same type and have same values */            \
    virtual SbBool                isSame(const SoField &f) const;               \
                                                                                \
    private:                                                                    \
    static SoType                classTypeId

#define SO__SFIELD_RW_HEADER(className)                                         \
  private:                                                                      \
    /** Reads value of field from file. */                                      \
    virtual SbBool                readValue(SoInput *in);                       \
                                                                                \
    /** Writes value of field to file. */                                       \
    virtual void                writeValue(SoOutput *out) const

#define SO__MFIELD_RW_HEADER(className)                                         \
  private:                                                                      \
    /** Reads indexed value of field from file. */                              \
    virtual SbBool                read1Value(SoInput *in, int index);           \
                                                                                \
    /** Writes one (indexed) value to file. */                                  \
    virtual void                write1Value(SoOutput *out, int index) const

#define SO__FIELD_INIT_CLASS(className,classPrintName,parentClass)              \
    classTypeId = SoType::createType(parentClass::getClassTypeId(),             \
                                     classPrintName,                            \
                                     &className::createInstance)

#define        SO__FIELD_ID_SOURCE(className)                                   \
                                                                                \
SoType                className::classTypeId;                                   \
                                                                                \
SoType                                                                          \
className::getTypeId() const                                                    \
{                                                                               \
    return classTypeId;                                                         \
}                                                                               \
void *                                                                          \
className::createInstance()                                                     \
{                                                                               \
    return new className;                                                       \
}

#define        SO__FIELD_EQ_SAME_SOURCE(className)                              \
                                                                                \
void                                                                            \
className::copyFrom(const SoField &f)                                           \
{                                                                               \
    *this = * (const className *) &f;                                           \
}                                                                               \
                                                                                \
SbBool                                                                          \
className::isSame(const SoField &f) const                                       \
{                                                                               \
    return (getTypeId() == f.getTypeId() &&                                     \
            (*this) == (const className &) f);                                  \
}

//===========================================================================
//
// Macros to be called within the class definition header for a
// single-value field subclass:
//
//===========================================================================

////////////////////////////////////////////////////////////////////////////
//
// This declares all header info required for a single-value field class.
//
////////////////////////////////////////////////////////////////////////////

#define SO_SFIELD_REQUIRED_HEADER(className)                                    \
    SO__FIELD_HEADER(className)

////////////////////////////////////////////////////////////////////////////
//
// This declares a constructor and destructor for a single-value field class.
//
////////////////////////////////////////////////////////////////////////////

#define SO_SFIELD_CONSTRUCTOR_HEADER(className)                                 \
  public:                                                                       \
    /** Constructor. */                                                         \
    className();                                                                \
    /** Destructor. */                                                          \
    virtual ~className()

////////////////////////////////////////////////////////////////////////////
//
// This declares value processing methods and variables for a
// single-value field class.
//
////////////////////////////////////////////////////////////////////////////

#define SO_SFIELD_VALUE_HEADER(className, valueType, valueRef)                  \
    SO__SFIELD_RW_HEADER(className);                                            \
  public:                                                                       \
    /** Returns this field's value. */                                          \
    valueRef                        getValue() const                            \
        { evaluate(); return value; }                                           \
                                                                                \
    /** Sets this field to \a newValue. */                                      \
    void                        setValue(valueRef newValue);                    \
    /** Sets this field to \a newValue. */                                      \
    valueRef                        operator =(valueRef newValue)               \
        { setValue(newValue); return value; }                                   \
                                                                                \
    /** Equality test for fields of same type */                                \
    int                                operator ==(const className &f) const;   \
    /** Inequality test for fields of same type */                              \
    int                                operator !=(const className &f) const    \
        { return ! ((*this) == f); }                                            \
                                                                                \
  protected:                                                                    \
    valueType                        value

////////////////////////////////////////////////////////////////////////////
//
// This is like the above macro, but can be used by a field class
// derived from another (non-abstract) field class. It will inherit
// most of the field processing stuff.
//
////////////////////////////////////////////////////////////////////////////

#define SO_SFIELD_DERIVED_VALUE_HEADER(className, valueType, valueRef)          \
    SO__SFIELD_RW_HEADER(className);                                            \
  public:                                                                       \
    /* Since = operator is not inherited, we redefine it here */                \
    valueRef                        operator =(valueRef newValue)               \
        { setValue(newValue); return value; }

////////////////////////////////////////////////////////////////////////////
//
// This macro is all that is needed for most single-value field
// classes. It includes the relevant macros.
//
////////////////////////////////////////////////////////////////////////////

#define SO_SFIELD_HEADER(className, valueType, valueRef)                        \
    SO_SFIELD_REQUIRED_HEADER(className);                                       \
    SO_SFIELD_CONSTRUCTOR_HEADER(className);                                    \
    SO_SFIELD_VALUE_HEADER(className, valueType, valueRef)

////////////////////////////////////////////////////////////////////////////
//
// This macro is all that is needed for most single-value field
// classes that are derived from other field classes. It includes the
// relevant macros.
//
////////////////////////////////////////////////////////////////////////////

#define SO_SFIELD_DERIVED_HEADER(className, valueType, valueRef)                \
    SO_SFIELD_REQUIRED_HEADER(className);                                       \
    SO_SFIELD_CONSTRUCTOR_HEADER(className);                                    \
    SO_SFIELD_DERIVED_VALUE_HEADER(className, valueType, valueRef)

//===========================================================================
//
// Macros to be called within the class definition header for a
// multiple-value field subclass:
//
//===========================================================================

////////////////////////////////////////////////////////////////////////////
//
// This declares all header info required for a multiple-value field class.
//
////////////////////////////////////////////////////////////////////////////

#define SO_MFIELD_REQUIRED_HEADER(className)                                    \
    SO__FIELD_HEADER(className)

////////////////////////////////////////////////////////////////////////////
//
// This declares a constructor and destructor for a multiple-value field class.
//
////////////////////////////////////////////////////////////////////////////

#define SO_MFIELD_CONSTRUCTOR_HEADER(className)                                 \
  public:                                                                       \
    /** Constructor. */                                                         \
    className();                                                                \
    /** Destructor */                                                           \
    virtual ~className()

////////////////////////////////////////////////////////////////////////////
//
// This declares value processing methods and variables for a
// multiple-value field class.
//
////////////////////////////////////////////////////////////////////////////

#define SO_MFIELD_VALUE_HEADER(className, valueType, valueRef)                                             \
    SO__MFIELD_RW_HEADER(className);                                                                       \
  public:                                                                                                  \
    /** Returns the \a i 'th value of the field.  Indexing past the end of the                          */ \
    /** field (passing in \a i greater than #getNum()) will return garbage.                             */ \
    valueRef                        operator [](int i) const                                               \
        { evaluate(); return values[i]; }                                                                  \
                                                                                                           \
    /** Returns a pointer into the array of values in the field, starting at index \a start.            */ \
    /** The values are read-only; see the #startEditing() and #finishEditing() methods for a way of     */ \
    /** modifying values in-place.                                                                      */ \
    const valueType *                getValues(int start) const                                            \
        { evaluate(); return (const valueType *)(values + start); }                                        \
                                                                                                           \
    /** Finds the given value in the array and returns the index of that value in the array.            */ \
    /** If the value is not found, -1 is returned.                                                      */ \
    /** If \a addIfNotFound is set, then targetValue will be added to the end of the array              */ \
    /** (but -1 is still returned).                                                                     */ \
    int                                find(valueRef targetValue,                                          \
                                     SbBool addIfNotFound = FALSE);                                        \
                                                                                                           \
    /** Sets \a num values starting at index \a start to the values in \a newValues.                    */ \
    /** The array will be automatically be made larger to accomodate the new values, if necessary.      */ \
    void                        setValues(int start, int num,                                              \
                                          const valueType *newValues);                                     \
                                                                                                           \
    /** Sets the \a index 'th value in the array to \a newValue.                                        */ \
    /** The array will be automatically expanded, if necessary.                                         */ \
    void                        set1Value(int index, valueRef newValue);                                   \
                                                                                                           \
    /** Sets the first value in the array to \a newValue, and deletes the second and subsequent values. */ \
    void                        setValue(valueRef newValue);                                               \
    /** Sets the first value in the array to \a newValue, and deletes the second and subsequent values. */ \
    valueRef                        operator =(valueRef newValue)                                          \
        { setValue(newValue); return newValue; }                                                           \
                                                                                                           \
    /** Returns TRUE if all of the values of this field equal (do not equal) those of the given field.  */ \
    /** If the fields are different types FALSE will always be returned (even if one field is an        */ \
    /** <tt>SoMFFloat</tt> with one value of 1.0 and the other is an <tt>SoMFInt</tt> with a value of 1,*/ \
    /** for example).                                                                                   */ \
    int                                operator ==(const className &f) const;                              \
    /** Inequality test for fields of same type                                                         */ \
    int                                operator !=(const className &f) const                               \
        { return ! ((*this) == f); }                                                                       \
                                                                                                           \
    /** Returns a pointer to the internally-maintained array that can be modified.                      */ \
    /** The values in the array may be changed, but values cannot be added or removed.                  */ \
    /** It is illegal to call any other editing methods between #startEditing()                         */ \
    /** and #finishEditing() (e.g. #set1Value(), #setValue(), etc)                                      */ \
    /**                                                                                                 */ \
    /** Fields, engines or sensors connected to this field and sensors are not                          */ \
    /** notified that this field has changed until #finishEditing() is called.                          */ \
    /** Calling #finishEditing() always sets the #isDefault() flag to FALSE and                         */ \
    /** informs engines and sensors that the field changed, even if none of the                         */ \
    /** values actually were changed.                                                                   */ \
    valueType *                        startEditing()                                                      \
        { evaluate(); return values; }                                                                     \
                                                                                                           \
    /** Indicate that batch edits have finished \sa #startEditing()                                     */ \
    void                        finishEditing() { valueChanged(); }                                        \
                                                                                                           \
  protected:                                                                                               \
    /** Allocates room for num values.                                                                  */ \
    /** Copies old values (if any) into new area.  Deletes old area, if any.  Will reduce room if       */ \
    /** needed, so a value of newNum==0 will delete all values.                                         */ \
    virtual void                allocValues(int newNum);                                                   \
                                                                                                           \
    /** Deletes all current values, resets number of values                                             */ \
    virtual void                deleteAllValues();                                                         \
                                                                                                           \
    /** Copies value indexed by "from" to value indexed by "to"                                         */ \
    virtual void                copyValue(int to, int from);                                               \
                                                                                                           \
    valueType                        *values

////////////////////////////////////////////////////////////////////////////
//
// This is like the above macro, but can be used by a field class
// derived from another (non-abstract) field class. It will inherit
// most of the field processing stuff.
//
////////////////////////////////////////////////////////////////////////////

#define SO_MFIELD_DERIVED_VALUE_HEADER(className, valueType, valueRef)              \
    SO__MFIELD_RW_HEADER(className);                                                \
  public:                                                                           \
    /* Since = operator is not inherited, we redefine it here */                    \
    valueRef                        operator =(valueRef newValue)                   \
        { setValue(newValue); return newValue; }

////////////////////////////////////////////////////////////////////////////
//
// This macro is all that is needed for most multiple-value field
// classes. It includes the relevant macros.
//
////////////////////////////////////////////////////////////////////////////

#define SO_MFIELD_HEADER(className, valueType, valueRef)                            \
    SO_MFIELD_REQUIRED_HEADER(className);                                           \
    SO_MFIELD_CONSTRUCTOR_HEADER(className);                                        \
    SO_MFIELD_VALUE_HEADER(className, valueType, valueRef)

////////////////////////////////////////////////////////////////////////////
//
// This macro is all that is needed for most multiple-value field
// classes that are derived from other field classes. It includes the
// relevant macros.
//
////////////////////////////////////////////////////////////////////////////

#define SO_MFIELD_DERIVED_HEADER(className, valueType, valueRef)                    \
    SO_MFIELD_REQUIRED_HEADER(className);                                           \
    SO_MFIELD_CONSTRUCTOR_HEADER(className);                                        \
    SO_MFIELD_DERIVED_VALUE_HEADER(className, valueType, valueRef)

//===========================================================================
//
// Macros to be called within the source file for a single-value field
// subclass:
//
//===========================================================================

////////////////////////////////////////////////////////////////////////////
//
// This initializes type identifer variables. It should be called from
// within initClass().
//
////////////////////////////////////////////////////////////////////////////

#define SO_SFIELD_INIT_CLASS(className,parentClass)                                 \
    SO__FIELD_INIT_CLASS(className, SO__QUOTE(className), parentClass)

////////////////////////////////////////////////////////////////////////////
//
// This defines the variables and methods declared in
// SO_SFIELD_REQUIRED_HEADER(). 
//
////////////////////////////////////////////////////////////////////////////

#define        SO_SFIELD_REQUIRED_SOURCE(className)                                 \
                                                                                    \
    SO__FIELD_ID_SOURCE(className);                                                 \
    SO__FIELD_EQ_SAME_SOURCE(className)                                             \
                                                                                    \
const className &                                                                   \
className::operator =(const className &f)                                           \
{                                                                                   \
    setValue(f.getValue());                                                         \
    return *this;                                                                   \
}

////////////////////////////////////////////////////////////////////////////
//
// This defines the constructor and destructor.
//
////////////////////////////////////////////////////////////////////////////

#define SO_SFIELD_CONSTRUCTOR_SOURCE(className)                                     \
                                                                                    \
className::className()                                                              \
{                                                                                   \
}                                                                                   \
className::~className()                                                             \
{                                                                                   \
}

////////////////////////////////////////////////////////////////////////////
//
// This defines the variables and methods declared in
// SO_SFIELD_VALUE_HEADER(). 
//
////////////////////////////////////////////////////////////////////////////

#define SO_SFIELD_VALUE_SOURCE(className, valueType, valueRef)                      \
                                                                                    \
void                                                                                \
className::setValue(valueRef newValue)                                              \
{                                                                                   \
    value = newValue;                                                               \
    valueChanged();                                                                 \
}                                                                                   \
                                                                                    \
int                                                                                 \
className::operator ==(const className &f) const                                    \
{                                                                                   \
    return getValue() == f.getValue();                                              \
}

////////////////////////////////////////////////////////////////////////////
//
// This macro is all that is needed for most single-value field
// classes. It includes the relevant macros.
//
////////////////////////////////////////////////////////////////////////////

#define SO_SFIELD_SOURCE(className, valueType, valueRef)                            \
        SO_SFIELD_REQUIRED_SOURCE(className);                                       \
        SO_SFIELD_CONSTRUCTOR_SOURCE(className);                                    \
        SO_SFIELD_VALUE_SOURCE(className, valueType, valueRef)

////////////////////////////////////////////////////////////////////////////
//
// This macro is all that is needed for most single-value field
// classes that are derived from other field classes. It includes the
// relevant macros.
//
////////////////////////////////////////////////////////////////////////////

#define SO_SFIELD_DERIVED_SOURCE(className, valueType, valueRef)                    \
        SO_SFIELD_REQUIRED_SOURCE(className);                                       \
        SO_SFIELD_CONSTRUCTOR_SOURCE(className)

//===========================================================================
//
// Macros to be called within the source file for a multiple-value field
// subclass:
//
//===========================================================================

////////////////////////////////////////////////////////////////////////////
//
// This initializes type identifer variables. It should be called from
// within initClass().
//
////////////////////////////////////////////////////////////////////////////

#define SO_MFIELD_INIT_CLASS(className,parentClass)                                 \
    SO__FIELD_INIT_CLASS(className, SO__QUOTE(className), parentClass)

////////////////////////////////////////////////////////////////////////////
//
// This defines the variables and methods declared in
// SO_MFIELD_REQUIRED_HEADER(). 
//
////////////////////////////////////////////////////////////////////////////

#define        SO_MFIELD_REQUIRED_SOURCE(className)                                 \
                                                                                    \
    SO__FIELD_ID_SOURCE(className);                                                 \
    SO__FIELD_EQ_SAME_SOURCE(className)                                             \
                                                                                    \
const className &                                                                   \
className::operator =(const className &f)                                           \
{                                                                                   \
    if (f.getNum() < getNum())                                                      \
        deleteAllValues();                                                          \
    setValues(0, f.getNum(), f.getValues(0));                                       \
    return *this;                                                                   \
}

////////////////////////////////////////////////////////////////////////////
//
// This defines a constructor and destructor that work with the
// variables and methods defined in SO_MFIELD_VALUE_HEADER(). If you
// define your own value processing differently, you may not be able
// to use these.
//
////////////////////////////////////////////////////////////////////////////

#define SO_MFIELD_CONSTRUCTOR_SOURCE(className)                                     \
                                                                                    \
className::className()                                                              \
{                                                                                   \
    values = NULL;                                                                  \
}                                                                                   \
                                                                                    \
className::~className()                                                             \
{                                                                                   \
    deleteAllValues();                                                              \
}

////////////////////////////////////////////////////////////////////////////
//
// This defines an empty constructor and destructor for classes
// derived from other field classes, since they inherit value
// processing.
//
////////////////////////////////////////////////////////////////////////////

#define SO_MFIELD_DERIVED_CONSTRUCTOR_SOURCE(className)                             \
                                                                                    \
className::className()                                                              \
{                                                                                   \
}                                                                                   \
                                                                                    \
className::~className()                                                             \
{                                                                                   \
}

////////////////////////////////////////////////////////////////////////////
//
// This defines the variables and methods declared in
// SO_MFIELD_VALUE_HEADER(). 
//
////////////////////////////////////////////////////////////////////////////

#define SO_MFIELD_VALUE_SOURCE(className, valueType, valueRef)                      \
                                                                                    \
int                                                                                 \
className::find(valueRef targetValue, SbBool addIfNotFound)                         \
{                                                                                   \
    int        i, num = getNum();                                                   \
                                                                                    \
    for (i = 0; i < num; i++)                                                       \
        if (values[i] == targetValue)                                               \
            return i;                                                               \
                                                                                    \
    if (addIfNotFound)                                                              \
        set1Value(num, targetValue);                                                \
                                                                                    \
    return -1;                                                                      \
}                                                                                   \
                                                                                    \
void                                                                                \
className::setValues(int start, int num, const valueType *newValues)                \
{                                                                                   \
    int        newNum = start + num, i;                                             \
                                                                                    \
    if (newNum > getNum())                                                          \
        makeRoom(newNum);                                                           \
                                                                                    \
    for (i = 0; i < num; i++)                                                       \
        values[start + i] = newValues[i];                                           \
                                                                                    \
    valueChanged();                                                                 \
}                                                                                   \
                                                                                    \
void                                                                                \
className::set1Value(int index, valueRef newValue)                                  \
{                                                                                   \
    if (index >= getNum())                                                          \
        makeRoom(index + 1);                                                        \
    values[index] = newValue;                                                       \
    valueChanged();                                                                 \
}                                                                                   \
                                                                                    \
void                                                                                \
className::setValue(valueRef newValue)                                              \
{                                                                                   \
    makeRoom(1);                                                                    \
    values[0] = newValue;                                                           \
    valueChanged();                                                                 \
}                                                                                   \
                                                                                    \
int                                                                                 \
className::operator ==(const className &f) const                                    \
{                                                                                   \
    int                        i, num = getNum();                                   \
    const valueType        *myVals, *itsVals;                                       \
                                                                                    \
    if (num != f.getNum())                                                          \
        return FALSE;                                                               \
                                                                                    \
    myVals  = getValues(0);                                                         \
    itsVals = f.getValues(0);                                                       \
                                                                                    \
    for (i = 0; i < num; i++)                                                       \
        if (! (myVals[i] == itsVals[i]))                                            \
            return FALSE;                                                           \
                                                                                    \
    return TRUE;                                                                    \
}                                                                                   \
                                                                                    \
void                                                                                \
className::deleteAllValues()                                                        \
{                                                                                   \
    allocValues(0);                                                                 \
}                                                                                   \
                                                                                    \
void                                                                                \
className::copyValue(int to, int from)                                              \
{                                                                                   \
    values[to] = values[from];                                                      \
}

////////////////////////////////////////////////////////////////////////////
//
// This allocates/reallocates room for values, using the "new" and
// "delete" operators, so the constructor and destructor are called
// for new and deleted values.
//
////////////////////////////////////////////////////////////////////////////

#define SO_MFIELD_ALLOC_SOURCE(className, valueType)                                \
void                                                                                \
className::allocValues(int newNum)                                                  \
{                                                                                   \
    if (values == NULL)        {                                                    \
        if (newNum > 0)                                                             \
            values = new valueType[newNum];                                         \
    }                                                                               \
    else {                                                                          \
        valueType        *oldValues = values;                                       \
        int                i;                                                       \
                                                                                    \
        if (newNum > 0) {                                                           \
            values = new valueType[newNum];                                         \
            for (i = 0; i < num && i < newNum; i++)                                 \
                values[i] = oldValues[i];                                           \
        }                                                                           \
        else                                                                        \
            values = NULL;                                                          \
        delete [] oldValues;                                                        \
    }                                                                               \
                                                                                    \
    num = maxNum = newNum;                                                          \
}

////////////////////////////////////////////////////////////////////////////
//
// This allocates/reallocates room for values, using malloc() and
// realloc(). This means that constructors and destructors will NOT be
// called for values. Therefore, this macro should not be used for any
// value types that have constructors or destructors! (It is used for
// fields that have simple types, like shorts and floats.)
//
////////////////////////////////////////////////////////////////////////////

#define SO_MFIELD_MALLOC_SOURCE(className, valueType)                               \
void                                                                                \
className::allocValues(int newNum)                                                  \
{                                                                                   \
    if (values == NULL) {                                                           \
        if (newNum > 0)                                                             \
            values = (valueType *) malloc(sizeof(valueType) * newNum);              \
    }                                                                               \
    else {                                                                          \
        if (newNum > 0)                                                             \
            values = (valueType *) realloc(values, sizeof(valueType)*newNum);       \
        else {                                                                      \
            free((char *) values);                                                  \
            values = NULL;                                                          \
        }                                                                           \
    }                                                                               \
                                                                                    \
    num = maxNum = newNum;                                                          \
}

////////////////////////////////////////////////////////////////////////////
//
// This macro is all that is needed for most multiple-value field
// classes. It includes the relevant macros, including the allocation
// macro that uses new and delete.
//
////////////////////////////////////////////////////////////////////////////

#define SO_MFIELD_SOURCE(className, valueType, valueRef)                   \
    SO_MFIELD_REQUIRED_SOURCE(className)                                   \
    SO_MFIELD_CONSTRUCTOR_SOURCE(className)                                \
    SO_MFIELD_VALUE_SOURCE(className, valueType, valueRef)                 \
    SO_MFIELD_ALLOC_SOURCE(className, valueType)

////////////////////////////////////////////////////////////////////////////
//
// This is like the above macro, but uses the allocation macro that
// uses malloc() and realloc().
//
////////////////////////////////////////////////////////////////////////////

#define SO_MFIELD_SOURCE_MALLOC(className, valueType, valueRef)            \
    SO_MFIELD_REQUIRED_SOURCE(className)                                   \
    SO_MFIELD_CONSTRUCTOR_SOURCE(className)                                \
    SO_MFIELD_VALUE_SOURCE(className, valueType, valueRef)                 \
    SO_MFIELD_MALLOC_SOURCE(className, valueType)

////////////////////////////////////////////////////////////////////////////
//
// This macro is all that is needed for most multiple-value field
// classes that are derived from other field classes. It includes the
// relevant macros.
//
////////////////////////////////////////////////////////////////////////////

#define SO_MFIELD_DERIVED_SOURCE(className, valueType, valueRef)           \
    SO_MFIELD_REQUIRED_SOURCE(className);                                  \
    SO_MFIELD_DERIVED_CONSTRUCTOR_SOURCE(className)

////////////////////////////////////////////////////////////////////////////
//
//
//
////////////////////////////////////////////////////////////////////////////
#define SO_MFIELD_SETVALUESPOINTER_HEADER(valueType)                       \
    public:                                                                \
    void setValuesPointer(const int num, const valueType * userdata);      \
    void setValuesPointer(const int num, valueType * userdata)

#define SO_MFIELD_SETVALUESPOINTER_SOURCE(className, valueType, userType)  \
    void                                                                   \
    className::setValuesPointer(const int _num, userType * userdata)       \
    {                                                                      \
        makeRoom(0);                                                       \
        if (_num > 0 && userdata) {                                        \
            values = reinterpret_cast<valueType*>(userdata);               \
            num = maxNum = _num;                                           \
            valueChanged();                                                \
        }                                                                  \
    }                                                                      \
    void                                                                   \
    className::setValuesPointer(const int _num, const userType * userdata) \
    {                                                                      \
        setValuesPointer(_num, const_cast<userType*>(userdata));           \
    }

#endif /* _SO_SUB_FIELD_ */
