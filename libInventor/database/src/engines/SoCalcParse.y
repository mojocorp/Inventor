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

%{
#include <Inventor/misc/SoBasic.h>
#include <Inventor/errors/SoDebugError.h>
#include "SoCalcExpr.h"
#include <stdio.h>

#ifdef SB_OS_WIN
#   define drand48() (((double)rand())/double(RAND_MAX))
#endif

#include <float.h>
#include <math.h>
#include <ctype.h>
#include <stdlib.h>

void yyerror(const char *);
extern "C" int yylex();
int yyparse();

static const char *In;
static SoCalcExprList   *EList;

static const struct {
    const char *name;
    double      val;
} Constants[] = {
    { "MAXFLOAT",   FLT_MAX },
    { "MINFLOAT",   FLT_MIN },
    { "M_E",        M_E },
    { "M_LOG2E",    M_LOG2E },
    { "M_LOG10E",   M_LOG10E },
    { "M_LN2",      M_LN2 },
    { "M_LN10",     M_LN10 },
    { "M_PI",       M_PI },
    { "M_SQRT2",    M_SQRT2 },
    { "M_SQRT1_2",  M_SQRT1_2 },
};
#define NCONSTANTS (sizeof(Constants)/sizeof(Constants[0]))

static const struct {
    const char *name;
    SoCalcExpr::Type  type;
} Inputs[] = {
    { "a", SoCalcExpr::FLOAT}, { "b", SoCalcExpr::FLOAT},
    { "c", SoCalcExpr::FLOAT}, { "d", SoCalcExpr::FLOAT},
    { "e", SoCalcExpr::FLOAT}, { "f", SoCalcExpr::FLOAT},
    { "g", SoCalcExpr::FLOAT}, { "h", SoCalcExpr::FLOAT},
    { "A", SoCalcExpr::VEC3F}, { "B", SoCalcExpr::VEC3F},
    { "C", SoCalcExpr::VEC3F}, { "D", SoCalcExpr::VEC3F},
    { "E", SoCalcExpr::VEC3F}, { "F", SoCalcExpr::VEC3F},
    { "G", SoCalcExpr::VEC3F}, { "H", SoCalcExpr::VEC3F},
}, Outputs[] = {
    { "oa", SoCalcExpr::FLOAT}, { "ob", SoCalcExpr::FLOAT},
    { "oc", SoCalcExpr::FLOAT}, { "od", SoCalcExpr::FLOAT},
    { "oA", SoCalcExpr::VEC3F}, { "oB", SoCalcExpr::VEC3F},
    { "oC", SoCalcExpr::VEC3F}, { "oD", SoCalcExpr::VEC3F},
}, Vars[] = {
    { "ta", SoCalcExpr::FLOAT}, { "tb", SoCalcExpr::FLOAT},
    { "tc", SoCalcExpr::FLOAT}, { "td", SoCalcExpr::FLOAT},
    { "te", SoCalcExpr::FLOAT}, { "tf", SoCalcExpr::FLOAT},
    { "tg", SoCalcExpr::FLOAT}, { "th", SoCalcExpr::FLOAT},
    { "tA", SoCalcExpr::VEC3F}, { "tB", SoCalcExpr::VEC3F},
    { "tC", SoCalcExpr::VEC3F}, { "tD", SoCalcExpr::VEC3F},
    { "tE", SoCalcExpr::VEC3F}, { "tF", SoCalcExpr::VEC3F},
    { "tG", SoCalcExpr::VEC3F}, { "tH", SoCalcExpr::VEC3F},
};
#define NINPUTS (sizeof(Inputs)/sizeof(Inputs[0]))
#define NOUTPUTS (sizeof(Outputs)/sizeof(Outputs[0]))
#define NVARS (sizeof(Vars)/sizeof(Vars[0]))

static double rand(double max) { return max * drand48(); }
static SbVec3f cross(const SbVec3f &a, const SbVec3f &b) { return a.cross(b); }
static double dot(const SbVec3f &a, const SbVec3f &b) { return a.dot(b); }
static double length(const SbVec3f &a) { return a.length(); }
static SbVec3f normalize(const SbVec3f &v) { SbVec3f t=v; t.normalize(); return t; }
static SbVec3f vec3f(double a, double b, double c) { return SbVec3f(float(a),float(b),float(c)); }

// Keep this up to date with the info in initFuncs()
#define NFUNCS 25

static struct {
    const SoCalcFunc *func;
} Funcs[NFUNCS];

%}

%token LEXERR
%token <expr> CONST FUNC INPUT OUTPUT VAR
%token OROR ANDAND GEQ LEQ EQEQ NEQ

%union
{
    SoCalcExpr    *expr;
    SoCalcExprList    *list;
}

%type <expr> asgn primary_expression postfix_expression
%type <expr> unary_expression
%type <expr> multiplicative_expression
%type <expr> additive_expression relational_expression
%type <expr> equality_expression logical_AND_expression
%type <expr> logical_OR_expression conditional_expression
%type <list> args

%%

