
/*
 * TINYEXPR - Tiny recursive descent parser and evaluation engine in C
 *
 * Copyright (c) 2015-2019 Lewis Van Winkle
 *
 * This software is provided 'as-is', without any express or implied
 * warranty. In no event will the authors be held liable for any damages
 * arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 * claim that you wrote the original software. If you use this software
 * in a product, an acknowledgement in the product documentation would be
 * appreciated but is not required.
 * 2. Altered source versions must be plainly marked as such, and must not be
 * misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 */

// -- http://CodePlea.com

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

/* Notice that this file is a largely modified version of original linked below. */

/* < https://github.com/codeplea/tinyexpr/blob/ffb0d41b13e5f8d318db95feb071c220c134fe70/tinyexpr.c > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#include "../m_spaghettis.h"
#include "../m_core.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "x_tinyexpr.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

/* Is there a better approach with ffast-math flag set? */

#if defined ( __cplusplus )

#define TE_NAN              std::numeric_limits < double >::quiet_NaN()
#define TE_INFINITY         std::numeric_limits < double >::infinity()

#else

#define TE_NAN              (0.0 / 0.0)
#define TE_INFINITY         (1.0 / 0.0)

#endif

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#define TE_MASK(type)       ((type)  & 0x1fUL)
#define TE_IS_PURE(type)    (((type) & TE_PURE) != 0)

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

enum {
    TOK_NULL        = 16,   // TE_FUNCTION7 + 1
    TOK_ERROR       = 17,
    TOK_END         = 18,
    TOK_COMMA       = 19,
    TOK_OPEN        = 20,
    TOK_CLOSE       = 21,
    TOK_NUMBER      = 22,
    TOK_VARIABLE    = 23,
    TOK_INFIX       = 24
    };

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

typedef double (*te_fn0) (void);
typedef double (*te_fn1) (double);
typedef double (*te_fn2) (double, double);
typedef double (*te_fn3) (double, double, double);
typedef double (*te_fn4) (double, double, double, double);
typedef double (*te_fn5) (double, double, double, double, double);
typedef double (*te_fn6) (double, double, double, double, double, double);
typedef double (*te_fn7) (double, double, double, double, double, double, double);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

typedef struct te_state {
    int                 x_type;
    int                 x_count;
    double              x_value;
    const double        *x_bound;
    const void          *x_fn;
    const te_variable   *x_variables;
    const char          *x_p;
    } te_state;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static te_expr *te_power        (te_state *);
