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

#ifndef _SO_CALC_EXPR_
#define _SO_CALC_EXPR_

#include <Inventor/SbLinear.h>
#include <Inventor/SbPList.h>

#include <string>

//
// base expression class
//
class SoCalcExpr {
  public:
    enum Type {
	FLOAT,
	VEC3F
    };
    Type	type;

    SoCalcExpr(Type t) : type(t) {}
    virtual ~SoCalcExpr();

    virtual void	print(int level);
    virtual float	getFloat();
    virtual SbVec3f	getVec3f();
    virtual float	setFloat(float v);
    virtual SbVec3f	setVec3f(const SbVec3f &);
    virtual void	eval();

    const char *typeName()
    {
	return type==FLOAT ? "float" : "Vec3f";
    }

  protected:
    friend class SoCalcParser;
    // global flag gets set true if there was an error
    static SbBool error;

    // Functions to look up data storage locations by name.
    // These should be set by the client before creating a parse tree.
    static float *(*lookupFloatField)(void *data, const char *name);
    static SbVec3f *(*lookupVec3fField)(void *data, const char *name);
    static void *data;

    void printSpace(int level);

  protected:
    // Use this like printf...
    void err(const char *formatString ...);
};

//
// List of expressions
//
class SoCalcExprList : public SbPList {
  public:
    SoCalcExprList() {}
    SoCalcExprList(const SoCalcExprList &l) : SbPList() { copy(l); }
    ~SoCalcExprList() { truncate(0); }

    void truncate(int n);
    void append(SoCalcExpr *e) { SbPList::append((void*) e); }
    SoCalcExpr *operator[](int i) const
    { return (SoCalcExpr*) (*(const SbPList*)this)[i]; }

    // evaluate each expression in the list.
    void eval();

    void	print();
};


//
// Terminal expressions
//
class SoCalcConst : public SoCalcExpr {
  public:
    const float	val;
    SoCalcConst(float v) : SoCalcExpr(FLOAT), val(v) {}
    ~SoCalcConst();
    virtual void print(int level);
    virtual float getFloat();
};

class SoCalcVar : public SoCalcExpr {
  public:
    std::string    name;
    SoCalcVar(const char *nm, Type type);
    ~SoCalcVar();
    virtual void	print(int level);
    virtual float	getFloat();
    virtual SbVec3f	getVec3f();
    virtual float	setFloat(float v);
    virtual SbVec3f	setVec3f(const SbVec3f &v);
  private:
    union {
	float	*pfloat;
	SbVec3f	*pvec3f;
    };
};

//
// Functions
//
class SoCalcFunc : public SoCalcExpr
{
  public:
    const char *const name;
    virtual SoCalcFunc *dup() const = 0;
    ~SoCalcFunc();
    virtual void setArgs(SoCalcExprList *args) = 0;
    virtual void print(int level);
  protected:
    SoCalcFunc(const char *nm, Type type);
    SoCalcExprList	*args;
};

class SoCalcFunc_d : public SoCalcFunc
{
  public:
    SoCalcFunc_d(const char *name, double (*f)(double));
    ~SoCalcFunc_d();
    virtual SoCalcFunc *dup() const;
    virtual float getFloat();
    virtual void setArgs(SoCalcExprList *args);
  private:
    double	(*const func)(double);
};

class SoCalcFunc_dd : public SoCalcFunc
{
  public:
    SoCalcFunc_dd(const char *name, double (*f)(double, double)) : SoCalcFunc(name, FLOAT), func(f) {}
    ~SoCalcFunc_dd();
    virtual SoCalcFunc *dup() const;
    virtual float getFloat();
    virtual void setArgs(SoCalcExprList *args);
  private:
    double	(*const func)(double, double);
};

class SoCalcFunc_id : public SoCalcFunc
{
  public:
    SoCalcFunc_id(const char *name, double (*f)(int, double)) : SoCalcFunc(name, FLOAT), func(f) {}
    ~SoCalcFunc_id();
    virtual SoCalcFunc *dup() const;
    virtual float getFloat();
    virtual void setArgs(SoCalcExprList *args);
  private:
    double	(*const func)(int, double);
};

class SoCalcFunc_v : public SoCalcFunc
{
  public:
    SoCalcFunc_v(const char *name, double (*f)(const SbVec3f &)) : SoCalcFunc(name, FLOAT), func(f) {}
    ~SoCalcFunc_v();
    virtual SoCalcFunc *dup() const;
    virtual float getFloat();
    virtual void setArgs(SoCalcExprList *args);
  private:
    double	(*const func)(const SbVec3f &);
};

