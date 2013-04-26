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

/*
 * Copyright (C) 1990,91   Silicon Graphics, Inc.
 *
 _______________________________________________________________________
 ______________  S I L I C O N   G R A P H I C S   I N C .  ____________
 |
 |   $Revision: 1.2 $
 |
 |   Classes:
 |      ???
 |
 |   Author(s)          : Ronen Barzel
 |
 ______________  S I L I C O N   G R A P H I C S   I N C .  ____________
 _______________________________________________________________________
 */

#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <Inventor/errors/SoDebugError.h>
#include "SoCalcExpr.h"

// for development, these provide a builtin
// data storage location (don't need to
// connect the parser to an engine).
static float *lookupfloat(void *, const char *nm)
{
    static float store[20];
    if (strcmp(nm, "a") ==0) return &store[0];
    if (strcmp(nm, "b") ==0) return &store[1];
    if (strcmp(nm, "c") ==0) return &store[2];
    if (strcmp(nm, "d") ==0) return &store[3];
    if (strcmp(nm, "e") ==0) return &store[4];
    if (strcmp(nm, "f") ==0) return &store[5];
    if (strcmp(nm, "g") ==0) return &store[6];
    if (strcmp(nm, "h") ==0) return &store[7];
    if (strcmp(nm, "ta") ==0) return &store[8];
    if (strcmp(nm, "tb") ==0) return &store[9];
    if (strcmp(nm, "tc") ==0) return &store[10];
    if (strcmp(nm, "td") ==0) return &store[11];
    if (strcmp(nm, "te") ==0) return &store[12];
    if (strcmp(nm, "tf") ==0) return &store[13];
    if (strcmp(nm, "tg") ==0) return &store[14];
    if (strcmp(nm, "th") ==0) return &store[15];
    if (strcmp(nm, "oa") ==0) return &store[16];
    if (strcmp(nm, "ob") ==0) return &store[17];
    if (strcmp(nm, "oc") ==0) return &store[18];
    if (strcmp(nm, "od") ==0) return &store[19];
    return NULL;
}
static SbVec3f *lookupvec3f(void *, const char *nm)
{
    static SbVec3f store[20];
    if (strcmp(nm, "A") ==0) return &store[0];
    if (strcmp(nm, "B") ==0) return &store[1];
    if (strcmp(nm, "C") ==0) return &store[2];
    if (strcmp(nm, "D") ==0) return &store[3];
    if (strcmp(nm, "E") ==0) return &store[4];
    if (strcmp(nm, "F") ==0) return &store[5];
    if (strcmp(nm, "G") ==0) return &store[6];
    if (strcmp(nm, "H") ==0) return &store[7];
    if (strcmp(nm, "tA") ==0) return &store[8];
    if (strcmp(nm, "tB") ==0) return &store[9];
    if (strcmp(nm, "tC") ==0) return &store[10];
    if (strcmp(nm, "tD") ==0) return &store[11];
    if (strcmp(nm, "tE") ==0) return &store[12];
    if (strcmp(nm, "tF") ==0) return &store[13];
    if (strcmp(nm, "tG") ==0) return &store[14];
    if (strcmp(nm, "tH") ==0) return &store[15];
    if (strcmp(nm, "oA") ==0) return &store[16];
    if (strcmp(nm, "oB") ==0) return &store[17];
    if (strcmp(nm, "oC") ==0) return &store[18];
    if (strcmp(nm, "oD") ==0) return &store[19];
    return NULL;
}

//
// base expression class
//

SbBool SoCalcExpr::error;
float *(*SoCalcExpr::lookupFloatField)(void *, const char *) = lookupfloat;
SbVec3f *(*SoCalcExpr::lookupVec3fField)(void *, const char *) = lookupvec3f;
void *SoCalcExpr::data;



SoCalcExpr::~SoCalcExpr() {}

void
SoCalcExpr::print(int level)
{
#ifdef DEBUG
    printSpace(level);
    printf("SoCalcExpr: type %s\n", type == FLOAT ? "FLOAT" : "VEC3F");
#else
    SB_UNUSED(level);
#endif /* DEBUG */
}

void
SoCalcExpr::printSpace(int level)
{
#ifdef DEBUG
    for (int i = level / 2; i > 0; --i)
	putchar('\t');

    if (level & 1) {
	putchar(' ');
	putchar(' ');
	putchar(' ');
	putchar(' ');
    }
#else
    SB_UNUSED(level);
#endif /* DEBUG */
}

float
SoCalcExpr::getFloat() {
    err("SoCalcExpr: internal error: can't get float val");
    return 0;
}

