
/* Copyright (c) 1997-2020 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#include "../../m_spaghettis.h"
#include "../../m_core.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#define ATOM_WIDTH_MAXIMUM      80

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

t_class *gatom_class;           /* Shared. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

struct _gatom {
    t_object    a_obj;          /* MUST be the first. */
    t_atom      a_atom;
    t_float     a_lowRange;
    t_float     a_highRange;
    t_glist     *a_owner;
    t_outlet    *a_outlet;
    };

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void gatom_set (t_gatom *, t_symbol *, int, t_atom *);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void gatom_bang (t_gatom *x)
{
    outlet_float (x->a_outlet, GET_FLOAT (&x->a_atom));
}

static void gatom_float (t_gatom *x, t_float f)
{
    t_atom a; SET_FLOAT (&a, f); gatom_set (x, NULL, 1, &a); gatom_bang (x);
}

static void gatom_set (t_gatom *x, t_symbol *s, int argc, t_atom *argv)
{
    if (argc) {
    //
    t_float f = atom_getFloat (argv);
    if (x->a_lowRange != 0.0 || x->a_highRange != 0.0) { f = PD_CLAMP (f, x->a_lowRange, x->a_highRange); }
    SET_FLOAT (&x->a_atom, f);
    //
    }
}

static void gatom_rangeProceed (t_gatom *x, t_float low, t_float high)
{
    x->a_lowRange  = PD_MIN (low, high);
    x->a_highRange = PD_MAX (low, high);
}

static void gatom_range (t_gatom *x, t_symbol *s, int argc, t_atom *argv)
{
    t_float low  = atom_getFloatAtIndex (0, argc, argv);
    t_float high = atom_getFloatAtIndex (1, argc, argv);
    
    gatom_rangeProceed (x, low, high);
    
    gatom_set (x, NULL, 1, &x->a_atom);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void gatom_functionSave (t_object *z, t_buffer *b, int flags)
{
    t_gatom *x = (t_gatom *)z;
    
    buffer_appendSymbol (b, sym___hash__X);
    buffer_appendSymbol (b, sym_floatatom);
    buffer_appendFloat (b,  object_getX (cast_object (x)));
    buffer_appendFloat (b,  object_getY (cast_object (x)));
    buffer_appendFloat (b,  object_getWidth (cast_object (x)));
    buffer_appendFloat (b,  x->a_lowRange);
    buffer_appendFloat (b,  x->a_highRange);
    if (SAVED_DEEP (flags)) { buffer_appendAtom (b, &x->a_atom); }
    buffer_appendSemicolon (b);
    
    object_serializeWidth (cast_object (x), b);
    
    object_saveIdentifiers (z, b, flags);
}

static t_buffer *gatom_functionData (t_object *z, int flags)
{
    if (SAVED_DEEP (flags)) {
    //
    t_buffer *b = buffer_new();
    
    buffer_appendSymbol (b, sym__restore);

    return b;
    //
    }
    
    return NULL;
}

/* Encapsulation. */

static void gatom_restore (t_gatom *x)
{
    t_gatom *old = (t_gatom *)instance_pendingFetch (cast_object (x));
    
    if (old) {
    //
    gatom_rangeProceed (x, old->a_lowRange, old->a_highRange); gatom_set (x, NULL, 1, &old->a_atom);
    //
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void gatom_makeObjectProceed (t_gatom *x, int argc, t_atom *argv)
{
    int width = (int)atom_getFloatAtIndex (2, argc, argv);

    width = PD_CLAMP (width, 0, ATOM_WIDTH_MAXIMUM);
    
    object_setBuffer (cast_object (x), buffer_new());
    object_setWidth (cast_object (x),  width);
    object_setType (cast_object (x),   TYPE_ATOM);
    object_setX (cast_object (x),      atom_getFloatAtIndex (0, argc, argv));
    object_setY (cast_object (x),      atom_getFloatAtIndex (1, argc, argv));
    
    x->a_lowRange  = atom_getFloatAtIndex (3, argc, argv);
    x->a_highRange = atom_getFloatAtIndex (4, argc, argv);
    
    if (argc > 5) { gatom_set (x, NULL, 1, argv + 5); }
    else {
        t_atom a; SET_FLOAT (&a, 0.0); gatom_set (x, NULL, 1, &a);
    }
    
    glist_objectAdd (x->a_owner, cast_object (x));
}

PD_LOCAL void gatom_makeObject (t_glist *glist, t_symbol *dummy, int argc, t_atom *argv)
{
    t_gatom *x = (t_gatom *)pd_new (gatom_class);
    
    SET_FLOAT (&x->a_atom, 0.0);
    
    x->a_lowRange  = 0;
    x->a_highRange = 0;
    x->a_owner     = glist;
    x->a_outlet    = outlet_newFloat (cast_object (x));

    gatom_makeObjectProceed (x, argc, argv);
    
    instance_setBoundA (cast_pd (x));
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void gatom_setup (void)
{
    t_class *c = NULL;
    
    c = class_new (sym_gatom,
            NULL,
            NULL,
            sizeof (t_gatom),
            CLASS_DEFAULT,
            A_NULL);
            
    class_addBang (c, (t_method)gatom_bang);
    class_addFloat (c, (t_method)gatom_float);
        
    class_addMethod (c, (t_method)gatom_set,        sym_set,        A_GIMME, A_NULL);
    class_addMethod (c, (t_method)gatom_range,      sym_range,      A_GIMME, A_NULL);
    class_addMethod (c, (t_method)gatom_restore,    sym__restore,   A_NULL);

    class_setSaveFunction (c, gatom_functionSave);
    class_setDataFunction (c, gatom_functionData);
    class_requirePending (c);
    
    gatom_class = c;
}

PD_LOCAL void gatom_destroy (void)
{
    class_free (gatom_class);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
