#include <Inventor/SbUniform.h>

SbUniform::SbUniform(const SbString & name, int val)
    : mName(name),
      mType(UNIFORM_INT),
      mNumVals(1)
{
    mIvals[0] = val;
}

SbUniform::SbUniform(const SbString & name, const SbVec2i32 & val)
    : mName(name),
      mType(UNIFORM_VEC2I),
      mNumVals(2)
{
    mIvals[0] = val[0];
    mIvals[1] = val[1];
}

SbUniform::SbUniform(const SbString & name, const SbVec3i32 & val)
    : mName(name),
      mType(UNIFORM_VEC3I),
      mNumVals(3)
{
    mIvals[0] = val[0];
    mIvals[1] = val[1];
    mIvals[2] = val[2];
}

SbUniform::SbUniform(const SbString & name, const SbVec4i32 & val)
    : mName(name),
      mType(UNIFORM_VEC4I),
      mNumVals(4)
{
    mIvals[0] = val[0];
    mIvals[1] = val[1];
    mIvals[2] = val[2];
    mIvals[3] = val[3];
}

SbUniform::SbUniform(const SbString & name, float val)
    : mName(name),
      mType(UNIFORM_FLOAT),
      mNumVals(1)
{
    mFvals[0] = val;
}

SbUniform::SbUniform(const SbString & name, const SbVec2f & val)
    : mName(name),
      mType(UNIFORM_VEC2F),
      mNumVals(2)
{
    mFvals[0] = val[0];
    mFvals[1] = val[1];
}

SbUniform::SbUniform(const SbString & name, const SbVec3f & val)
    : mName(name),
      mType(UNIFORM_VEC3F),
      mNumVals(3)
{
    mFvals[0] = val[0];
    mFvals[1] = val[1];
    mFvals[2] = val[2];
}

SbUniform::SbUniform(const SbString & name, const SbVec4f & val)
    : mName(name),
      mType(UNIFORM_VEC4F),
      mNumVals(4)
{
    mFvals[0] = val[0];
    mFvals[1] = val[1];
    mFvals[2] = val[2];
    mFvals[3] = val[3];
}

SbUniform::SbUniform(const SbString & name, const SbMatrix & val)
    : mName(name),
      mType(UNIFORM_MAT4F),
      mNumVals(16)
{
    mFvals[0]  = val[0][0]; mFvals[1]  = val[1][0]; mFvals[2]  = val[2][0]; mFvals[3]  = val[3][0];
    mFvals[4]  = val[0][1]; mFvals[5]  = val[1][1]; mFvals[6]  = val[2][1]; mFvals[7]  = val[3][1];
    mFvals[8]  = val[0][2]; mFvals[9]  = val[1][2]; mFvals[10] = val[2][2]; mFvals[11] = val[3][2];
    mFvals[12] = val[0][3]; mFvals[13] = val[1][3]; mFvals[14] = val[2][3]; mFvals[15] = val[3][3];
}

SbUniform::~SbUniform()
{
}
