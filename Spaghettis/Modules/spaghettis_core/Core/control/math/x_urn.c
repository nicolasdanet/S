
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#include "../../m_spaghettis.h"
#include "../../m_core.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

static t_class *urn_class;              /* Shared. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

typedef struct _urn {
    t_object    x_obj;                  /* Must be the first. */
    t_float     x_range;
    int         x_index;
    t_buffer    *x_buffer;
    t_outlet    *x_outletLeft;
    t_outlet    *x_outletRight;
    } t_urn;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#define URN_LIMIT   4096                /* Arbitrary. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

// -- TODO: Use a best approach in case of a large range?

/* < https://meta.stackoverflow.com/questions/334325 > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void urn_range (t_urn *x, t_float f)
{
    x->x_range = PD_CLAMP (1, f, URN_LIMIT);
}

static void urn_set (t_urn *x)
{
    int i, n = (int)x->x_range;
    
    x->x_index = 0;
    
    buffer_clear (x->x_buffer);
    buffer_reserve (x->x_buffer, n);
    
    for (i = 0; i < n; i++) { buffer_appendFloat (x->x_buffer, (t_float)i); }
    
    buffer_shuffle (x->x_buffer);
}

static int urn_next (t_urn *x)
{
    int n = buffer_getSize (x->x_buffer);
    
    if (x->x_index < n) {
        return (int)atom_getFloatAtIndex (x->x_index++, n, buffer_getAtoms (x->x_buffer));
    }
    
    return -1;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void urn_bang (t_urn *x)
{
    int n = urn_next (x);
    
    if (n < 0) { outlet_bang (x->x_outletRight); } else { outlet_float (x->x_outletLeft, (t_float)n); }
}

static void urn_float (t_urn *x, t_float f)
{
    urn_range (x, f); urn_set (x);
    
    if (f > URN_LIMIT) { warning_invalid (cast_object (x), sym_urn, sym_range); }
}

static void urn_clear (t_urn *x)
{
    urn_float (x, x->x_range);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static t_buffer *urn_functionData (t_object *z, int flags)
{
    if (SAVED_DEEP (flags)) {
    //
    t_urn *x = (t_urn *)z;
    t_buffer *b = buffer_new();
    int n = buffer_getSize (x->x_buffer);
    
    buffer_appendSymbol (b, sym__inlet2);
    buffer_appendFloat (b,  x->x_range);
    buffer_appendComma (b);
    buffer_appendSymbol (b, sym__restore);
        
    if (n) { buffer_appendFloat (b, x->x_index); buffer_appendBuffer (b, x->x_buffer); }
    
    return b;
    //
    }
    
    return NULL;
}

static void urn_restoreEncapsulate (t_urn *x, t_urn *old)
{
    x->x_range = old->x_range;
    x->x_index = old->x_index;
    
    t_buffer *t = x->x_buffer; x->x_buffer = old->x_buffer; old->x_buffer = t;
}

static void urn_restoreProceed (t_urn *x, int argc, t_atom *argv)
{
    if (argc > 1) {
    //
    int i = (int)atom_getFloatAtIndex (0, argc, argv);
    
    buffer_clear (x->x_buffer);
    
    x->x_index = PD_MAX (0, i);
    
    buffer_append (x->x_buffer, argc - 1, argv + 1);
    //
    }
}

static void urn_restore (t_urn *x, t_symbol *s, int argc, t_atom *argv)
{
    t_urn *old = (t_urn *)instance_pendingFetch (cast_object (x));

    if (old) { urn_restoreEncapsulate (x, old); } else { urn_restoreProceed (x, argc, argv); }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void *urn_new (t_float f)
{
    t_urn *x = (t_urn *)pd_new (urn_class);
    
    x->x_buffer      = buffer_new();
    x->x_outletLeft  = outlet_newFloat (cast_object (x));
    x->x_outletRight = outlet_newBang (cast_object (x));
    
    inlet_new2 (x, &s_float);
    
    urn_float (x, f);
    
    return x;
}

static void urn_free (t_urn *x)
{
    buffer_free (x->x_buffer);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void urn_setup (void)
{
    t_class *c = NULL;
    
    c = class_new (sym_urn,
            (t_newmethod)urn_new,
            (t_method)urn_free,
            sizeof (t_urn),
            CLASS_DEFAULT,
            A_DEFFLOAT,
            A_NULL);
            
    class_addBang (c, (t_method)urn_bang);
    
    class_addMethod (c, (t_method)urn_clear,    sym_clear,      A_NULL);
    class_addMethod (c, (t_method)urn_float,    sym__inlet2,    A_FLOAT, A_NULL);
    class_addMethod (c, (t_method)urn_restore,  sym__restore,   A_GIMME, A_NULL);

    class_setDataFunction (c, urn_functionData);
    class_requirePending (c);

    urn_class = c;
}

PD_LOCAL void urn_destroy (void)
{
    class_free (urn_class);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
