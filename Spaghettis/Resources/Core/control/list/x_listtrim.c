
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#include "../../m_spaghettis.h"
#include "../../m_core.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

static t_class *listtrim_class;         /* Shared. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

typedef struct _listtrim {
    t_object    x_obj;                  /* Must be the first. */
    t_outlet    *x_outlet;
    } t_listtrim;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void listtrim_list (t_listtrim *x, t_symbol *s, int argc, t_atom *argv)
{
    if (!argc || !IS_SYMBOL (argv)) { outlet_list (x->x_outlet, argc, argv); }
    else { 
        outlet_anything (x->x_outlet, GET_SYMBOL (argv), argc - 1, argv + 1);
    }
}

static void listtrim_anything (t_listtrim *x, t_symbol *s, int argc, t_atom *argv)
{
    outlet_anything (x->x_outlet, s, argc, argv);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void *listtrim_new (t_symbol *s, int argc, t_atom *argv)
{
    t_listtrim *x = (t_listtrim *)pd_new (listtrim_class);
    
    x->x_outlet = outlet_newMixed (cast_object (x));
    
    if (argc) { warning_unusedArguments (s, argc, argv); }
    
    return x;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void listtrim_setup (void)
{
    t_class *c = NULL;
    
    c = class_new (sym_list__space__trim,
            (t_newmethod)listtrim_new,
            NULL,
            sizeof (t_listtrim),
            CLASS_DEFAULT,
            A_GIMME,
            A_NULL);
            
    class_addList (c, (t_method)listtrim_list);
    class_addAnything (c, (t_method)listtrim_anything);
    
    class_setHelpName (c, &s_list);
    
    listtrim_class = c;
}

PD_LOCAL void listtrim_destroy (void)
{
    class_free (listtrim_class);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

