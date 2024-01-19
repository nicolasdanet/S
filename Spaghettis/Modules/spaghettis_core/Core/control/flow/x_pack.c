
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#include "../../m_headers.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "../x_atomoutlet.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

static t_class *pack_class;         /* Shared. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

typedef struct _pack {
    t_object        x_obj;          /* Must be the first. */
    int             x_size;
    t_atomoutlet    *x_vector;
    t_outlet        *x_outlet;
    } t_pack;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void pack_bang (t_pack *x)
{
    t_atom *a = NULL;
    int i;
        
    PD_ATOMS_ALLOCA (a, x->x_size);
    
    for (i = 0; i < x->x_size; i++) { atomoutlet_copyAtom (x->x_vector + i, a + i); }
    
    outlet_list (x->x_outlet, x->x_size, a);
    
    PD_ATOMS_FREEA (a, x->x_size);
}

static void pack_float (t_pack *x, t_float f)
{
    t_atom a; SET_FLOAT (&a, f);
    
    if (atomoutlet_setAtom (x->x_vector + 0, &a)) { warning_badType (cast_object (x), sym_pack, &s_float); }
    
    pack_bang (x);
}

static void pack_symbol (t_pack *x, t_symbol *s)
{
    t_atom a; SET_SYMBOL (&a, s);
    
    if (atomoutlet_setAtom (x->x_vector + 0, &a)) { warning_badType (cast_object (x), sym_pack, &s_symbol); }
    
    pack_bang (x);
}

static void pack_list (t_pack *x, t_symbol *s, int argc, t_atom *argv)
{
    object_distributeAtomsOnInlets (cast_object (x), argc, argv);
}

static void pack_anything (t_pack *x, t_symbol *s, int argc, t_atom *argv)
{
    utils_anythingToList (cast_pd (x), (t_listmethod)pack_list, s, argc, argv);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static t_buffer *pack_functionData (t_object *z, int flags)
{
    if (SAVED_DEEP (flags)) {
    //
    t_pack *x   = (t_pack *)z;
    t_buffer *b = buffer_new();
    int i;
    
    buffer_appendSymbol (b, sym__restore);
    
    for (i = 0; i < x->x_size; i++) { buffer_appendAtom (b, atomoutlet_getAtom (x->x_vector + i)); }
    
    return b;
    //
    }
    
    return NULL;
}

static void pack_restore (t_pack *x, t_symbol *s, int argc, t_atom *argv)
{
    t_pack *old = (t_pack *)instance_pendingFetch (cast_object (x));

    int i;
    
    PD_ASSERT (!old || (x->x_size == old->x_size));
    
    for (i = 0; i < PD_MIN (x->x_size, argc); i++) {
    //
    t_atom *a   = old ? atomoutlet_getAtom (old->x_vector + i) : (argv + i);
    t_error err = atomoutlet_setAtom (x->x_vector + i, a);
    
    PD_ASSERT (!err); PD_UNUSED (err);
    //
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void *pack_newProceed (t_symbol *s, int argc, t_atom *argv)
{
    t_pack *x = (t_pack *)pd_new (pack_class);
    int i;
    
    x->x_size   = argc;
    x->x_vector = (t_atomoutlet *)PD_MEMORY_GET (x->x_size * sizeof (t_atomoutlet));

    for (i = 0; i < x->x_size; i++) {
        int create = (i != 0) ? ATOMOUTLET_INLET : ATOMOUTLET_NONE;
        atomoutlet_makeParsed (x->x_vector + i, cast_object (x), create, argv + i);
        if (s == sym_pak) {
            if (create == ATOMOUTLET_INLET) {
                inlet_setHot (atomoutlet_getInlet (x->x_vector + i));
            }
        }
    }
    
    x->x_outlet = outlet_newList (cast_object (x));
    
    return x;
}

static void *pack_new (t_symbol *s, int argc, t_atom *argv)
{
    if (argc) { return pack_newProceed (s, argc, argv); }
    else {
        t_atom a[2];
        SET_FLOAT (&a[0], 0.0);
        SET_FLOAT (&a[1], 0.0);
        return pack_newProceed (s, 2, a);
    }
}

static void pack_free (t_pack *x)
{
    PD_MEMORY_FREE (x->x_vector);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void pack_setup (void)
{
    t_class *c = NULL;
    
    c = class_new (sym_pack,
            (t_newmethod)pack_new,
            (t_method)pack_free,
            sizeof (t_pack),
            CLASS_DEFAULT,
            A_GIMME,
            A_NULL);

    class_addCreator ((t_newmethod)pack_new, sym_pak, A_GIMME, A_NULL);
    
    class_addBang (c, (t_method)pack_bang);
    class_addFloat (c, (t_method)pack_float);
    class_addSymbol (c, (t_method)pack_symbol);
    class_addList (c, (t_method)pack_list);
    class_addAnything (c, (t_method)pack_anything);
    
    class_addMethod (c, (t_method)pack_restore, sym__restore, A_GIMME, A_NULL);

    class_setDataFunction (c, pack_functionData);
    class_requirePending (c);
    
    pack_class = c;
}

void pack_destroy (void)
{
    class_free (pack_class);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
