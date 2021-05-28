
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

/* Is there any sense to manage pointers in the future? */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

static t_class *select1_class;                  /* Shared. */
static t_class *select2_class;                  /* Shared. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

typedef struct _select1 {
    t_object            x_obj;                  /* Must be the first. */
    t_atom              x_atom;
    t_outlet            *x_outletLeft;
    t_outlet            *x_outletRight;
    } t_select1;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

typedef struct _select2 {
    t_object            x_obj;                  /* Must be the first. */
    int                 x_size;
    t_atomoutlet        *x_vector;
    t_outlet            *x_outlet;
    } t_select2;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void select1_float (t_select1 *x, t_float f)
{
    if (IS_FLOAT (&x->x_atom) && (f == GET_FLOAT (&x->x_atom))) { outlet_bang (x->x_outletLeft); }
    else {
        outlet_float (x->x_outletRight, f);
    }
}

static void select1_symbol (t_select1 *x, t_symbol *s)
{
    if (IS_SYMBOL (&x->x_atom) && (s == GET_SYMBOL (&x->x_atom))) { outlet_bang (x->x_outletLeft); }
    else { 
        outlet_symbol (x->x_outletRight, s);
    }
}

static void select1_list (t_select1 *x, t_symbol *s, int argc, t_atom *argv)
{
    t_error err = PD_ERROR_NONE;
    
    if (argc > 1) {
        err = !atom_typesAreEquals (argv + 1, &x->x_atom); if (!err) { atom_copyAtom (argv + 1, &x->x_atom); }
    }
    
    if (argc) {
        if (IS_FLOAT (argv)) { select1_float (x, GET_FLOAT (argv)); }
        else if (IS_SYMBOL (argv)) { select1_symbol (x, GET_SYMBOL (argv)); }
        else {
            err = PD_ERROR;
        }
    }
    
    if (err) { error_invalid (cast_object (x), sym_select, sym_type); }
}

static void select1_anything (t_select1 *x, t_symbol *s, int argc, t_atom *argv)
{
    utils_anythingToList (cast_pd (x), (t_listmethod)select1_list, s, argc, argv);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static t_buffer *select1_functionData (t_object *z, int flags)
{
    if (SAVED_DEEP (flags)) {
    //
    t_select1 *x = (t_select1 *)z;
    t_buffer *b  = buffer_new();
    
    buffer_appendSymbol (b, sym__restore);
    buffer_appendAtom (b, &x->x_atom);
    
    return b;
    //
    }
    
    return NULL;
}

static void select1_restore (t_select1 *x, t_symbol *s, int argc, t_atom *argv)
{
    t_select1 *old = (t_select1 *)instance_pendingFetch (cast_object (x));
    
    if (old) {
        atom_copyAtom (&old->x_atom, &x->x_atom);
    } else if (argc == 1) {
        atom_copyAtom (argv, &x->x_atom);
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void select2_float (t_select2 *x, t_float f)
{
    int i, k = 0;
    
    for (i = x->x_size - 1; i >= 0; i--) {

        t_atom a; SET_FLOAT (&a, f); 
        
        if (atomoutlet_isEqualToAtom (x->x_vector + i, &a)) {
            k |= 1; outlet_bang (atomoutlet_getOutlet (x->x_vector + i));
        }
    }
    
    if (!k) { outlet_float (x->x_outlet, f); }
}

static void select2_symbol (t_select2 *x, t_symbol *s)
{
    int i, k = 0;
    
    for (i = x->x_size - 1; i >= 0; i--) {

        t_atom a; SET_SYMBOL (&a, s); 
        
        if (atomoutlet_isEqualToAtom (x->x_vector + i, &a)) {
            k |= 1; outlet_bang (atomoutlet_getOutlet (x->x_vector + i));
        }
    }
    
    if (!k) { outlet_symbol (x->x_outlet, s); }
}

static void select2_anything (t_select2 *x, t_symbol *s, int argc, t_atom *argv)
{
    if (s != &s_list) { select2_symbol (x, s); }
    else if (argc) {
        if (IS_SYMBOL (argv))     { select2_symbol (x, GET_SYMBOL (argv)); }
        else if (IS_FLOAT (argv)) { select2_float (x, GET_FLOAT (argv)); }
        else {
            error_invalid (cast_object (x), sym_select, sym_type);
        }
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void *select1_new (int argc, t_atom *argv)
{
    t_select1 *x = (t_select1 *)pd_new (select1_class);
    
    atom_copyAtom (argv, &x->x_atom);
    
    x->x_outletLeft = outlet_newBang (cast_object (x));
    
    if (IS_FLOAT (argv)) { x->x_outletRight = outlet_newMixed (cast_object (x)); }
    else {
        x->x_outletRight = outlet_newMixed (cast_object (x));
    }
    
    if (IS_FLOAT (argv)) { inlet_newFloat (cast_object (x), ADDRESS_FLOAT (&x->x_atom)); } 
    else {
        inlet_newSymbol (cast_object (x), ADDRESS_SYMBOL (&x->x_atom));
    }
    
    return x;
}

static void *select2_new (int argc, t_atom *argv)
{
    t_select2 *x = (t_select2 *)pd_new (select2_class);
    int i;
        
    x->x_size   = argc;
    x->x_vector = (t_atomoutlet *)PD_MEMORY_GET (x->x_size * sizeof (t_atomoutlet));

    for (i = 0; i < argc; i++) {
        atomoutlet_make (x->x_vector + i, cast_object (x), ATOMOUTLET_OUTLET, argv + i);
    }
    
    x->x_outlet = outlet_newMixed (cast_object (x));

    return x;
}

static void select2_free (t_select2 *x)
{
    PD_MEMORY_FREE (x->x_vector);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void *select_new (t_symbol *s, int argc, t_atom *argv)
{
    if (argc == 0) { t_atom a; SET_FLOAT (&a, 0.0); return select1_new (1, &a); }
    if (argc == 1) { return select1_new (argc, argv); }

    return select2_new (argc, argv);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void select_setup (void)
{
    select1_class = class_new (sym_select,
                        NULL,
                        NULL,
                        sizeof (t_select1),
                        CLASS_DEFAULT,
                        A_NULL);
    
    select2_class = class_new (sym_select,
                        NULL,
                        (t_method)select2_free,
                        sizeof (t_select2),
                        CLASS_DEFAULT,
                        A_NULL);
    
    class_addCreator ((t_newmethod)select_new, sym_select,  A_GIMME, A_NULL);
    
    class_addFloat (select1_class,          (t_method)select1_float);
    class_addSymbol (select1_class,         (t_method)select1_symbol);
    class_addList (select1_class,           (t_method)select1_list);
    class_addAnything (select1_class,       (t_method)select1_anything);
    
    class_addMethod (select1_class,         (t_method)select1_restore, sym__restore, A_GIMME, A_NULL);

    class_setDataFunction (select1_class,   select1_functionData);
    class_requirePending (select1_class);

    class_addFloat (select2_class,          (t_method)select2_float);
    class_addSymbol (select2_class,         (t_method)select2_symbol);
    class_addAnything (select2_class,       (t_method)select2_anything);
}

PD_LOCAL void select_destroy (void)
{
    class_free (select1_class);
    class_free (select2_class);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
