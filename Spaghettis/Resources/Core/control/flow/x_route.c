
/* Copyright (c) 1997-2020 Miller Puckette and others. */

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

static t_class *route_class;            /* Shared. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

typedef struct _route {
    t_object        x_obj;              /* Must be the first. */
    t_atomtype      x_type;
    int             x_size;
    t_atomoutlet    *x_vector;
    t_outlet        *x_outlet;
    } t_route;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void route_listFailed (t_route *x, int argc, t_atom *argv)
{
    if (!argc) { outlet_bang (x->x_outlet); }
    else if (argc == 1) {
        if (IS_FLOAT (argv)) { outlet_float (x->x_outlet, GET_FLOAT (argv)); }
        else if (IS_SYMBOL (argv)) { outlet_symbol (x->x_outlet, GET_SYMBOL (argv)); }
        else {
            PD_BUG;
        }
    } else {
        outlet_list (x->x_outlet, argc, argv);
    }
}

static int route_listForFloat (t_route *x, int argc, t_atom *argv)
{
    int k = 0;
    
    if (argc && IS_FLOAT (argv)) {
    //
    int i;
    
    for (i = 0; i < x->x_size; i++) {
    
        if (atomoutlet_isEqualToAtom (x->x_vector + i, argv)) {
            t_outlet *outlet = atomoutlet_getOutlet (x->x_vector + i);
            if (argc < 2 || !IS_SYMBOL (argv + 1)) { outlet_list (outlet, argc - 1, argv + 1); }
            else {
                outlet_anything (outlet, GET_SYMBOL (argv + 1), argc - 2, argv + 2);
            }
            k = 1; break;
        }
    }
    //
    }
    
    return k;
}