SbVec3f
SoCalcExpr::getVec3f() {
    err("SoCalcExpr: internal error: can't get vector val");
    return SbVec3f(0,0,0);
}


float
SoCalcExpr::setFloat(float) {
    err("SoCalcExpr: internal error: can't set float val");
    return 0;
}

SbVec3f
SoCalcExpr::setVec3f(const SbVec3f &) {
    err("SoCalcExpr: internal error: can't set vector val");
    return SbVec3f(0,0,0);
}

void
SoCalcExpr::eval() {
    if (type == FLOAT)
	getFloat();
    else
	getVec3f();
}

void
SoCalcExpr::err(const char *formatString ...)
{
    char        buf[1000];
    va_list     ap;

    va_start(ap, formatString);
    vsprintf(buf, formatString, ap);
    va_end(ap);

#ifdef DEBUG
    SoDebugError::post("SoCalculator::evaluate", buf);
#endif
    error = TRUE;
}


//
// List of expressions
//
void
SoCalcExprList::truncate(int n)
{
    for (int i=n; i<getLength(); i++)
    {
	delete (*this)[i];
    }

    SbPList::truncate(0);
}

void
SoCalcExprList::eval()
{
    for (int i=0; i<getLength(); i++)
	(*this)[i]->eval();
}

void
SoCalcExprList::print()
{
#ifdef DEBUG
    printf("List: %d expressions\n", getLength());
    for (int i = 0; i < getLength(); i++) {
	printf("\nExpression %d:\n\n", i);
	(*this)[i]->print(0);
    }
#endif /* DEBUG */
}

//
// Terminal expressions
//
SoCalcConst::~SoCalcConst() {}

void
SoCalcConst::print(int level)
{
#ifdef DEBUG
    printSpace(level);
    printf("SoCalcConst: val = %g\n", val);
#else
    SB_UNUSED(level);
#endif /* DEBUG */
}

float
SoCalcConst::getFloat()
{
    return val;
}


SoCalcVar::SoCalcVar(const char *nm, Type type)
    : SoCalcExpr(type), name(nm)
{
    if (type == FLOAT)
    pfloat = lookupFloatField(data, name.c_str());
    else
    pvec3f = lookupVec3fField(data, name.c_str());
}

SoCalcVar::~SoCalcVar() { }

void SoCalcVar::print(int level)
{
#ifdef DEBUG
    printSpace(level);
    printf("SoCalcVar: name = %s, value = ", name.c_str());
    if (type == FLOAT)
	printf("%g", *pfloat);
    else
	printf("(%g %g %g)", (*pvec3f)[0], (*pvec3f)[1], (*pvec3f)[2]);
    printf("\n");
#else
    SB_UNUSED(level);
#endif /* DEBUG */
}

float SoCalcVar::getFloat()
{
    return *pfloat;
}

SbVec3f SoCalcVar::getVec3f()
{
    return *pvec3f;
}

float SoCalcVar::setFloat(float v)
{
    return *pfloat = v;
}

SbVec3f SoCalcVar::setVec3f(const SbVec3f &v)
{
    return *pvec3f = v;
}

//
// Functions
//
SoCalcFunc::SoCalcFunc(const char *nm, Type type) : SoCalcExpr(type), name(nm), args(NULL) {}
SoCalcFunc::~SoCalcFunc() { delete args; }

void
SoCalcFunc::print(int level)
{
#ifdef DEBUG
    printSpace(level);
    printf("SoCalcFunc: name = %s, %d args:\n", name, args->getLength());
    for (int i = 0; i < args->getLength(); i++)
	(*args)[i]->print(level + 1);
#else
    SB_UNUSED(level);
#endif /* DEBUG */
}

SoCalcFunc_d::SoCalcFunc_d(const char *name, double (*f)(double)) :
        SoCalcFunc(name, FLOAT), func(f) {}
SoCalcFunc_d::~SoCalcFunc_d() {}
SoCalcFunc *SoCalcFunc_d::dup() const { return new SoCalcFunc_d(name, func); }
float SoCalcFunc_d::getFloat() { return (*func)((*args)[0]->getFloat()); }
void SoCalcFunc_d::setArgs(SoCalcExprList *a)
{
    args = a;
    if (args->getLength() != 1)
	err("Function %s: wrong number of arguments (%d should be 1)", name, args->getLength());
    if ((*args)[0]->type != FLOAT)
	err("Function %s: argument must be scalar", name);
};

