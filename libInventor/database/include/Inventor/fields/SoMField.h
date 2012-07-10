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
 |	Defines the SoMField class.
 |
 |   Author(s)		: Paul S. Strauss, Nick Thompson, Gavin Bell
 |
 ______________  S I L I C O N   G R A P H I C S   I N C .  ____________
 _______________________________________________________________________
 */

#ifndef  _SO_MFIELD_
#define  _SO_MFIELD_

#include <Inventor/fields/SoField.h>

/// Base class for all multiple-valued fields.
/// \ingroup Fields
/// Each class derived from <tt>SoMField</tt> begins with an <tt>SoMF</tt> prefix
/// and contains a dynamic array of values of a particular type.  Each has a
/// #setValues() method
/// that is passed a pointer to a const array of values of the correct
/// type; these values are copied into the array in the field, making
/// extra room in the array if necessary. The start and num parameters to
/// this method indicate the starting array index to copy into and the
/// number of values to copy.
///
/// The #getValues() method
/// for a multiple-value field returns a const pointer to the array of
/// values in the field. (Because this pointer is const, it cannot be used
/// to change values in this array.)
///
/// In addition, the indexing operator "[]" is overloaded to return the
/// \a i'th value in the array; because it returns a const reference, it can
/// be used only to get values, not to set them.
///
/// Methods are provided for getting the number of values in the field,
/// inserting space for new values in the middle, and deleting values.
///
/// There are other methods that allow you to set only one value of
/// several in the field and to set the field to contain one and only one
/// value.
///
/// Two other methods can be used to make several changes to a
/// multiple-value field without the overhead of copying values into and
/// out of the fields. The #startEditing() method
/// returns a non-const pointer to the array of values in the field; this
/// pointer can then be used to change (but not add or remove) any values
/// in the array. The #finishEditing() method
/// indicates that the editing is done and notifies any sensors or engines
/// that may be connected to the field.
///
/// <tt>SoMFields</tt> are written to file as a series of values separated by
/// commas, all enclosed in square brackets.  If the field has no values
/// (#getNum() returns zero), then only the square brackets ("[]") are
/// written.  The last value may optionally be followed by a comma.  Each
/// field subtype defines how the values are written; for example, a field
/// whose values are integers might be written as:
/// [ 1, 2, 3, 4 ]
///    or:
/// [ 1, 2, 3, 4, ]
/// \sa SoNode, SoEngine
class INVENTOR_API SoMField : public SoField {

  public:
    /// Destructor
    virtual ~SoMField();

    /// Returns the number of values currently in the field.
    int			getNum() const { evaluate(); return num; }

    /// Forces this field to have exactly num values, inserting or deleting
    /// values as necessary.
    void		setNum(int num);

    /// Deletes \a num values beginning at index \a start (index \a start through
    /// \a start + \a num -1 will be deleted, and any leftover values will be moved
    /// down to fill in the gap created).  A \a num of -1 means delete all values
    /// from \a start to the last value in the field; #getNum() will return
    /// \a start as the number of values in the field after this operation
    /// (#deleteValues(0, -1) empties the field).
    virtual void	deleteValues(int start, int num = -1);

    /// Inserts space for \a num values at index \a start.  Index \a start through
    /// \a start + \a num -1 will be moved up to make room.  For example, to make room
    /// for 7 new values at the beginning of the field call #insertSpace(0, 10).
    virtual void	insertSpace(int start, int num);

    /// These are equivalent to the SoField::set() and SoField::get()
    /// methods, but operate on only the value given by the index.
    SbBool		set1(int index, const char *valueString);
    void		get1(int index, SbString &valueString);

    /// Return the type identifier for this field class.
    static SoType	getClassTypeId()	{ return classTypeId; }

  SoINTERNAL public:
    static void		initClass();

    static void *	createInstance();

  protected:
    int                 num;            // Number of values
    int                 maxNum;         // Number of values allocated
    bool                hasUserData;    // Indicator if data has been set via a pointer and memory isn't managed by SoMField


    // Constructor
    SoMField();

    // Make sure there is room for newNum vals
    virtual void	makeRoom(int newNum);

  private:
    static SoType	classTypeId;

    // These are used by SoMField::get1() to hold the returned field string
    static char    *fieldBuf;
    static size_t   fieldBufSize;

    // Allocates room for num values. Copies old values (if any) into
    // new area. Deletes old area, if any.
    virtual void	allocValues(int num) = 0;

    // Deletes all current values
    virtual void	deleteAllValues() = 0;

    // Copies value indexed by "from" to value indexed by "to"
    virtual void	copyValue(int to, int from) = 0;

    // Reads all values of field
    virtual SbBool	readValue(SoInput *in);

    // Reads one indexed value of field from file
    virtual SbBool	read1Value(SoInput *in, int index) = 0;

    // Writes all values of field
    virtual void	writeValue(SoOutput *out) const;

    // Writes indexed value to file
    virtual void	write1Value(SoOutput *out, int index) const = 0;

    // Reads array of binary values from file
    virtual SbBool	readBinaryValues(SoInput *in, int numToRead);

    // Writes array of binary values to file
    virtual void	writeBinaryValues(SoOutput *out) const;

    // Returns number of ASCII values to write per output line (default 1)
    virtual int		getNumValuesPerLine() const;

    // This is used to reallocate the string buffer used by
    // SoField::get()
    static void *	reallocFieldBuf(void *ptr, size_t newSize);
};

#endif /* _SO_MFIELD_ */
