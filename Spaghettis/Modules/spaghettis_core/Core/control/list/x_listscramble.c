
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

static t_class *listscramble_class;         /* Shared. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

typedef struct _listscramble {
    t_object    x_obj;                      /* Must be the first. */
    t_outlet    *x_outlet;
    } t_listscramble;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void listscramble_list (t_listscramble *x, t_symbol *s, int argc, t_atom *argv)
{
    if (!argc) { outlet_list (x->x_outlet, 0, NULL); }
    else {
    //
    t_atom *t = NULL;
    
    PD_ATOMS_ALLOCA (t, argc);
    
    atom_copyAtoms (argv, argc, t, argc); atom_shuffle (argc, t);
    
    outlet_list (x->x_outlet, argc, t);
    
    PD_ATOMS_FREEA (t, argc);
    //
    }
}

static void listscramble_anything (t_listscramble *x, t_symbol *s, int argc, t_atom *argv)
{
    utils_anythingToList (cast_pd (x), (t_listmethod)listscramble_list, s, argc, argv);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void *listscramble_new (t_symbol *s, int argc, t_atom *argv)
{
    t_listscramble *x = (t_listscramble *)pd_new (listscramble_class);
    
    x->x_outlet = outlet_newList (cast_object (x));
    
    if (argc) { warning_unusedArguments (s, argc, argv); }
    
    return x;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void listscramble_setup (void)
{
    t_class *c = NULL;
    
    c = class_new (sym_list__space__scramble,
            (t_newmethod)listscramble_new,
            NULL,
            sizeof (t_listscramble),
            CLASS_DEFAULT,
            A_GIMME,
            A_NULL);
            
    class_addList (c, (t_method)listscramble_list);
    class_addAnything (c, (t_method)listscramble_anything);
    
    class_setHelpName (c, &s_list);
    
    listscramble_class = c;
}

PD_LOCAL void listscramble_destroy (void)
{
    class_free (listscramble_class);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

