
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#include "../../m_spaghettis.h"
#include "../../m_core.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "../x_atomoutlet.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

static t_class *trigger_class;          /* Shared. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

typedef struct _trigger {
    t_object        x_obj;              /* Must be the first. */
    int             x_size;
    t_atomoutlet    *x_vector;
    } t_trigger;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void trigger_list (t_trigger *, t_symbol *, int, t_atom *);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void trigger_bang (t_trigger *x)
{
    trigger_list (x, &s_bang, 0, NULL);
}

static void trigger_float (t_trigger *x, t_float f)
{
    t_atom a; SET_FLOAT (&a, f); trigger_list (x, &s_float, 1, &a);
}

static void trigger_symbol (t_trigger *x, t_symbol *s)
{
    t_atom a; SET_SYMBOL (&a, s); trigger_list (x, &s_symbol, 1, &a);
}

static void trigger_list (t_trigger *x, t_symbol *s, int argc, t_atom *argv)
{
    int i;
    
    for (i = x->x_size - 1; i >= 0; i--) {
    //
    t_symbol *type = atom_getSymbol (atomoutlet_getAtom (x->x_vector + i));
    t_outlet *outlet = atomoutlet_getOutlet (x->x_vector + i);
    
    if (type == &s_) { outlet_float (outlet, atom_getFloat (atomoutlet_getAtom (x->x_vector + i))); }
    else {
    //
    if (type == &s_float)       { outlet_float (outlet, atom_getFloatAtIndex (0, argc, argv)); }
    else if (type == &s_bang)   { outlet_bang (outlet); }
    else if (type == &s_symbol) {
        t_symbol *t = atom_getSymbolAtIndex (0, argc, argv); outlet_symbol (outlet, t == &s_ ? s : t);
    
    } else {
        if (type == &s_anything) {
            if (argc && IS_SYMBOL (argv)) {
                outlet_anything (outlet, atom_getSymbol (argv), argc - 1, argv + 1);
            } else {
                outlet_list (outlet, argc, argv);
            }
        } else if (type == &s_list) {
            outlet_list (outlet, argc, argv);
        } else {
            outlet_anything (outlet, type, 0, NULL);
        }
    }
    //
    }
    //
    }
}

static void trigger_anything (t_trigger *x, t_symbol *s, int argc, t_atom *argv)
{
    utils_anythingToList (cast_pd (x), (t_listmethod)trigger_list, s, argc, argv);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void *trigger_newProceed (int argc, t_atom *argv)
{
    t_trigger *x = (t_trigger *)pd_new (trigger_class);
    int i;

    x->x_size   = argc;
    x->x_vector = (t_atomoutlet *)PD_MEMORY_GET (x->x_size * sizeof (t_atomoutlet));
    
    for (i = 0; i < argc; i++) {
        if (IS_SYMBOL (argv + i)) {
            atomoutlet_makeSymbolParsed (x->x_vector + i, cast_object (x), ATOMOUTLET_OUTLET, argv + i);
        } else {
            atomoutlet_make (x->x_vector + i, cast_object (x), ATOMOUTLET_OUTLET, argv + i);
        }
    }
    
    return x;
}

static void *trigger_new (t_symbol *s, int argc, t_atom *argv)
{
    if (argc) { return trigger_newProceed (argc, argv); }
    else {
        t_atom a; SET_SYMBOL (&a, &s_bang); return trigger_newProceed (1, &a);
    }
}

static void trigger_free (t_trigger *x)
{
    PD_MEMORY_FREE (x->x_vector);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void trigger_setup (void)
{
    t_class *c = NULL;
    
    c = class_new (sym_trigger,
            (t_newmethod)trigger_new,
            (t_method)trigger_free,
            sizeof (t_trigger),
            CLASS_DEFAULT,
            A_GIMME,
            A_NULL);
            
    class_addCreator ((t_newmethod)trigger_new, sym_t, A_GIMME, A_NULL);
    
    class_addBang (c, (t_method)trigger_bang);
    class_addFloat (c, (t_method)trigger_float);
    class_addSymbol (c, (t_method)trigger_symbol);
    class_addList (c, (t_method)trigger_list);
    class_addAnything (c, (t_method)trigger_anything);
    
    trigger_class = c;
}

void trigger_destroy (void)
{
    class_free (trigger_class);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
