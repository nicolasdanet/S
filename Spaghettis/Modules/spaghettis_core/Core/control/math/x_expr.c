
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#include "../../m_spaghettis.h"
#include "../../m_core.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

/* < https://github.com/codeplea/tinyexpr > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "../x_tinyexpr.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "x_expr.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

static t_class *expr_class;     /* Shared. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

static void expr_initializeVariables (t_expr *x)
{
    int i;
    
    for (i = 0; i < EXPR_VARIABLES; i++) {
        char t[PD_STRING] = { 0 };
        string_sprintf (t, PD_STRING, "vf%d", i + 1);
        EXPR_TE_VARIABLE (i, gensym (t));
    }
}

static int expr_getNumberOfVariables (char *expression)
{
    int i, n = 0;
    
    for (i = 0; i < EXPR_VARIABLES; i++) {
        char t[PD_STRING] = { 0 };
        string_sprintf (t, PD_STRING, "$f%d", i + 1);
        if (string_contains (expression, t)) { n = i + 1; }
    }
    
    return n;
}

PD_LOCAL int expr_initializeExpression (t_expr *x, int argc, t_atom *argv)
{
    char *z  = (char *)"0";
    char *t  = argc ? atom_atomsToString (argc, argv) : z;
    int size = expr_getNumberOfVariables (t);
    
    expr_initializeVariables (x);
    string_replaceCharacter (t, '$', 'v');
    x->x_expression = te_compile (t, x->x_variables, size);
        
    if (argc) { PD_MEMORY_FREE (t); }
    
    return size;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void expr_bang (t_expr *x)
{
    int i; for (i = 0; i < EXPR_VARIABLES; i++) { x->x_v[i] = (double)x->x_f[i]; }
    
    {
        double f = te_eval (x->x_expression);
    
        outlet_float (x->x_outlet, PD_FLOAT64_IS_INVALID_OR_ZERO (f) ? 0.0 : f);
    }
}

static void expr_float (t_expr *x, t_float f)
{
    x->x_f[0] = f; expr_bang (x);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static t_buffer *expr_functionData (t_object *z, int flags)
{
    if (SAVED_DEEP (flags)) {
    //
    t_expr *x   = (t_expr *)z;
    t_buffer *b = buffer_new();
    int i;
    
    buffer_appendSymbol (b, sym__restore);
    
    for (i = 0; i < EXPR_VARIABLES; i++) { buffer_appendFloat (b, x->x_f[i]); }
        
    return b;
    //
    }
    
    return NULL;
}

static void expr_restore (t_expr *x, t_symbol *s, int argc, t_atom *argv)
{
    t_expr *old = (t_expr *)instance_pendingFetch (cast_object (x));

    int i;
    
    for (i = 0; i < EXPR_VARIABLES; i++) {
    //
    x->x_f[i] = old ? old->x_f[i] : atom_getFloatAtIndex (i, argc, argv);
    //
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void *expr_new (t_symbol *s, int argc, t_atom *argv)
{
    t_expr *x  = (t_expr *)pd_new (expr_class);
    
    x->x_count = expr_initializeExpression (x, argc, argv);
    
    if (x->x_expression) {
        
        int i;
        x->x_outlet = outlet_newFloat (cast_object (x));
        for (i = 1; i < x->x_count; i++) { inlet_newFloat (cast_object (x), &x->x_f[i]); }
    
    } else {
    
        error_invalid (sym_expr, sym_expression);
        pd_free (cast_pd (x));
        x = NULL;
    }
    
    return x;
}

static void expr_free (t_expr *x)
{
    if (x->x_expression) { te_free (x->x_expression); }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void expr_setup (void)
{
    t_class *c = NULL;
    
    c = class_new (sym_expr,
            (t_newmethod)expr_new,
            (t_method)expr_free,
            sizeof (t_expr),
            CLASS_DEFAULT,
            A_GIMME,
            A_NULL);
    
    class_addBang (c, (t_method)expr_bang);
    class_addFloat (c, (t_method)expr_float);
    
    class_addMethod (c, (t_method)expr_restore, sym__restore, A_GIMME, A_NULL);

    class_setDataFunction (c, expr_functionData);
    class_requirePending (c);
    
    expr_class = c;
}

PD_LOCAL void expr_destroy (void)
{
    class_free (expr_class);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