SoCalcFunc_dd::~SoCalcFunc_dd() {}
SoCalcFunc *SoCalcFunc_dd::dup() const { return new SoCalcFunc_dd(name, func); }
float SoCalcFunc_dd::getFloat() {
    SoCalcExpr *e0 = (*args)[0], *e1 = (*args)[1];
    return (*func)(e0->getFloat(), e1->getFloat());
}
void SoCalcFunc_dd::setArgs(SoCalcExprList *a)
{
    args = a;
    if (args->getLength() != 2)
	err("Function %s: wrong number of arguments (%d should be 2)", name, args->getLength());
    if ((*args)[0]->type != FLOAT)
	err("Function %s: first argument must be scalar", name);
    if ((*args)[1]->type != FLOAT)
	err("Function %s: second argument must be scalar", name);
};

SoCalcFunc_id::~SoCalcFunc_id() {}
SoCalcFunc *SoCalcFunc_id::dup() const { return new SoCalcFunc_id(name, func); }
float SoCalcFunc_id::getFloat() {
    SoCalcExpr *e0 = (*args)[0], *e1 = (*args)[1];
    return (*func)((int) e0->getFloat(), e1->getFloat());
}
void SoCalcFunc_id::setArgs(SoCalcExprList *a)
{
    args = a;
    if (args->getLength() != 2)
	err("Function %s: wrong number of arguments (%d should be 2)", name, args->getLength());
    if ((*args)[0]->type != FLOAT)
	err("Function %s: first argument must be scalar", name);
    if ((*args)[1]->type != FLOAT)
	err("Function %s: second argument must be scalar", name);
};

SoCalcFunc_v::~SoCalcFunc_v() {}
SoCalcFunc *SoCalcFunc_v::dup() const { return new SoCalcFunc_v(name, func); }
float SoCalcFunc_v::getFloat() { return (*func)((*args)[0]->getVec3f()); }
void SoCalcFunc_v::setArgs(SoCalcExprList *a)
{
    args = a;
    if (args->getLength() != 1)
	err("Function %s: wrong number of arguments (%d should be 1)", name, args->getLength());
    if ((*args)[0]->type != VEC3F)
	err("Function %s: first argument must be vector", name);
};

SoCalcFuncv_v::~SoCalcFuncv_v() {}
SoCalcFunc *SoCalcFuncv_v::dup() const { return new SoCalcFuncv_v(name, func); }
SbVec3f SoCalcFuncv_v::getVec3f() { return (*func)((*args)[0]->getVec3f()); }
void SoCalcFuncv_v::setArgs(SoCalcExprList *a)
{
    args = a;
    if (args->getLength() != 1)
	err("Function %s: wrong number of arguments (%d should be 1)", name, args->getLength());
    if ((*args)[0]->type != VEC3F)
	err("Function %s: first argument must be vector", name);
};

SoCalcFunc_vv::~SoCalcFunc_vv() {}
SoCalcFunc *SoCalcFunc_vv::dup() const { return new SoCalcFunc_vv(name, func); }
float SoCalcFunc_vv::getFloat() {
    SoCalcExpr *e0 = (*args)[0], *e1 = (*args)[1];
    return (*func)(e0->getVec3f(), e1->getVec3f());
}
void SoCalcFunc_vv::setArgs(SoCalcExprList *a)
{
    args = a;
    if (args->getLength() != 2)
	err("Function %s: wrong number of arguments (%d should be 2)", name, args->getLength());
    if ((*args)[0]->type != VEC3F)
	err("Function %s: first argument must be vector", name);
    if ((*args)[1]->type != VEC3F)
	err("Function %s: second argument must be vector", name);
};

SoCalcFuncv_vv::~SoCalcFuncv_vv() {}
SoCalcFunc *SoCalcFuncv_vv::dup() const { return new SoCalcFuncv_vv(name, func); }
SbVec3f SoCalcFuncv_vv::getVec3f() {
    SoCalcExpr *e0 = (*args)[0], *e1 = (*args)[1];
    return (*func)(e0->getVec3f(), e1->getVec3f());
}
void SoCalcFuncv_vv::setArgs(SoCalcExprList *a)
{
    args = a;
    if (args->getLength() != 2)
	err("Function %s: wrong number of arguments (%d should be 2)", name, args->getLength());
    if ((*args)[0]->type != VEC3F)
	err("Function %s: first argument must be vector", name);
    if ((*args)[1]->type != VEC3F)
	err("Function %s: second argument must be vector", name);
};

