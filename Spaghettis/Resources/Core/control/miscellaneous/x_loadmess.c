
/* Copyright (c) 1997-2020 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#include "../../m_spaghettis.h"
#include "../../m_core.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

static t_class *loadmess_class;         /* Shared. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

typedef struct _loadmess {
    t_object    x_obj;                  /* Must be the first. */
    t_buffer    *x_buffer;
    t_outlet    *x_outlet;
    } t_loadmess;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void loadmess_loadbang (t_loadmess *x)
{
    int n = buffer_getSize (x->x_buffer);
    
    if (!n) { outlet_list (x->x_outlet, 0, NULL); }
    else {
    //
    t_symbol *s = atom_getSymbol (buffer_getAtomAtIndex (x->x_buffer, 0));

    if (s != &s_) { outlet_anything (x->x_outlet, s, n - 1, buffer_getAtoms (x->x_buffer) + 1); }
    else {
        outlet_list (x->x_outlet, n, buffer_getAtoms (x->x_buffer));
    }
    //
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void *loadmess_new (t_symbol *s, int argc, t_atom *argv)
{
    t_loadmess *x = (t_loadmess *)pd_new (loadmess_class);
    
    x->x_buffer = buffer_new();
    x->x_outlet = outlet_newMixed (cast_object (x));
    
    buffer_append (x->x_buffer, argc, argv);
    
    return x;
}

static void loadmess_free (t_loadmess *x)
{
    buffer_free (x->x_buffer);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void loadmess_setup (void)
{
    t_class *c = NULL;
    
    c = class_new (sym_loadmess,
            (t_newmethod)loadmess_new,
            (t_method)loadmess_free,
            sizeof (t_loadmess),
            CLASS_DEFAULT | CLASS_NOINLET,
            A_GIMME,
            A_NULL);
    
    class_addLoadbang (c, (t_method)loadmess_loadbang);
    
    loadmess_class = c;
}

PD_LOCAL void loadmess_destroy (void)
{
    class_free (loadmess_class);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
