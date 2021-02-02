
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#include "../../m_spaghettis.h"
#include "../../m_core.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

static t_class *bag_class;                  /* Shared. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

typedef struct _bagelement {
    t_float             e_value;
    struct _bagelement  *e_next;
    } t_bagelement;

typedef struct _bag {
    t_object            x_obj;              /* Must be the first. */
    t_float             x_velocity;
    t_bagelement        *x_elements;
    t_outlet            *x_outlet;
    } t_bag;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void bag_flush (t_bag *);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void bag_add (t_bag *x, t_float f)
{
    t_bagelement *e = (t_bagelement *)PD_MEMORY_GET (sizeof (t_bagelement));
    
    e->e_value = f;
    e->e_next  = NULL;
    
    if (!x->x_elements) { x->x_elements = e; }
    else {
        t_bagelement *e1 = NULL;
        t_bagelement *e2 = NULL;
        for ((e1 = x->x_elements); (e2 = e1->e_next); (e1 = e2)) { }
        e1->e_next = e;
    }
}

static void bag_remove (t_bag *x, t_float f)
{
    if (x->x_elements) {
    //
    t_bagelement *e = NULL;
    
    if (x->x_elements->e_value == f) {
        e = x->x_elements;
        x->x_elements = x->x_elements->e_next; PD_MEMORY_FREE (e);
        
    } else {
        t_bagelement *e1 = NULL;
        t_bagelement *e2 = NULL;
        for ((e1 = x->x_elements); (e2 = e1->e_next); (e1 = e2)) {
            if (e2->e_value == f) { e1->e_next = e2->e_next; PD_MEMORY_FREE (e2); break; }
        }
    }
    //
    }
}

static void bag_removeAll (t_bag *x, int dump)
{
    t_bagelement *e = NULL;
    
    while ((e = x->x_elements)) {
        if (dump) { outlet_float (x->x_outlet, e->e_value); }
        x->x_elements = e->e_next; PD_MEMORY_FREE (e);
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void bag_bang (t_bag *x)
{
    bag_flush (x);
}

static void bag_float (t_bag *x, t_float f)
{
    if (x->x_velocity) { bag_add (x, f); }
    else {
        bag_remove (x, f);
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void bag_flush (t_bag *x)
{
    bag_removeAll (x, 1);
}

static void bag_clear (t_bag *x)
{
    bag_removeAll (x, 0);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static t_buffer *bag_functionData (t_object *z, int flags)
{
    if (SAVED_DEEP (flags)) {
    //
    t_bag *x = (t_bag *)z;
    t_buffer *b = buffer_new();
    
    buffer_appendSymbol (b, sym__restore);
    buffer_appendFloat (b, x->x_velocity);
    
    t_bagelement *e = x->x_elements; while (e) { buffer_appendFloat (b, e->e_value); e = e->e_next; }
    
    return b;
    //
    }
    
    return NULL;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void bag_restoreEncapsulation (t_bag *x, t_bag *old)
{
    x->x_velocity = old->x_velocity;
    x->x_elements = old->x_elements; old->x_elements = NULL;
}

static void bag_restore (t_bag *x, t_symbol *s, int argc, t_atom *argv)
{
    t_bag *old = (t_bag *)instance_pendingFetch (cast_object (x));
    
    if (old) { bag_restoreEncapsulation (x, old); }
    else {
    //
    x->x_velocity = atom_getFloatAtIndex (0, argc, argv);
    
    if (argc > 1) {
    //
    int i; for (i = 1; i < argc; i++) { t_float f = atom_getFloat (argv + i); bag_add (x, f); }
    //
    }
    //
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void *bag_new (t_float f)
{
    t_bag *x = (t_bag *)pd_new (bag_class);
    
    x->x_velocity = f;
    x->x_elements = NULL;
    x->x_outlet   = outlet_newFloat (cast_object (x));
    
    inlet_newFloat (cast_object (x), &x->x_velocity);
    
    return x;
}

static void bag_free (t_bag *x)
{                                                                   
    bag_clear (x);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void bag_setup (void)
{
    t_class *c = NULL;
    
    c = class_new (sym_bag, 
            (t_newmethod)bag_new,
            (t_method)bag_free,
            sizeof (t_bag),
            CLASS_DEFAULT,
            A_DEFFLOAT,
            A_NULL);
    
    class_addBang (c, (t_method)bag_bang);
    class_addFloat (c, (t_method)bag_float);
    
    class_addMethod (c, (t_method)bag_flush,    sym_flush,      A_NULL);
    class_addMethod (c, (t_method)bag_clear,    sym_clear,      A_NULL);
    class_addMethod (c, (t_method)bag_restore,  sym__restore,   A_GIMME, A_NULL);

    class_setDataFunction (c, bag_functionData);
    class_requirePending (c);
    
    bag_class = c;
}

PD_LOCAL void bag_destroy (void)
{
    class_free (bag_class);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
