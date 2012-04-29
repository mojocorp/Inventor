#ifndef  _SO_SF_BOX3F_
#define  _SO_SF_BOX3F_

#include <Inventor/fields/SoSubField.h>
#include <Inventor/SbBox3f.h>

/// Field containing a SbBox3f.
/// \ingroup Fields
/// This field is used where nodes, engines or other field containers needs to store a box.
/// \sa SoField, SoSField, SoMFInt32, SoSFUInt32
class INVENTOR_API SoSFBox3f : public SoSField {

    // Use standard field stuff
    SO_SFIELD_HEADER(SoSFBox3f, SbBox3f, const SbBox3f &);
  public:
    /// Set value from 6 floats.
    void setValue(float xmin, float ymin, float zmin,
                  float xmax, float ymax, float zmax);

    /// Set value from 2 vectors.
    void setValue(const SbVec3f & minvec, const SbVec3f & maxvec);
  SoINTERNAL public:

    static void		initClass();
};

#endif /* _SO_SF_BOX3F_ */