static te_expr *te_list         (te_state *);
static te_expr *te_expression   (te_state *);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static int te_arity (int type)
{
    switch (TE_MASK (type)) {
    //
    case TE_FUNCTION0 : return 0;
    case TE_FUNCTION1 : return 1;
    case TE_FUNCTION2 : return 2;
    case TE_FUNCTION3 : return 3;
    case TE_FUNCTION4 : return 4;
    case TE_FUNCTION5 : return 5;
    case TE_FUNCTION6 : return 6;
    case TE_FUNCTION7 : return 7;
    default           : return 0;
    //
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "x_tinyfunctions.c"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#define TE_BUILTINS 37

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static const te_variable te_functions[TE_BUILTINS] =  /* Static. */
    {
        { "abs",    (const void *)fn_abs,       TE_FUNCTION1 | TE_PURE },
        { "acos",   (const void *)fn_acos,      TE_FUNCTION1 | TE_PURE },
        { "asin",   (const void *)fn_asin,      TE_FUNCTION1 | TE_PURE },
        { "atan",   (const void *)fn_atan,      TE_FUNCTION1 | TE_PURE },
        { "atan2",  (const void *)fn_atan2,     TE_FUNCTION2 | TE_PURE },
        { "ceil",   (const void *)fn_ceil,      TE_FUNCTION1 | TE_PURE },
        { "cos",    (const void *)fn_cos,       TE_FUNCTION1 | TE_PURE },
        { "cosh",   (const void *)fn_cosh,      TE_FUNCTION1 | TE_PURE },
        { "e",      (const void *)fn_e,         TE_FUNCTION0 | TE_PURE },
        { "eq",     (const void *)fn_eq,        TE_FUNCTION2 | TE_PURE },
        { "exp",    (const void *)fn_exp,       TE_FUNCTION1 | TE_PURE },
        { "fac",    (const void *)fn_fac,       TE_FUNCTION1 | TE_PURE },
        { "floor",  (const void *)fn_floor,     TE_FUNCTION1 | TE_PURE },
        { "ftz",    (const void *)fn_ftz,       TE_FUNCTION1 | TE_PURE },
        { "ge",     (const void *)fn_ge,        TE_FUNCTION2 | TE_PURE },
        { "gt",     (const void *)fn_gt,        TE_FUNCTION2 | TE_PURE },
        { "isinf",  (const void *)fn_isinf,     TE_FUNCTION1 | TE_PURE },
        { "isnan",  (const void *)fn_isnan,     TE_FUNCTION1 | TE_PURE },
        { "le",     (const void *)fn_le,        TE_FUNCTION2 | TE_PURE },
        { "ln",     (const void *)fn_log,       TE_FUNCTION1 | TE_PURE },
        { "log",    (const void *)fn_log10,     TE_FUNCTION1 | TE_PURE },
        { "log10",  (const void *)fn_log10,     TE_FUNCTION1 | TE_PURE },
        { "lt",     (const void *)fn_lt,        TE_FUNCTION2 | TE_PURE },
        { "max",    (const void *)fn_max,       TE_FUNCTION2 | TE_PURE },
        { "min",    (const void *)fn_min,       TE_FUNCTION2 | TE_PURE },
        { "ncr",    (const void *)fn_ncr,       TE_FUNCTION2 | TE_PURE },
        { "ne",     (const void *)fn_ne,        TE_FUNCTION2 | TE_PURE },
        { "npr",    (const void *)fn_npr,       TE_FUNCTION2 | TE_PURE },
        { "pi",     (const void *)fn_pi,        TE_FUNCTION0 | TE_PURE },
        { "pow",    (const void *)fn_pow,       TE_FUNCTION2 | TE_PURE },
        { "rand",   (const void *)fn_rand,      TE_FUNCTION0           },
        { "randmt", (const void *)fn_randmt,    TE_FUNCTION0           },
        { "sin",    (const void *)fn_sin,       TE_FUNCTION1 | TE_PURE },
        { "sinh",   (const void *)fn_sinh,      TE_FUNCTION1 | TE_PURE },
        { "sqrt",   (const void *)fn_sqrt,      TE_FUNCTION1 | TE_PURE },
        { "tan",    (const void *)fn_tan,       TE_FUNCTION1 | TE_PURE },
        { "tanh",   (const void *)fn_tanh,      TE_FUNCTION1 | TE_PURE }
    };

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

static const te_variable *te_find (const te_variable *variables, int count, const char *name, size_t n)
{
    int i;
    
    for (i = 0; i < count; i++) {
    //
    const te_variable *t = variables + i;
    
    if ((strncmp (name, t->te_name, n) == 0) && (t->te_name[n] == 0)) { return t; }
    //
    }
    
    return NULL;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

static const te_variable *te_findFunction (const char *name, size_t n)
{
    return te_find (te_functions, TE_BUILTINS, name, n);
}

static const te_variable *te_findVariable (te_state *s, const char *name, size_t n)
{
    if (s->x_variables) { return te_find (s->x_variables, s->x_count, name, n); } return NULL;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static te_expr *te_new (int type)
{
    te_expr *x = (te_expr *)PD_MEMORY_GET (sizeof (te_expr));

    x->te_type = type;
    
    return x;
}

static te_expr *te_newWithParameters (int type, te_expr *e0, te_expr *e1)
{
    te_expr *x = te_new (type);

    x->te_parameters[0] = e0;
    x->te_parameters[1] = e1;
    
    return x;
}

static void te_freeParameters (te_expr *x)
{
    if (x) {
    //
    int i;
    
    for (i = 0; i < TE_PARAMETERS; i++) {
        if (x->te_parameters[i]) { te_free (x->te_parameters[i]); x->te_parameters[i] = NULL; }
        else {
            break;
        }
    }
    //
    }
}

PD_LOCAL void te_free (te_expr *x)
{
    if (x) {
    //
    te_freeParameters (x); PD_MEMORY_FREE (x);
    //
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void te_nextTokenNumber (te_state *s)
{
    s->x_type = TOK_NUMBER; s->x_value = strtod (s->x_p, (char **)&s->x_p);
}

static void te_nextTokenVariableOrFunction (te_state *s)
{
    const char *start = s->x_p;

    while (char_isAlphanumericLowercaseOrUnderscore ((*s->x_p))) { s->x_p++; }

    {
    //
    size_t n = s->x_p - start;
    
    const te_variable *v = te_findVariable (s, start, n);

    if (!v) { v = te_findFunction (start, n); }

    if (!v) { s->x_type = TOK_ERROR; }
    else {
    //
    switch (TE_MASK (v->te_type)) {
    //
    case TE_VARIABLE  : s->x_type = TOK_VARIABLE; s->x_bound = (const double *)v->te_address; break;
    case TE_FUNCTION0 : /* Falls through. */
    case TE_FUNCTION1 : /* Falls through. */
    case TE_FUNCTION2 : /* Falls through. */
    case TE_FUNCTION3 : /* Falls through. */
    case TE_FUNCTION4 : /* Falls through. */
    case TE_FUNCTION5 : /* Falls through. */
    case TE_FUNCTION6 : /* Falls through. */
    case TE_FUNCTION7 : s->x_type = v->te_type; s->x_fn = v->te_address; break;
    //
    }
    //
    }
    //
    }
}

static void te_nextTokenOperator (te_state *s)
{
    char c = (*s->x_p);
    
    s->x_p++;
    
    switch (c) {
    //
    case '+'  : s->x_type = TOK_INFIX; s->x_fn = (const void *)fn_add;       break;
    case '-'  : s->x_type = TOK_INFIX; s->x_fn = (const void *)fn_substract; break;
    case '*'  : s->x_type = TOK_INFIX; s->x_fn = (const void *)fn_multiply;  break;
    case '/'  : s->x_type = TOK_INFIX; s->x_fn = (const void *)fn_divide;    break;
    case '^'  : s->x_type = TOK_INFIX; s->x_fn = (const void *)fn_pow;       break;     // --
    case '%'  : s->x_type = TOK_INFIX; s->x_fn = (const void *)fn_fmod;      break;
    case '('  : s->x_type = TOK_OPEN;  break;                                           // --
    case ')'  : s->x_type = TOK_CLOSE; break;
    case ','  : s->x_type = TOK_COMMA; break;
    case ' '  : break;
    case '\t' : break;
    case '\n' : break;
    case '\r' : break;
    default   : s->x_type = TOK_ERROR; break;
    //
    }
}

static void te_nextToken (te_state *s)
{
    s->x_type  = TOK_NULL;
    s->x_value = 0.0;
    s->x_bound = NULL;
    s->x_fn    = NULL;
    
    do {
    //
    char c = (*s->x_p);
    
    if (c == 0) { s->x_type = TOK_END; }
    else if (char_isNumericOrDot (c)) { te_nextTokenNumber (s); }
    else if (char_isAlphabeticLowercase (c)) { te_nextTokenVariableOrFunction (s); }
    else {
        te_nextTokenOperator (s);
    }
    //
    } while (s->x_type == TOK_NULL);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

// -- <base> = <constant>
// --   | <variable>
// --   | <function-0> { "(" ")" }
// --   | <function-1> <power>
// --   | <function-X> "(" <expr> { "," <expr> } ")"
// --   | "(" <list> ")"

static te_expr *te_base (te_state *s)
{
    te_expr *x = NULL;

    switch (TE_MASK (s->x_type)) {
    //
    case TOK_NUMBER     :
        x = te_new (TE_CONSTANT);
        x->te_value = s->x_value;
        te_nextToken (s);
        break;

    case TOK_VARIABLE   :
        x = te_new (TE_VARIABLE);
        x->te_bound = s->x_bound;
        te_nextToken (s);
        break;

    case TE_FUNCTION0   :
        x = te_new (s->x_type);
        x->te_fn = s->x_fn;
        te_nextToken (s);
        if (s->x_type == TOK_OPEN) {
            te_nextToken (s);
            if (s->x_type != TOK_CLOSE) { s->x_type = TOK_ERROR; }
            else {
                te_nextToken (s);
            }
        }
        break;

    case TE_FUNCTION1   :
        x = te_new (s->x_type);
        x->te_fn = s->x_fn;
        te_nextToken (s);
        x->te_parameters[0] = te_power (s);
        break;

    case TE_FUNCTION2   :
    case TE_FUNCTION3   :
    case TE_FUNCTION4   :
    case TE_FUNCTION5   :
    case TE_FUNCTION6   :
    case TE_FUNCTION7   :
        {
        //
        int arity = te_arity (s->x_type);
        x = te_new (s->x_type);
        x->te_fn = s->x_fn;
        te_nextToken (s);

        if (s->x_type != TOK_OPEN) { s->x_type = TOK_ERROR; }
        else {
            int i;
            for (i = 0; i < arity; i++) {
                te_nextToken (s);
                x->te_parameters[i] = te_expression (s);
                if (s->x_type != TOK_COMMA) { break; }
            }
            if (s->x_type != TOK_CLOSE || i != arity - 1) { s->x_type = TOK_ERROR; }
            else {
                te_nextToken (s);
            }
        }
        //
        }
        break;

    case TOK_OPEN   :
        te_nextToken (s);
        x = te_list (s);
        if (s->x_type != TOK_CLOSE) { s->x_type = TOK_ERROR; }
        else {
            te_nextToken (s);
        }
        break;

    default         :
        x = te_new (TE_CONSTANT);
        s->x_type   = TOK_ERROR;
        x->te_value = TE_NAN;
        break;
    //
    }

    return x;
}

// -- <power> = { ("-" | "+") } <base>

static te_expr *te_power (te_state *s)
{
    int sign = 1;
    
    while (s->x_type == TOK_INFIX && (s->x_fn == fn_add || s->x_fn == fn_substract)) {
    //
    if (s->x_fn == fn_substract) { sign = -sign; }
    te_nextToken (s);
    //
    }

    te_expr *x;

    if (sign == 1) { x = te_base (s); }
    else {
    //
    x = te_newWithParameters (TE_FUNCTION1 | TE_PURE, te_base (s), NULL);
    x->te_fn = (const void *)fn_negate;
    //
    }

    return x;
}

// -- <factor> = <power> { "^" <power> }

static te_expr *te_factor (te_state *s)
{
    te_expr *x = te_power (s);

    while (s->x_type == TOK_INFIX && (s->x_fn == fn_pow)) {
    //
    const void *t = s->x_fn;
    te_nextToken (s);
    x = te_newWithParameters (TE_FUNCTION2 | TE_PURE, x, te_power (s));
    x->te_fn = t;
    //
    }

    return x;
}

// -- <term> = <factor> { ("*" | "/" | "%") <factor> }

static te_expr *te_term (te_state *s)
{
    te_expr *x = te_factor (s);
    
    while (s->x_type == TOK_INFIX && (s->x_fn == fn_multiply || s->x_fn == fn_divide || s->x_fn == fn_fmod)) {
    //
    const void * t = s->x_fn;
    te_nextToken (s);
    x = te_newWithParameters (TE_FUNCTION2 | TE_PURE, x, te_factor (s));
    x->te_fn = t;
    //
    }

    return x;
}

// -- <expression> = <term> { ("+" | "-") <term> }

static te_expr *te_expression (te_state *s)
{
    te_expr *x = te_term (s);

    while (s->x_type == TOK_INFIX && (s->x_fn == fn_add || s->x_fn == fn_substract)) {
    //
    const void *t = s->x_fn;
    te_nextToken (s);
    x = te_newWithParameters (TE_FUNCTION2 | TE_PURE, x, te_term (s));
    x->te_fn = t;
    //
    }

    return x;
}

// -- <list> = <expression> { "," <expression> }

static te_expr *te_list (te_state *s)
{
    te_expr *x = te_expression (s);

    while (s->x_type == TOK_COMMA) {
    //
    te_nextToken (s);
    x = te_newWithParameters (TE_FUNCTION2 | TE_PURE, x, te_expression (s));
    x->te_fn = (const void *)fn_comma;
    //
    }

    return x;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#define TE_M(i)     te_eval (x->te_parameters[i])

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#define TE_M0       TE_M (0)
#define TE_M1       TE_M (0), TE_M (1)
#define TE_M2       TE_M (0), TE_M (1), TE_M (2)
#define TE_M3       TE_M (0), TE_M (1), TE_M (2), TE_M (3)
#define TE_M4       TE_M (0), TE_M (1), TE_M (2), TE_M (3), TE_M (4)
#define TE_M5       TE_M (0), TE_M (1), TE_M (2), TE_M (3), TE_M (4), TE_M (5)
#define TE_M6       TE_M (0), TE_M (1), TE_M (2), TE_M (3), TE_M (4), TE_M (5), TE_M (6)

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL double te_eval (const te_expr *x)
{
    if (x) {
    //
    switch (TE_MASK (x->te_type)) {
    //
    case TE_CONSTANT  : return (x->te_value);
    case TE_VARIABLE  : return (*x->te_bound);
    case TE_FUNCTION0 : return ((te_fn0)x->te_fn) ();           // --
    case TE_FUNCTION1 : return ((te_fn1)x->te_fn) (TE_M0);
    case TE_FUNCTION2 : return ((te_fn2)x->te_fn) (TE_M1);
    case TE_FUNCTION3 : return ((te_fn3)x->te_fn) (TE_M2);
    case TE_FUNCTION4 : return ((te_fn4)x->te_fn) (TE_M3);
    case TE_FUNCTION5 : return ((te_fn5)x->te_fn) (TE_M4);
    case TE_FUNCTION6 : return ((te_fn6)x->te_fn) (TE_M5);
    case TE_FUNCTION7 : return ((te_fn7)x->te_fn) (TE_M6);
    default           : return TE_NAN;
    //
    }
    //
    }
    
    return TE_NAN;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#undef TE_M0
#undef TE_M1
#undef TE_M2
#undef TE_M3
#undef TE_M4
#undef TE_M5
#undef TE_M6

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

static void te_optimize (te_expr *x)
{
    if (x->te_type != TE_CONSTANT) {
    if (x->te_type != TE_VARIABLE) {
    //
    if (TE_IS_PURE (x->te_type)) {
    //
    int arity = te_arity (x->te_type);
    
    int i, known = 1;
    
    for (i = 0; i < arity; ++i) {
        te_optimize (x->te_parameters[i]);
        if (x->te_parameters[i]->te_type != TE_CONSTANT) { known = 0; break; }
    }
        
    if (known) {
        double t = te_eval (x);
        te_freeParameters (x);
        x->te_type  = TE_CONSTANT;
        x->te_value = t;
    }
    //
    }
    //
    }
    }
}

PD_LOCAL te_expr *te_compile (const char *expression, const te_variable *variables, int count)
{
    te_state s;
    
    s.x_count     = count;
    s.x_variables = variables;
    s.x_p         = expression;
    
    te_nextToken (&s);
    
    te_expr *x = te_list (&s);

    if (s.x_type != TOK_END) { te_free (x); return NULL; }
    else {
        te_optimize (x); return x;
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