static int route_listForSymbol (t_route *x, int argc, t_atom *argv)
{
    int i, k = 0;
    
    if (argc && IS_SYMBOL (argv)) {
    //
    for (i = 0; i < x->x_size; i++) {

        if (atomoutlet_isEqualToAtom (x->x_vector + i, argv)) {
            t_outlet *outlet = atomoutlet_getOutlet (x->x_vector + i);
            if (argc < 2 || !IS_SYMBOL (argv + 1)) { outlet_list (outlet, argc - 1, argv + 1); }
            else {
                outlet_anything (outlet, GET_SYMBOL (argv + 1), argc - 2, argv + 2);
            }
            k = 1; break;
        }
    }
    //
    }
    
    if (!k) {
    //
    if (argc > 1) {
        for (i = 0; i < x->x_size; i++) {
        
            if (atom_getSymbol (atomoutlet_getAtom (x->x_vector + i)) == &s_list) {
                t_outlet *outlet = atomoutlet_getOutlet (x->x_vector + i);
                outlet_list (outlet, argc, argv);
                k = 1; break;
            }
        }
        
    } else if (argc == 0) {
        for (i = 0; i < x->x_size; i++) {
        
            if (atom_getSymbol (atomoutlet_getAtom (x->x_vector + i)) == &s_bang) {
                outlet_bang (atomoutlet_getOutlet (x->x_vector + i)); 
                k = 1; break;
            }
        }
        
    } else if (IS_FLOAT (argv)) {
        for (i = 0; i < x->x_size; i++) {

            if (atom_getSymbol (atomoutlet_getAtom (x->x_vector + i)) == &s_float) {
                outlet_float (atomoutlet_getOutlet (x->x_vector + i), GET_FLOAT (argv));
                k = 1; break;
            }
        }
        
    } else if (IS_SYMBOL (argv)) {
        for (i = 0; i < x->x_size; i++) {
        
            if (atom_getSymbol (atomoutlet_getAtom (x->x_vector + i)) == &s_symbol) {
                outlet_symbol (atomoutlet_getOutlet (x->x_vector + i), GET_SYMBOL (argv));
                k = 1; break;
            }
        }
    }
    //
    }
    
    return k;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void route_list (t_route *x, t_symbol *dummy, int argc, t_atom *argv)
{
    int k = 0;
    
    if (x->x_type == A_FLOAT) { k = route_listForFloat (x, argc, argv); }
    else {
        k = route_listForSymbol (x, argc, argv);
    }

    if (!k) { route_listFailed (x, argc, argv); }
}

static void route_anything (t_route *x, t_symbol *s, int argc, t_atom *argv)
{
    int k = 0;
    
    if (x->x_type == A_SYMBOL) {
    //
    int i;
    
    for (i = 0; i < x->x_size; i++) {
    //
    t_atom a; SET_SYMBOL (&a, s);
    
    if (atomoutlet_isEqualToAtom (x->x_vector + i, &a)) {
        t_outlet *outlet = atomoutlet_getOutlet (x->x_vector + i);
        if (!argc || !IS_SYMBOL (argv)) { outlet_list (outlet, argc, argv); }
        else {
            outlet_anything (outlet, GET_SYMBOL (argv), argc - 1, argv + 1);
        } 
        k = 1; break;
    }
    //
    }

    if (!k) {
    //
    for (i = 0; i < x->x_size; i++) {
    //
    if (atom_getSymbol (atomoutlet_getAtom (x->x_vector + i)) == &s_anything) {
        outlet_anything (atomoutlet_getOutlet (x->x_vector + i), s, argc, argv);
        k = 1; break;
    }
    //
    }
    //
    }
    //
    }
    
    if (!k) { outlet_anything (x->x_outlet, s, argc, argv); }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static t_buffer *route_functionData (t_object *z, int flags)
{
    if (SAVED_DEEP (flags)) {
    //
    t_route *x = (t_route *)z;
    
    if (x->x_size == 1) {
    //
    t_buffer *b = buffer_new();
    
    buffer_appendSymbol (b, sym__restore);
    buffer_appendAtom (b, atomoutlet_getAtom (x->x_vector));
    
    return b;
    //
    }
    //
    }
    
    return NULL;
}

static void route_restore (t_route *x, t_symbol *s, int argc, t_atom *argv)
{
    if (x->x_size == 1 && argc == 1) {
    //
    t_route *old = (t_route *)instance_pendingFetch (cast_object (x));
    t_error err  = atomoutlet_setAtom (x->x_vector, old ? atomoutlet_getAtom (old->x_vector) : argv);
    
    PD_ASSERT (!err); PD_UNUSED (err);
    //
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void *route_newProceed (int argc, t_atom *argv)
{
    t_route *x = (t_route *)pd_new (route_class);
    int i;
    int create = (argc == 1) ? ATOMOUTLET_BOTH : ATOMOUTLET_OUTLET;
    
    x->x_type   = atom_getType (argv);
    x->x_size   = argc;
    x->x_vector = (t_atomoutlet *)PD_MEMORY_GET (x->x_size * sizeof (t_atomoutlet));
    
    PD_ASSERT (x->x_type == A_FLOAT || x->x_type == A_SYMBOL);
    
    for (i = 0; i < argc; i++) {
        if (IS_SYMBOL (argv + i)) {
            t_symbol *t = atomoutlet_parseAbbreviated (atom_getSymbol (argv + i));
            atomoutlet_makeSymbol (x->x_vector + i, cast_object (x), create, t);
        } else {
            t_float t = atom_getFloat (argv + i);
            atomoutlet_makeFloat (x->x_vector + i,  cast_object (x), create, t);
        }
    }
    
    x->x_outlet = outlet_newMixed (cast_object (x));
    
    return x;
}

static void *route_new (t_symbol *s, int argc, t_atom *argv)
{
    if (argc == 0) { t_atom a; SET_FLOAT (&a, 0.0); return route_newProceed (1, &a); }
    else {
        return route_newProceed (argc, argv);
    }
}

static void route_free (t_route *x)
{
    PD_MEMORY_FREE (x->x_vector);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void route_setup (void)
{
    t_class *c = NULL;
    
    c = class_new (sym_route,
            (t_newmethod)route_new,
            (t_method)route_free,
            sizeof (t_route),
            CLASS_DEFAULT,
            A_GIMME,
            A_NULL);
            
    class_addList (c, (t_method)route_list);
    class_addAnything (c, (t_method)route_anything);
    
    class_addMethod (c, (t_method)route_restore, sym__restore, A_GIMME, A_NULL);

    class_setDataFunction (c, route_functionData);
    class_requirePending (c);
    
    route_class = c;
}

PD_LOCAL void route_destroy (void)
{
    class_free (route_class);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
