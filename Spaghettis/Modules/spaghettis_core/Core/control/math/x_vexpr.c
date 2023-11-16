
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

static t_class *vexpr_class;    /* Shared. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

typedef t_expr t_vexpr;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void vexpr_fetch (t_vexpr *x, int i)
{
    int j;
    
    for (j = 0; j < (x->x_count - 1); j++) {
    //
    t_listinlet *t = x->x_listinlets + j;
    int n = listinlet_listSize (t);
    t_float f = n ? atom_getFloat (listinlet_getAtomAtIndex (t, i % n)) : 0.0;
    
    x->x_f[j + 1] = f;
    //
    }
    
    for (j = 0; j < EXPR_VARIABLES; j++) { x->x_v[j] = (double)x->x_f[j]; }
}

static void vexpr_proceed (t_vexpr *x)
{
    t_atom *t = NULL;
    int i, size = buffer_getSize (x->x_vector);
    
    PD_ATOMS_ALLOCA (t, size);
    
    for (i = 0; i < size; i++) {
    //
    x->x_f[0] = atom_getFloat (buffer_getAtomAtIndex (x->x_vector, i));
    
    vexpr_fetch (x, i);
    
    {
        t_float f = te_eval (x->x_expression);
    
        SET_FLOAT (t + i, PD_FLOAT64_IS_INVALID_OR_ZERO (f) ? 0.0 : f);
    }
    //
    }
    
    outlet_list (x->x_outlet, size, t);
    
    PD_ATOMS_FREEA (t, size);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void vexpr_bang (t_vexpr *x)
{
    vexpr_proceed (x);
}

static void vexpr_float (t_vexpr *x, t_float f)
{
    buffer_clear (x->x_vector); buffer_appendFloat (x->x_vector, f);
    
    vexpr_bang (x);
}

static void vexpr_list (t_vexpr *x, t_symbol *s, int argc, t_atom *argv)
{
    buffer_clear (x->x_vector); buffer_append (x->x_vector, argc, argv);
    
    vexpr_bang (x);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static t_buffer *vexpr_functionData (t_object *z, int flags)
{
    if (SAVED_DEEP (flags)) {
    //
    t_vexpr *x  = (t_vexpr *)z;
    t_buffer *b = buffer_new();
    int i;
    
    for (i = 0; i < (x->x_count - 1); i++) {
    //
    buffer_appendSymbol (b, sym__set);
    buffer_appendFloat (b, i);
    listinlet_listGet (x->x_listinlets + i, b);
    buffer_appendComma (b);
    //
    }
    
    buffer_appendSymbol (b, sym__restore);
    buffer_appendBuffer (b, x->x_vector);
    
    return b;
    //
    }
    
    return NULL;
}

static void vexpr_set (t_vexpr *x, t_symbol *s, int argc, t_atom *argv)
{
    if (argc > 1) {
    //
    int i = atom_getFloatAtIndex (0, argc, argv);
    
    if (i < x->x_count) { listinlet_listSet (x->x_listinlets + i, argc - 1, argv + 1); }
    //
    }
}

static void vexpr_restore (t_vexpr *x, t_symbol *s, int argc, t_atom *argv)
{
    t_vexpr *old = (t_vexpr *)instance_pendingFetch (cast_object (x));
    
    buffer_clear (x->x_vector);
    
    if (old) { buffer_appendBuffer (x->x_vector, old->x_vector); }
    else {
        buffer_append (x->x_vector, argc, argv);
    }
    
    if (old) {
    //
    int i, n = PD_MIN (x->x_count, old->x_count);
    
    for (i = 0; i < (n - 1); i++) { listinlet_listSetByCopy (x->x_listinlets + i, old->x_listinlets + i); }
    //
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void *vexpr_new (t_symbol *s, int argc, t_atom *argv)
{
    t_vexpr *x = (t_vexpr *)pd_new (vexpr_class);
    
    x->x_count = expr_initializeExpression (x, argc, argv);

    if (x->x_expression) {
        
        int i;
        x->x_outlet = outlet_newList (cast_object (x));
        x->x_vector = buffer_new();
        for (i = 0; i < (x->x_count - 1); i++) {
            listinlet_init (x->x_listinlets + i);
            inlet_new (cast_object (x), cast_pd (x->x_listinlets + i), NULL, NULL);
        }
    
    } else {
    
        error_invalid (cast_object (x), sym_vexpr, sym_expression);
        pd_free (cast_pd (x));
        x = NULL;
    }
    
    return x;
}

static void vexpr_free (t_vexpr *x)
{
    int i; for (i = 0; i < (x->x_count - 1); i++) { listinlet_free (x->x_listinlets + i); }
    
    if (x->x_vector)     { buffer_free (x->x_vector); }
    if (x->x_expression) { te_free (x->x_expression); }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void vexpr_setup (void)
{
    t_class *c = NULL;
    
    c = class_new (sym_vexpr,
            (t_newmethod)vexpr_new,
            (t_method)vexpr_free,
            sizeof (t_vexpr),
            CLASS_DEFAULT,
            A_GIMME,
            A_NULL);
    
    class_addBang (c,  (t_method)vexpr_bang);
    class_addFloat (c, (t_method)vexpr_float);
    class_addList (c,  (t_method)vexpr_list);
    
    class_addMethod (c, (t_method)vexpr_set,     sym__set,      A_GIMME, A_NULL);
    class_addMethod (c, (t_method)vexpr_restore, sym__restore,  A_GIMME, A_NULL);

    class_setDataFunction (c, vexpr_functionData);
    
    class_requirePending (c);
    
    class_setHelpName (c, sym_expr);
    
    vexpr_class = c;
}

void vexpr_destroy (void)
{
    class_free (vexpr_class);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
