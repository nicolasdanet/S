
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

static t_class *unpack_class;       /* Shared. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

typedef struct _unpack {
    t_object        x_obj;          /* Must be the first. */
    int             x_size;
    t_atomoutlet    *x_vector;
    } t_unpack;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void unpack_list (t_unpack *x, t_symbol *s, int argc, t_atom *argv)
{
    int i;
    
    for (i = PD_MIN (x->x_size, argc) - 1; i >= 0; i--) {
        if (atomoutlet_broadcastIfTypeMatch (x->x_vector + i, argv + i)) { 
            error_mismatch (cast_object (x), sym_unpack, sym_type);
        }
    }
}

static void unpack_anything (t_unpack *x, t_symbol *s, int argc, t_atom *argv)
{
    utils_anythingToList (cast_pd (x), (t_listmethod)unpack_list, s, argc, argv);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void *unpack_newProceed (int argc, t_atom *argv)
{
    t_unpack *x = (t_unpack *)pd_new (unpack_class);
    int i;

    x->x_size   = argc;
    x->x_vector = (t_atomoutlet *)PD_MEMORY_GET (x->x_size * sizeof (t_atomoutlet));
    
    for (i = 0; i < x->x_size; i++) {
        atomoutlet_makeParsed (x->x_vector + i, cast_object (x), ATOMOUTLET_OUTLET, argv + i);
    }
    
    return x;
}

static void *unpack_new (t_symbol *s, int argc, t_atom *argv)
{
    if (argc) { return unpack_newProceed (argc, argv); }
    else {
        t_atom a[2];
        SET_FLOAT (&a[0], 0.0);
        SET_FLOAT (&a[1], 0.0);
        return unpack_newProceed (2, a);
    }
}

static void unpack_free (t_unpack *x)
{
    PD_MEMORY_FREE (x->x_vector);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void unpack_setup (void)
{
    t_class *c = NULL;
    
    c = class_new (sym_unpack,
            (t_newmethod)unpack_new,
            (t_method)unpack_free,
            sizeof (t_unpack),
            CLASS_DEFAULT,
            A_GIMME,
            A_NULL);
            
    class_addList (c, (t_method)unpack_list);
    class_addAnything (c, (t_method)unpack_anything);
    
    unpack_class = c;
}

PD_LOCAL void unpack_destroy (void)
{
    class_free (unpack_class);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