asgnlist:
    asgn { EList->append($1); }
    | asgnlist  ';' asgn { EList->append($3); }
    | asgnlist  ';'
    ;

asgn:
    OUTPUT '=' conditional_expression
                { $$ = new SoCalcAssign($1, $3); }
    | OUTPUT '[' conditional_expression ']' '=' conditional_expression
                { $$ = new SoCalcAssignIndex($1, $3, $6); }
    | VAR    '=' conditional_expression
                { $$ = new SoCalcAssign($1, $3); }
    | VAR '[' conditional_expression ']' '=' conditional_expression
                { $$ = new SoCalcAssignIndex($1, $3, $6); }
    ;

primary_expression:
    CONST                 { $$ = $1; }
    | INPUT                { $$ = $1; }
    | OUTPUT            { $$ = $1; }
    | VAR                { $$ = $1; }
        | '(' conditional_expression ')'    { $$ = $2; }
        ;

postfix_expression:
        primary_expression    { $$ = $1; }
        | postfix_expression '[' conditional_expression ']'
                 { $$ = new SoCalcIndex($1, $3); }
    | FUNC '(' args ')'    { ((SoCalcFunc *)$1)->setArgs($3); $$ = $1; }
        ;

args    :
    conditional_expression        { ($$ = new SoCalcExprList)->append($1); }
    | args ',' conditional_expression { $1->append($3); $$ = $1; }
    ;

unary_expression:
        postfix_expression
        | '-' unary_expression    { $$ = new SoCalcNegate($2); }
        | '!' unary_expression    { $$ = new SoCalcNot($2); }
        ;

multiplicative_expression:
        unary_expression
        | multiplicative_expression '*' unary_expression
            { $$ = new SoCalcMult($1, $3); }
        | multiplicative_expression '/' unary_expression
            { $$ = new SoCalcDivide($1, $3); }
        | multiplicative_expression '%' unary_expression
            { $$ = new SoCalcMod($1, $3); }
        ;

additive_expression:
        multiplicative_expression
        | additive_expression '+' multiplicative_expression
            { $$ = new SoCalcPlus($1, $3); }
        | additive_expression '-' multiplicative_expression
            { $$ = new SoCalcMinus($1, $3); }
        ;

relational_expression:
        additive_expression
        | relational_expression '<' additive_expression
            { $$ = new SoCalcLessThan($1, $3); }
        | relational_expression '>' additive_expression
            { $$ = new SoCalcGreaterThan($1, $3); }
        | relational_expression LEQ additive_expression
            { $$ = new SoCalcLessEQ($1, $3); }
        | relational_expression GEQ additive_expression
            { $$ = new SoCalcGreaterEQ($1, $3); }
        ;

equality_expression:
        relational_expression
        | equality_expression EQEQ relational_expression
            { $$ = new SoCalcEquals($1, $3); }
        | equality_expression NEQ relational_expression
            { $$ = new SoCalcNotEquals($1, $3); }
        ;

logical_AND_expression:
        equality_expression
        | logical_AND_expression ANDAND equality_expression
            { $$ = new SoCalcAnd($1, $3); }
        ;

logical_OR_expression:
        logical_AND_expression
        | logical_OR_expression OROR logical_AND_expression
            { $$ = new SoCalcOr($1, $3); }
        ;

conditional_expression:
        logical_OR_expression
        | logical_OR_expression '?' conditional_expression ':'
                conditional_expression
            { $$ = new SoCalcTernary($1, $3, $5); }
        ;

%%

//
// The static Funcs array contains instances of objects that have
// constructors. Since this wouldn't get set up correctly in a DSO, we
// have to initialize the array at run-time.
//

static void
initFuncs()
{
    int    i = 0;

#define MAKEFUNC(CLASS, NAME)                              \
    Funcs[i++].func = new CLASS(SO__QUOTE(NAME),NAME)

    MAKEFUNC(SoCalcFunc_d, acos);
    MAKEFUNC(SoCalcFunc_d, asin);
    MAKEFUNC(SoCalcFunc_d, atan);
    MAKEFUNC(SoCalcFunc_dd, atan2);
    MAKEFUNC(SoCalcFunc_d, ceil);
    MAKEFUNC(SoCalcFunc_d, cos);
    MAKEFUNC(SoCalcFunc_d, cosh);
    MAKEFUNC(SoCalcFuncv_vv, cross);
    MAKEFUNC(SoCalcFunc_vv, dot);
    MAKEFUNC(SoCalcFunc_d, exp);
    MAKEFUNC(SoCalcFunc_d, fabs);
    MAKEFUNC(SoCalcFunc_d, floor);
    MAKEFUNC(SoCalcFunc_dd, fmod);
    MAKEFUNC(SoCalcFunc_v, length);
    MAKEFUNC(SoCalcFunc_d, log);
    MAKEFUNC(SoCalcFunc_d, log10);
    MAKEFUNC(SoCalcFuncv_v, normalize);
    MAKEFUNC(SoCalcFunc_dd, pow);
    MAKEFUNC(SoCalcFunc_d, rand);
    MAKEFUNC(SoCalcFunc_d, sin);
    MAKEFUNC(SoCalcFunc_d, sinh);
    MAKEFUNC(SoCalcFunc_d, sqrt);
    MAKEFUNC(SoCalcFunc_d, tan);
    MAKEFUNC(SoCalcFunc_d, tanh);
    MAKEFUNC(SoCalcFuncv_ddd, vec3f);

#ifdef DEBUG
    // Sanity check
    if (i != NFUNCS)
    SoDebugError::post("SoCalcParse initFuncs (internal)",
                       "Wrong number of functions declared");
#endif /* DEBUG */
}

