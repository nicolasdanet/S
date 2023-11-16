
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#include "../../m_spaghettis.h"
#include "../../m_core.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

static t_class *tabread4_class;         /* Shared. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

typedef struct _tabread4 {
    t_object    x_obj;                  /* Must be the first. */
    t_symbol    *x_name;
    t_outlet    *x_outlet;
    } t_tabread4;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void tabread4_float (t_tabread4 *x, t_float f)
{
    t_garray *a = garray_fetch (x->x_name);
    
    if (!a) { error_canNotFind (cast_object (x), sym_tabread4, x->x_name); }
    else {
    //
    int size = 0;
    t_word *data = NULL;
    int n = (int)f;
    
    garray_getData (a, &size, &data);
    
    if (size < 4)           { outlet_float (x->x_outlet, 0.0); }
    else if (n < 1)         { outlet_float (x->x_outlet, w_getFloat (data + 1)); }
    else if (n > size - 3)  { outlet_float (x->x_outlet, w_getFloat (data + size - 2)); }
    else {
        outlet_float (x->x_outlet, dsp_4PointsInterpolationWithWords ((f - n), data + n - 1));
    }
    //
    }
}

static void tabread4_set (t_tabread4 *x, t_symbol *s)
{
    x->x_name = s;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static t_buffer *tabread4_functionData (t_object *z, int flags)
{
    if (SAVED_DEEP (flags)) {
    //
    t_tabread4 *x = (t_tabread4 *)z;
    t_buffer *b = buffer_new();
    
    buffer_appendSymbol (b, sym_set);
    buffer_appendSymbol (b, x->x_name);
    buffer_appendComma (b);
    buffer_appendSymbol (b, sym__restore);
    
    return b;
    //
    }
    
    return NULL;
}

static void tabread4_restore (t_tabread4 *x)
{
    t_tabread4 *old = (t_tabread4 *)instance_pendingFetch (cast_object (x));

    if (old) { tabread4_set (x, old->x_name); }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void *tabread4_new (t_symbol *s)
{
    t_tabread4 *x = (t_tabread4 *)pd_new (tabread4_class);
    
    x->x_name   = s;
    x->x_outlet = outlet_newFloat (cast_object (x));
    
    return x;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void tabread4_setup (void)
{
    t_class *c = NULL;
    
    c = class_new (sym_tabread4,
            (t_newmethod)tabread4_new,
            NULL,
            sizeof (t_tabread4),
            CLASS_DEFAULT,
            A_DEFSYMBOL,
            A_NULL);
            
    class_addFloat (c, (t_method)tabread4_float);
    
    class_addMethod (c, (t_method)tabread4_set,     sym_set,        A_SYMBOL, A_NULL);
    class_addMethod (c, (t_method)tabread4_restore, sym__restore,   A_NULL);

    class_setDataFunction (c, tabread4_functionData);
    class_requirePending (c);

    tabread4_class = c;
}

void tabread4_destroy (void)
{
    class_free (tabread4_class);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