class SoCalcFuncv_v : public SoCalcFunc
{
  public:
    SoCalcFuncv_v(const char *name, SbVec3f (*f)(const SbVec3f &)) : SoCalcFunc(name, VEC3F), func(f) {}
    ~SoCalcFuncv_v();
    virtual SoCalcFunc *dup() const;
    virtual SbVec3f getVec3f();
    virtual void setArgs(SoCalcExprList *args);
  private:
    SbVec3f	(*const func)(const SbVec3f &);
};

class SoCalcFunc_vv : public SoCalcFunc
{
  public:
    SoCalcFunc_vv(const char *name, double (*f)(const SbVec3f &, const SbVec3f &)) : SoCalcFunc(name, FLOAT), func(f) {}
    ~SoCalcFunc_vv();
    virtual SoCalcFunc *dup() const;
    virtual float getFloat();
    virtual void setArgs(SoCalcExprList *args);
  private:
    double	(*const func)(const SbVec3f &, const SbVec3f &);
};

class SoCalcFuncv_vv : public SoCalcFunc
{
  public:
    SoCalcFuncv_vv(const char *name, SbVec3f (*f)(const SbVec3f &, const SbVec3f &)) : SoCalcFunc(name, VEC3F), func(f) {}
    ~SoCalcFuncv_vv();
    virtual SoCalcFunc *dup() const;
    virtual SbVec3f getVec3f();
    virtual void setArgs(SoCalcExprList *args);
  private:
    SbVec3f	(*const func)(const SbVec3f &, const SbVec3f &);
};

class SoCalcFuncv_ddd : public SoCalcFunc
{
  public:
    SoCalcFuncv_ddd(const char *name, SbVec3f (*f)(double, double, double)) : SoCalcFunc(name, VEC3F), func(f) {}
    ~SoCalcFuncv_ddd();
    virtual SoCalcFunc *dup() const;
    virtual SbVec3f getVec3f();
    virtual void setArgs(SoCalcExprList *args);
  private:
    SbVec3f	(*const func)(double, double, double);
};

//
// Operations
//
class SoCalcBinaryOp : public SoCalcExpr
{
  public:
    virtual void print(int level);
  protected:
    // bit mask to select allowed operation types
    enum Types { FF=(1<<0), FV=(1<<1), VF=(1<<2), VV=(1<<3) };
    SoCalcBinaryOp(SoCalcExpr *ea, SoCalcExpr *eb, int TypeBits);
    ~SoCalcBinaryOp();
    SoCalcExpr	*a, *b;
};

class SoCalcAssign : public SoCalcBinaryOp
{
  public:
    SoCalcAssign(SoCalcExpr *a, SoCalcExpr *b) : SoCalcBinaryOp(a,b,FF|VV) { type=a->type; }
    ~SoCalcAssign();
    virtual float getFloat();
    virtual SbVec3f getVec3f();
};

class SoCalcAssignIndex : public SoCalcExpr
{
  public:
    // Expression is "a[b] = c"
    SoCalcAssignIndex(SoCalcExpr *a, SoCalcExpr *b, SoCalcExpr *c);
    ~SoCalcAssignIndex();
    virtual float getFloat();
  private:
    SoCalcExpr *a, *b, *c;
};

class SoCalcPlus : public SoCalcBinaryOp
{
  public:
    SoCalcPlus(SoCalcExpr *a, SoCalcExpr *b) : SoCalcBinaryOp(a,b,FF|VV) { type=a->type; }
    ~SoCalcPlus();
    virtual float getFloat();
    virtual SbVec3f getVec3f();
};

class SoCalcMinus : public SoCalcBinaryOp
{
  public:
    SoCalcMinus(SoCalcExpr *a, SoCalcExpr *b) : SoCalcBinaryOp(a,b,FF|VV) { type=a->type; }
    ~SoCalcMinus();
    virtual float getFloat();
    virtual SbVec3f getVec3f();
};

class SoCalcMult : public SoCalcBinaryOp
{
  public:
    SoCalcMult(SoCalcExpr *a, SoCalcExpr *b) : SoCalcBinaryOp(a,b,FF|FV|VF) { type=a->type==b->type?a->type:VEC3F; }
    ~SoCalcMult();
    virtual float getFloat();
    virtual SbVec3f getVec3f();
};

class SoCalcDivide : public SoCalcBinaryOp
{
  public:
    SoCalcDivide(SoCalcExpr *a, SoCalcExpr *b) : SoCalcBinaryOp(a,b,FF|VF) { type=a->type; }
    ~SoCalcDivide();
    virtual float getFloat();
    virtual SbVec3f getVec3f();
};

class SoCalcMod : public SoCalcBinaryOp
{
  public:
    SoCalcMod(SoCalcExpr *a, SoCalcExpr *b) : SoCalcBinaryOp(a,b,FF) { type=FLOAT; }
    ~SoCalcMod();
    virtual float getFloat();
};