static SoCalcConst *
isConst(const char *nm)
{
    for (size_t i=0; i<NCONSTANTS; i++)
        if (strcmp(nm, Constants[i].name)==0)
            return new SoCalcConst(float(Constants[i].val));
    return NULL;
}

static SoCalcFunc *
isFunc(const char *nm)
{
    for (int i=0; i<NFUNCS; i++) {
        const SoCalcFunc *f = Funcs[i].func;
        if (strcmp(nm, f->name)==0)
            return f->dup();
    }
    return NULL;
}

static SoCalcVar *
isInput(const char *nm)
{
    for (size_t i=0; i<NINPUTS; i++) if (strcmp(nm, Inputs[i].name)==0)
        return new SoCalcVar(nm, Inputs[i].type);
    return NULL;
}

static SoCalcVar *
isOutput(const char *nm)
{
    for (size_t i=0; i<NOUTPUTS; i++) if (strcmp(nm, Outputs[i].name)==0)
        return new SoCalcVar(nm, Outputs[i].type);
    return NULL;
}

static SoCalcVar *
isVar(const char *nm)
{
    for (size_t i=0; i<NVARS; i++) if (strcmp(nm, Vars[i].name)==0)
        return new SoCalcVar(nm, Vars[i].type);
    return NULL;
}

int yylex()
{
#define BSZ 100
    char c, buf[BSZ+1], *cp = buf;

    // skip over blanks
    while (isspace(*In))
    In++;

    if (!In[0])
        return EOF;

    if (isdigit(In[0]) || In[0] == '.') {

        // skip past a valid floating-point number
        // (don't need to compute the number, will
        // use atof() to do that).
        SbBool dot = FALSE;
        SbBool exp = FALSE;
        for (;;) {
            c = *In++;
            if (cp - buf < BSZ)
                *cp++ = c;
            *cp = c;
            if (isdigit(c))
                continue;
            if (c== '.') {
                if (dot || exp)
                    return LEXERR;
                dot = TRUE;
                continue;
            }
            if (c == 'e' || c == 'E') {
                if (exp)
                    return LEXERR;
                exp = TRUE;
                continue;
            }
            // Check for negative exponent
            if (c == '-' && exp && (In[-2] == 'e' || In[-2] == 'E')) {
                continue;
            }
            break;    // end of number
        }
        if (In[-1] == 'e')
            return LEXERR;

        *cp = 0;
        In--;    // push back last character "read"

        yylval.expr = new SoCalcConst((float)atof(buf));
        return CONST;
    }

    if (isalpha(In[0])) {
        while (isalnum(In[0]) || In[0] == '_') {
            if (cp - buf < BSZ)
                *cp++ = *In++;
        }
        *cp = 0;

        if ( (yylval.expr = isConst(buf)) )
            return CONST;

        if ( (yylval.expr = isFunc(buf)) )
            return FUNC;

        if ( (yylval.expr = isInput(buf)) )
            return INPUT;

        if ( (yylval.expr = isOutput(buf)) )
            return OUTPUT;

        if ( (yylval.expr = isVar(buf)) )
            return VAR;

    }

#define EQ2(x,y)    (x[0]==y[0] && x[1]==y[1])
    if (EQ2(In, "==")) { In += 2; return EQEQ; }
    if (EQ2(In, "!=")) { In += 2; return NEQ; }
    if (EQ2(In, ">=")) { In += 2; return GEQ; }
    if (EQ2(In, "<=")) { In += 2; return LEQ; }
    if (EQ2(In, "&&")) { In += 2; return ANDAND; }
    if (EQ2(In, "||")) { In += 2; return OROR; }

    return *In++;
}

void
yyerror(const char *)
{
    // printf("parse error: %s\n", s);
}

SbBool
SoCalcParse(SoCalcExprList *elist, const char *buf)
{
    static SbBool initted = FALSE;

    if (! initted) {
        initFuncs();
        initted = TRUE;
    }

    In = buf;
    EList = elist;
    return(yyparse());
}

#ifdef TESTMAIN

void
main()
{
    char buf[1000];
#if YYDEBUG
    extern int yydebug;
    yydebug = 1;
#endif
    EList = new SoCalcExprList;

    while (gets(buf)) {
    In = buf;
    EList->truncate(0);
    if (yyparse())
        printf("Parse error.\n");
    else {
        EList->eval();
        for (int i=0; i<EList->length(); i++) {
            (*EList)[i]->pr();
        }
    }
    }
}

#endif
