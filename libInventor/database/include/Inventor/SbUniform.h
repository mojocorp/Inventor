#ifndef _SB_UNIFORM_
#define _SB_UNIFORM_

#include <Inventor/SbString.h>
#include <Inventor/SbVec2i32.h>
#include <Inventor/SbVec3i32.h>
#include <Inventor/SbVec4i32.h>
#include <Inventor/SbVec2f.h>
#include <Inventor/SbVec3f.h>
#include <Inventor/SbVec4f.h>
#include <Inventor/SbMatrix.h>

class INVENTOR_API SbUniform {
public:
    enum UniformType
    {
        UNIFORM_INT   = 1,
        UNIFORM_VEC2I = 2,
        UNIFORM_VEC3I = 3,
        UNIFORM_VEC4I = 4,
        UNIFORM_FLOAT = 5,
        UNIFORM_VEC2F = 6,
        UNIFORM_VEC3F = 7,
        UNIFORM_VEC4F = 8,
        UNIFORM_MAT4F = 9
    };

    SbUniform(const SbString & name, int val);
    SbUniform(const SbString & name, const SbVec2i32 & val);
    SbUniform(const SbString & name, const SbVec3i32 & val);
    SbUniform(const SbString & name, const SbVec4i32 & val);
    SbUniform(const SbString & name, float val);
    SbUniform(const SbString & name, const SbVec2f & val);
    SbUniform(const SbString & name, const SbVec3f & val);
    SbUniform(const SbString & name, const SbVec4f & val);
    SbUniform(const SbString & name, const SbMatrix & val);

    ~SbUniform();

    const SbString &getName() const      {return mName;}
    UniformType     getType() const      {return mType;}
    int             getNumVals() const   {return mNumVals;}
    const int *     getIntVals() const   {return mIvals;}
    const float *   getFloatVals() const {return mFvals;}

private:
    SbUniform();

    SbString    mName;
    UniformType mType;

    int         mNumVals;
    int         mIvals[4];
    float       mFvals[16];
};

#endif /* _SB_UNIFORM_ */
