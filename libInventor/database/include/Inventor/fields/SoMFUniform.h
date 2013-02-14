#ifndef  _SO_MF_UNIFORM_
#define  _SO_MF_UNIFORM_

#include <Inventor/fields/SoSubField.h>
#include <Inventor/SbUniform.h>

/// Multiple-value field containing any number of uniform.
/// \ingroup Fields
/// A multiple-value field that contains any number of uniform.
///
/// <tt>SoMFUniform</tt>s are written to file as one or more pairs of string
/// values separated by whitespace.
/// When more than one value is present, all of the
/// values are enclosed in square brackets and separated by commas; for
/// example:
///
/// [ "time" 0, "foo" 3.4, "bar" -4e1 ]
///
class INVENTOR_API SoMFUniform : public SoMField {
    // Use standard field stuff
    SO_MFIELD_HEADER(SoMFUniform, SbUniform, const SbUniform &);
    SO_MFIELD_SETVALUESPOINTER_HEADER(SbUniform);

  public:
    //
    // Some additional convenience functions:
    //

    /// Set the \a index 'th value to the given floating point values.
    void	set1Value(int index, float x, float y);

    /// Sets the field to contain the given value and only the given value (if
    /// the array had multiple values before, they are deleted).
    void	setValue(float x, float y);

  SoINTERNAL public:
    static void	initClass();

  private:
    virtual void	writeBinaryValues(SoOutput *out) const;
    virtual SbBool	readBinaryValues(SoInput *in, int numToRead);
};

#endif /* _SO_MF_UNIFORM_ */