SoCalcFuncv_ddd::~SoCalcFuncv_ddd() {}
SoCalcFunc *SoCalcFuncv_ddd::dup() const { return new SoCalcFuncv_ddd(name, func); }
SbVec3f SoCalcFuncv_ddd::getVec3f() {
    SoCalcExpr *e0 = (*args)[0], *e1 = (*args)[1], *e2 = (*args)[2];
    return (*func)(e0->getFloat(), e1->getFloat(), e2->getFloat());
}
void SoCalcFuncv_ddd::setArgs(SoCalcExprList *a)
{
    args = a;
    if (args->getLength() != 3)
	err("Function %s: wrong number of arguments (%d should be 3)", name, args->getLength());
    if ((*args)[0]->type != FLOAT)
	err("Function %s: first argument must be scalar", name);
    if ((*args)[1]->type != FLOAT)
	err("Function %s: second argument must be scalar", name);
    if ((*args)[2]->type != FLOAT)
	err("Function %s: third argument must be scalar", name);
};

//
// Operations
//
SoCalcBinaryOp::~SoCalcBinaryOp() { delete a; delete b; }
SoCalcBinaryOp::SoCalcBinaryOp(SoCalcExpr *ea, SoCalcExpr *eb, int TypeBits) : SoCalcExpr(ea->type), a(ea), b(eb)
{
    SbBool OK = FALSE;

    if (a->type == FLOAT && b->type == FLOAT)
	OK = TypeBits & FF;

    else if (a->type == FLOAT && b->type == VEC3F)
	OK = TypeBits & FV;

    else if (a->type == VEC3F && b->type == FLOAT)
	OK = TypeBits & VF;

    else if (a->type == VEC3F && b->type == VEC3F)
	OK = TypeBits & VV;
    
    if (!OK)
	err("BinaryOp:  incompatible types (%s and %s)",
	    a->typeName(), b->typeName());
}

SoCalcAssignIndex::SoCalcAssignIndex(SoCalcExpr *_a, SoCalcExpr *_b, SoCalcExpr *_c)
    : SoCalcExpr(SoCalcExpr::FLOAT)
{
    a = _a;
    b = _b;
    c = _c;

    if (a->type != SoCalcExpr::VEC3F)
	err("In expressions of the form 'A[b] = c', A must be a vector");
    if (b->type != SoCalcExpr::FLOAT)
	err("In expressions of the form 'A[b] = c', b must be a float");
    if (c->type != SoCalcExpr::FLOAT)
	err("In expressions of the form 'A[b] = c', c must be a float");
}
    
SoCalcAssignIndex::~SoCalcAssignIndex() {}

float
SoCalcAssignIndex::getFloat()
{
    float result = c->getFloat();
    SbVec3f val = a->getVec3f();
    int index = int(b->getFloat());
    if (index < 0 || index > 2)
	err("Vector index out of range (must be 0-2, is %d)", index);
    else {
	val[index] = result;
	a->setVec3f(val);
    }
    return result;
}

void SoCalcBinaryOp::print(int level)
{
#ifdef DEBUG
    printSpace(level);
    printf("BinaryOp: args:\n");
    a->print(level + 1);
    b->print(level + 1);
#else
    SB_UNUSED(level);
#endif /* DEBUG */
}

SoCalcAssign::~SoCalcAssign() {}
SoCalcPlus::~SoCalcPlus() {}
SoCalcMinus::~SoCalcMinus() {}
SoCalcMult::~SoCalcMult() {}
SoCalcDivide::~SoCalcDivide() {}
SoCalcMod::~SoCalcMod() {}
SoCalcIndex::~SoCalcIndex() {}

float SoCalcAssign::getFloat() { return a->setFloat(b->getFloat()); }
SbVec3f SoCalcAssign::getVec3f() { return a->setVec3f(b->getVec3f()); }

float SoCalcPlus::getFloat() { return a->getFloat() + b->getFloat(); }
SbVec3f SoCalcPlus::getVec3f() { return a->getVec3f() + b->getVec3f(); }

float SoCalcMinus::getFloat(){ return a->getFloat() - b->getFloat(); }
SbVec3f SoCalcMinus::getVec3f() { return a->getVec3f() - b->getVec3f(); }

float SoCalcMult::getFloat() { return a->getFloat() * b->getFloat(); }
SbVec3f SoCalcMult::getVec3f()
{
    return (a->type == FLOAT) ?
	a->getFloat() * b->getVec3f() :
	a->getVec3f() * b->getFloat();
}

float SoCalcDivide::getFloat() { return a->getFloat() / b->getFloat(); }
SbVec3f SoCalcDivide::getVec3f() { return a->getVec3f() / b->getFloat(); }

