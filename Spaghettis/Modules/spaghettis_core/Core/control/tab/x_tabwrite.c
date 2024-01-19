
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#include "../../m_headers.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

static t_class *tabwrite_class;         /* Shared. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

typedef struct _tabwrite {
    t_object    x_obj;                  /* Must be the first. */
    t_float     x_index;
    t_symbol    *x_name;
    } t_tabwrite;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void tabwrite_float (t_tabwrite *x, t_float f)
{
    t_garray *a = garray_fetch (x->x_name);
    
    if (!a) { error_canNotFind (cast_object (x), sym_tabwrite, x->x_name); }
    else {
        garray_setFloatAtIndex (a, x->x_index, f);
    }
}

static void tabwrite_set (t_tabwrite *x, t_symbol *s)
{
    x->x_name = s;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static t_buffer *tabwrite_functionData (t_object *z, int flags)
{
    if (SAVED_DEEP (flags)) {
    //
    t_tabwrite *x = (t_tabwrite *)z;
    t_buffer *b = buffer_new();
    
    buffer_appendSymbol (b, sym_set);
    buffer_appendSymbol (b, x->x_name);
    buffer_appendComma (b);
    buffer_appendSymbol (b, sym__restore);
    buffer_appendFloat (b, x->x_index);
    
    return b;
    //
    }
    
    return NULL;
}

static void tabwrite_restore (t_tabwrite *x, t_float f)
{
    t_tabwrite *old = (t_tabwrite *)instance_pendingFetch (cast_object (x));

    if (old) { x->x_index = old->x_index; tabwrite_set (x, old->x_name); }
    else {
        x->x_index = f;
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void *tabwrite_new (t_symbol *s)
{
    t_tabwrite *x = (t_tabwrite *)pd_new (tabwrite_class);
    
    x->x_name = s;
    
    inlet_newFloat (cast_object (x), &x->x_index);
    
    return x;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void tabwrite_setup (void)
{
    t_class *c = NULL;
    
    c = class_new (sym_tabwrite,
            (t_newmethod)tabwrite_new,
            NULL,
            sizeof (t_tabwrite),
            CLASS_DEFAULT,
            A_DEFSYMBOL,
            A_NULL);
            
    class_addFloat (c, (t_method)tabwrite_float);
    
    class_addMethod (c, (t_method)tabwrite_set,     sym_set,        A_SYMBOL, A_NULL);
    class_addMethod (c, (t_method)tabwrite_restore, sym__restore,   A_FLOAT, A_NULL);

    class_setDataFunction (c, tabwrite_functionData);
    class_requirePending (c);

    tabwrite_class = c;
}

void tabwrite_destroy (void)
{
    class_free (tabwrite_class);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
