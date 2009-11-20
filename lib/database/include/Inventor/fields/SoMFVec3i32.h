#ifndef  _SO_MF_VEC3I32_
#define  _SO_MF_VEC3I32_

#include <Inventor/fields/SoSubField.h>
#include <Inventor/SbVec3i32.h>

/// Multiple-value field containing any number of three-dimensional vectors.
/// \ingroup Fields
/// A multiple-value field that contains any number of three-dimensional
/// vectors.
///
/// <tt>SoMFVec3i32</tt>s are written to file as one or more triples of integer
/// point values separated by whitespace.
///
/// When more than one value is present, all of the
/// values are enclosed in square brackets and separated by commas; for
/// example:
///
/// [ 0 0 0, 1 3 5, 98 -4 212 ]
///
class INVENTOR_API SoMFVec3i32 : public SoMField {
    // Use standard field stuff
    SO_MFIELD_HEADER(SoMFVec3i32, SbVec3i32, const SbVec3i32 &);
    SO_MFIELD_SETVALUESPOINTER_HEADER(SbVec3i32);
    SO_MFIELD_SETVALUESPOINTER_HEADER(int32_t);

  public:

    //
    // Some additional convenience functions:
    //

    /// Sets \a num values starting at index \a start to the given int32_ting point
    /// values.  There must be \a num *3 values in the passed array.
    void	setValues(int start, int num, const int32_t xyz[][3]);

    /// Set the \a index 'th value to the given int32_ting point values.
    void	set1Value(int index, int32_t x, int32_t y, int32_t z);

    /// Set the \a index 'th value to the given int32_ting point values.
    void	set1Value(int index, const int32_t xyz[3]);

    /// Sets the field to contain the given value and only the given value (if
    /// the array had multiple values before, they are deleted).
    void	setValue(int32_t x, int32_t y, int32_t z);

    /// Sets the field to contain the given value and only the given value (if
    /// the array had multiple values before, they are deleted).
    void	setValue(const int32_t xyz[3]);

  SoINTERNAL public:
    static void		initClass();

  private:

    // Write the values out as a block of data
    virtual void	writeBinaryValues(SoOutput *out) const;
    virtual SbBool	readBinaryValues(SoInput *in, int numToRead);
};

#endif /* _SO_MF_VEC3I32_ */
