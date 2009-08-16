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

//////////////////////////////////////////////////////////////////////////////
//
//  Class: SoMField
//
//  Field that can have multiple values.
//
//////////////////////////////////////////////////////////////////////////////

class INVENTOR_API SoMField : public SoField {

  public:
    // Destructor
    virtual ~SoMField();

    // Returns number of values
    int			getNum() const { evaluate(); return num; }

    // Adjust the number of values to be num, adding or deleting
    // values at the end as needed.  If adding, the initial values
    // in the new space are undefined.
    void		setNum(int num);

    // Delete num values, starting at start. A num of -1 (the default)
    // means delete all values after start, inclusive
    virtual void	deleteValues(int start, int num = -1);

    // Insert space for num values starting at start.  The initial
    // values in the new space are undefined.
    virtual void	insertSpace(int start, int num);

    // These are equivalent to the SoField::set() and SoField::get()
    // methods, but operate on only the value given by the index.
    SbBool		set1(int index, const char *valueString);
    void		get1(int index, SbString &valueString);

    // Returns type identifier for SoMField class
    static SoType	getClassTypeId()	{ return classTypeId; }

  SoINTERNAL public:
    static void		initClass();

    static void *	createInstance();

  protected:
    int			num;		// Number of values
    int			maxNum;		// Number of values allocated

    // Constructor
    SoMField();

    // Make sure there is room for newNum vals
    virtual void	makeRoom(int newNum);

  private:
    static SoType	classTypeId;

    // These are used by SoMField::get1() to hold the returned field string
    static char		*fieldBuf;
    static int		fieldBufSize;

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