class SoCalcIndex : public SoCalcBinaryOp
{
  public:
    SoCalcIndex(SoCalcExpr *a, SoCalcExpr *b) : SoCalcBinaryOp(a,b,VF) { type=FLOAT; }
    ~SoCalcIndex();
    virtual float getFloat();
};

class SoCalcLessThan : public SoCalcBinaryOp
{
  public:
    SoCalcLessThan(SoCalcExpr *a, SoCalcExpr *b) : SoCalcBinaryOp(a,b,FF) { type=FLOAT; }
    ~SoCalcLessThan();
    virtual float getFloat();
};

class SoCalcGreaterThan : public SoCalcBinaryOp
{
  public:
    SoCalcGreaterThan(SoCalcExpr *a, SoCalcExpr *b) : SoCalcBinaryOp(a,b,FF) { type=FLOAT; }
    ~SoCalcGreaterThan();
    virtual float getFloat();
};

class SoCalcLessEQ : public SoCalcBinaryOp
{
  public:
    SoCalcLessEQ(SoCalcExpr *a, SoCalcExpr *b) : SoCalcBinaryOp(a,b,FF) { type=FLOAT; }
    ~SoCalcLessEQ();
    virtual float getFloat();
};

class SoCalcGreaterEQ : public SoCalcBinaryOp
{
  public:
    SoCalcGreaterEQ(SoCalcExpr *a, SoCalcExpr *b) : SoCalcBinaryOp(a,b,FF) { type=FLOAT; }
    ~SoCalcGreaterEQ();
    virtual float getFloat();
};

class SoCalcEquals : public SoCalcBinaryOp
{
  public:
    SoCalcEquals(SoCalcExpr *a, SoCalcExpr *b) : SoCalcBinaryOp(a,b,FF) { type=FLOAT; }
    ~SoCalcEquals();
    virtual float getFloat();
};

class SoCalcNotEquals : public SoCalcBinaryOp
{
  public:
    SoCalcNotEquals(SoCalcExpr *a, SoCalcExpr *b) : SoCalcBinaryOp(a,b,FF) { type=FLOAT; }
    ~SoCalcNotEquals();
    virtual float getFloat();
};

class SoCalcAnd : public SoCalcBinaryOp
{
  public:
    SoCalcAnd(SoCalcExpr *a, SoCalcExpr *b) : SoCalcBinaryOp(a,b,FF) { type=FLOAT; }
    ~SoCalcAnd();
    virtual float getFloat();
};

class SoCalcOr : public SoCalcBinaryOp
{
  public:
    SoCalcOr(SoCalcExpr *a, SoCalcExpr *b) : SoCalcBinaryOp(a,b,FF) { type=FLOAT; }
    ~SoCalcOr();
    virtual float getFloat();
};

class SoCalcNot : public SoCalcExpr
{
  public:
    SoCalcNot(SoCalcExpr *exp) : SoCalcExpr(FLOAT), e(exp) {}
    ~SoCalcNot();
    virtual float getFloat();
    virtual void print(int level);
  private:
    SoCalcExpr *e;
};

class SoCalcNegate : public SoCalcExpr
{
  public:
    SoCalcNegate(SoCalcExpr *exp) : SoCalcExpr(exp->type), e(exp) { }
    ~SoCalcNegate();
    virtual float getFloat();
    virtual SbVec3f getVec3f();
    virtual void print(int level);
  private:
    SoCalcExpr *e;
};

class SoCalcTernary : public SoCalcExpr
{
  public:
    SoCalcTernary(SoCalcExpr *a, SoCalcExpr *b, SoCalcExpr *c);
    ~SoCalcTernary();
    virtual float getFloat();
    virtual SbVec3f getVec3f();
    virtual void print(int level);
  private:
    SoCalcExpr *etest;
    SoCalcExpr *etrue;
    SoCalcExpr *efalse;
};

//
// Top-level parser class
//
class SoCalcParser {
  public:
    SoCalcParser(float *(*lookupFloatField)(void *data, const char *name),
	 SbVec3f *(*lookupVec3fField)(void *data, const char *name),
	void *data);
    ~SoCalcParser();

    // parse some more expressions, add to the collection
    SbBool	parse(const char *buf);

    // evaluate the collection of expressions
    void	eval() { elist->eval(); }

    // empty the collection of parsed expressions
    void	clear() { elist->truncate(0); }

  private:
    float *(*lookupFloatField)(void *data, const char *name);
    SbVec3f *(*lookupVec3fField)(void *data, const char *name);
    void *data;

    SoCalcExprList	*elist;
};

#endif
