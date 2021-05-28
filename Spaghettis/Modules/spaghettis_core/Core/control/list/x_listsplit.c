
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#include "../../m_spaghettis.h"
#include "../../m_core.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

static t_class *listsplit_class;        /* Shared. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

typedef struct _listsplit {
    t_object    x_obj;                  /* Must be the first. */
    t_float     x_f;
    t_outlet    *x_outletLeft;
    t_outlet    *x_outletMiddle;
    t_outlet    *x_outletRight;
}   t_listsplit;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void listsplit_list (t_listsplit *x, t_symbol *s, int argc, t_atom *argv)
{
    int n = (int)PD_MAX (0.0, x->x_f);
    
    if (argc >= n) {
        outlet_list (x->x_outletMiddle, argc - n, argv + n);
        outlet_list (x->x_outletLeft,   n, argv);
    } else {
        outlet_list (x->x_outletRight,  argc, argv);
    }
}

static void listsplit_anything (t_listsplit *x, t_symbol *s, int argc, t_atom *argv)
{
    utils_anythingToList (cast_pd (x), (t_listmethod)listsplit_list, s, argc, argv);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static t_buffer *listsplit_functionData (t_object *z, int flags)
{
    if (SAVED_DEEP (flags)) {
    //
    t_listsplit *x = (t_listsplit *)z;
    t_buffer *b = buffer_new();
    
    buffer_appendSymbol (b, sym__restore);
    buffer_appendFloat (b, x->x_f);
    
    return b;
    //
    }
    
    return NULL;
}

static void listsplit_restore (t_listsplit *x, t_float f)
{
    t_listsplit *old = (t_listsplit *)instance_pendingFetch (cast_object (x));
    
    x->x_f = old ? old->x_f : f;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void *listsplit_new (t_symbol *s, int argc, t_atom *argv)
{
    t_listsplit *x = (t_listsplit *)pd_new (listsplit_class);
    
    x->x_f = atom_getFloatAtIndex (0, argc, argv);
    
    x->x_outletLeft   = outlet_newList (cast_object (x));
    x->x_outletMiddle = outlet_newList (cast_object (x));
    x->x_outletRight  = outlet_newList (cast_object (x));
    
    inlet_newFloat (cast_object (x), &x->x_f);
    
    if (argc > 1) { warning_unusedArguments (cast_object (x), s, argc - 1, argv + 1); }
    
    return x;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void listsplit_setup (void)
{
    t_class *c = NULL;
    
    c = class_new (sym_list__space__split,
            (t_newmethod)listsplit_new,
            NULL,
            sizeof (t_listsplit),
            CLASS_DEFAULT,
            A_GIMME,
            A_NULL);
            
    class_addList (c, (t_method)listsplit_list);
    class_addAnything (c, (t_method)listsplit_anything);
    
    class_addMethod (c, (t_method)listsplit_restore, sym__restore, A_FLOAT, A_NULL);

    class_setDataFunction (c, listsplit_functionData);
    class_requirePending (c);
    
    class_setHelpName (c, &s_list);
    
    listsplit_class = c;
}

PD_LOCAL void listsplit_destroy (void)
{
    class_free (listsplit_class);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