float SoCalcMod::getFloat() { return (int) a->getFloat() % (int) b->getFloat(); }

float SoCalcIndex::getFloat() { return a->getVec3f()[(int) b->getFloat()]; }

SoCalcLessThan::~SoCalcLessThan() {}
SoCalcGreaterThan::~SoCalcGreaterThan() {}
SoCalcLessEQ::~SoCalcLessEQ() {}
SoCalcGreaterEQ::~SoCalcGreaterEQ() {}
SoCalcEquals::~SoCalcEquals() {}
SoCalcNotEquals::~SoCalcNotEquals() {}
SoCalcAnd::~SoCalcAnd() {}
SoCalcOr::~SoCalcOr() {}

float SoCalcLessThan::getFloat() { return a->getFloat() < b->getFloat(); }
float SoCalcGreaterThan::getFloat() { return a->getFloat() > b->getFloat(); }
float SoCalcLessEQ::getFloat() { return a->getFloat() <= b->getFloat(); }
float SoCalcGreaterEQ::getFloat() { return a->getFloat() >= b->getFloat(); }
float SoCalcEquals::getFloat() { return a->getFloat() == b->getFloat(); }
float SoCalcNotEquals::getFloat() { return a->getFloat() != b->getFloat(); }
float SoCalcAnd::getFloat() { return a->getFloat() && b->getFloat(); }
float SoCalcOr::getFloat() { return a->getFloat() || b->getFloat(); }

SoCalcNot::~SoCalcNot() { delete e; }
float SoCalcNot::getFloat() { return ! e->getFloat(); }

void SoCalcNot::print(int level)
{
#ifdef DEBUG
    printSpace(level);
    printf("Not: arg:\n");
    e->print(level + 1);
#else
    SB_UNUSED(level);
#endif /* DEBUG */
}

SoCalcNegate::~SoCalcNegate() { delete e; }
float SoCalcNegate::getFloat() { return - e->getFloat(); }
SbVec3f SoCalcNegate::getVec3f() { return - e->getVec3f(); }

void SoCalcNegate::print(int level)
{
#ifdef DEBUG
    printSpace(level);
    printf("Negate: arg:\n");
    e->print(level + 1);
#else
    SB_UNUSED(level);
#endif /* DEBUG */
}

SoCalcTernary::~SoCalcTernary() { delete etest; delete etrue; delete efalse; }
SoCalcTernary::SoCalcTernary(SoCalcExpr *a, SoCalcExpr *b, SoCalcExpr *c)
    : SoCalcExpr(SoCalcExpr::FLOAT), etest(a), etrue(b), efalse(c)
{
    if (etest->type != FLOAT)
	err("Ternary:: conditional expression in ?: operator must be scalar");

    if (etrue->type != efalse->type)
	err("Ternary:: different types across ?: operator (%s and %s)",
		etrue->typeName(), efalse->typeName());
    
    type = etrue->type;
};

float SoCalcTernary::getFloat() { return etest->getFloat() ? etrue->getFloat() : efalse->getFloat(); }
SbVec3f SoCalcTernary::getVec3f() { return etest->getFloat() ? etrue->getVec3f() : efalse->getVec3f(); }

void SoCalcTernary::print(int level)
{
#ifdef DEBUG
    printSpace(level);
    printf("Ternary: conditional + 2 args:\n");
    etest->print(level + 1);
    etrue->print(level + 1);
    efalse->print(level + 1);
#else
    SB_UNUSED(level);
#endif /* DEBUG */
}

/******************************************
**
** This is the top-level interface to the parser
**
******************************************/

SoCalcParser::SoCalcParser(float *(*lf)(void *data, const char *name),
	       SbVec3f *(*lv)(void *data, const char *name),
	       void *d)
{
    lookupFloatField = lf;
    lookupVec3fField = lv;
    data = d;

    elist = new SoCalcExprList;
}

SoCalcParser::~SoCalcParser()
{
    delete elist;
}

SbBool
SoCalcParser::parse(const char *buf)
{
    SoCalcExpr::lookupFloatField = lookupFloatField;
    SoCalcExpr::lookupVec3fField = lookupVec3fField;
    SoCalcExpr::data = data;

    SoCalcExpr::error = FALSE;

    extern SbBool SoCalcParse(SoCalcExprList *, const char *);

    if (SoCalcParse(elist, buf))
	return FALSE;

// Enable this to print all parsed expressions
#if DEBUG_PARSED_RESULT
    elist->print();
#endif

    return !SoCalcExpr::error;
}
