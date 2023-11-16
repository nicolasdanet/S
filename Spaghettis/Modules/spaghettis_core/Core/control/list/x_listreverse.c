
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

static t_class *listreverse_class;          /* Shared. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

typedef struct _listreverse {
    t_object    x_obj;                      /* Must be the first. */
    t_outlet    *x_outlet;
    } t_listreverse;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void listreverse_list (t_listreverse *x, t_symbol *s, int argc, t_atom *argv)
{
    if (!argc) { outlet_list (x->x_outlet, 0, NULL); }
    else {
    //
    t_atom *t = NULL;
    
    PD_ATOMS_ALLOCA (t, argc);
    
    int i; for (i = 0; i < argc; i++) { atom_copyAtom (argv + i, t + argc - 1 - i); }
    
    outlet_list (x->x_outlet, argc, t);
    
    PD_ATOMS_FREEA (t, argc);
    //
    }
}

static void listreverse_anything (t_listreverse *x, t_symbol *s, int argc, t_atom *argv)
{
    utils_anythingToList (cast_pd (x), (t_listmethod)listreverse_list, s, argc, argv);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void *listreverse_new (t_symbol *s, int argc, t_atom *argv)
{
    t_listreverse *x = (t_listreverse *)pd_new (listreverse_class);
    
    x->x_outlet = outlet_newList (cast_object (x));
    
    if (argc) { warning_unusedArguments (cast_object (x), s, argc, argv); }
    
    return x;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void listreverse_setup (void)
{
    t_class *c = NULL;
    
    c = class_new (sym_list__space__reverse,
            (t_newmethod)listreverse_new,
            NULL,
            sizeof (t_listreverse),
            CLASS_DEFAULT,
            A_GIMME,
            A_NULL);
            
    class_addList (c, (t_method)listreverse_list);
    class_addAnything (c, (t_method)listreverse_anything);
    
    class_setHelpName (c, &s_list);
    
    listreverse_class = c;
}

void listreverse_destroy (void)
{
    class_free (listreverse_class);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

