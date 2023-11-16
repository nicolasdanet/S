
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

/* http://CodePlea.com */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

/* Notice that this file is a largely modified version of original linked below. */

/* < https://github.com/codeplea/tinyexpr/blob/ffb0d41b13e5f8d318db95feb071c220c134fe70/tinyexpr.h > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#ifndef X_TINYEXPR_H_
#define X_TINYEXPR_H_

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#define TE_PARAMETERS   7

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

typedef struct te_expr {
    int                 te_type;
    double              te_value;
    const double        *te_bound;
    const void          *te_fn;
    struct te_expr      *te_parameters[TE_PARAMETERS];
    } te_expr;

typedef struct te_variable {
    const char          *te_name;
    const void          *te_address;
    int                 te_type;
    } te_variable;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

enum {
    TE_VARIABLE         = 0,
    TE_CONSTANT         = 1,
    TE_FUNCTION0        = 8,
    TE_FUNCTION1        = 9,
    TE_FUNCTION2        = 10,
    TE_FUNCTION3        = 11,
    TE_FUNCTION4        = 12,
    TE_FUNCTION5        = 13,
    TE_FUNCTION6        = 14,
    TE_FUNCTION7        = 15,
    TE_PURE             = 32
    };

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

te_expr    *te_compile (const char *expression, const te_variable *variables, int count);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

double     te_eval     (const te_expr *x);
void       te_free     (te_expr *x);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#if defined ( PD_BUILDING_TESTS )

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

/* Just for unit tests. */

static inline double te_interpret (const char *expression, const te_variable *variables, int count)
{
    te_expr *x = te_compile (expression, variables, count);
    
    if (x) { double f = te_eval (x); te_free (x); return f; }
    
    return 0;
}

static inline double te_interpret (const char *expression)
{
    return te_interpret (expression, NULL, 0);
}

static inline double te_raw (const char *expression)
{
    te_expr *x = te_compile (expression, NULL, 0);
    
    if (x) { double f = x->te_value; te_free (x); return f; }
    
    return 0;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#endif // PD_BUILDING_TESTS

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#endif // X_TINYEXPR_H_
