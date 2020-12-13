
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#include "../../m_spaghettis.h"
#include "../../m_core.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "x_list.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

static t_class *listiterate_class;          /* Shared. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

typedef struct _listiterate {
    t_object    x_obj;                      /* Must be the first. */
    t_outlet    *x_outletLeft;
    t_outlet    *x_outletMiddle;
    t_outlet    *x_outletRight;
    } t_listiterate;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void listiterate_list (t_listiterate *x, t_symbol *s, int argc, t_atom *argv)
{
    int i;
    
    for (i = 0; i < argc; i++) {
    //
    t_atom a = argv[i];
    
    outlet_float (x->x_outletRight, (t_float)i);
    
    if (IS_FLOAT (&a))        { outlet_float (x->x_outletLeft,   GET_FLOAT (&a));   }
    else if (IS_SYMBOL (&a))  { outlet_symbol (x->x_outletLeft,  GET_SYMBOL (&a));  }
    else {
        PD_BUG;
    }
    //
    }
    
    outlet_bang (x->x_outletMiddle);
}

static void listiterate_anything (t_listiterate *x, t_symbol *s, int argc, t_atom *argv)
{
    utils_anythingToList (cast_pd (x), (t_listmethod)listiterate_list, s, argc, argv);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void *listiterate_new (t_symbol *s, int argc, t_atom *argv)
{
    t_listiterate *x = (t_listiterate *)pd_new (listiterate_class);
    
    x->x_outletLeft   = outlet_newMixed (cast_object (x));
    x->x_outletMiddle = outlet_newBang (cast_object (x));
    x->x_outletRight  = outlet_newFloat (cast_object (x));
    
    if (argc) { warning_unusedArguments (s, argc, argv); }
    
    return x;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void listiterate_setup (void)
{
    t_class *c = NULL;
    
    c = class_new (sym_list__space__iterate,
            (t_newmethod)listiterate_new,
            NULL,
            sizeof (t_listiterate),
            CLASS_DEFAULT,
            A_GIMME,
            A_NULL);
            
    class_addList (c, (t_method)listiterate_list);
    class_addAnything (c, (t_method)listiterate_anything);
    
    class_setHelpName (c, &s_list);
    
    listiterate_class = c;
}

PD_LOCAL void listiterate_destroy (void)
{
    class_free (listiterate_class);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

