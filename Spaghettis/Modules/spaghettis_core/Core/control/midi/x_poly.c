
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#include "../../m_spaghettis.h"
#include "../../m_core.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

static t_class *poly_class;                 /* Shared. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

typedef struct voice {
    t_float         v_pitch;
    int             v_used;
    uint64_t        v_serial;
    } t_voice;

typedef struct poly {
    t_object        x_obj;                  /* Must be the first. */
    t_float         x_velocity;
    int             x_hasStealMode;
    uint64_t        x_serial;
    int             x_size;
    t_voice         *x_vector;
    t_outlet        *x_outletLeft;
    t_outlet        *x_outletMiddle;
    t_outlet        *x_outletRight;
    } t_poly;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#define POLY_UNDEFINED  ULLONG_MAX
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void poly_addAtIndex (t_poly *x, t_float f, int i)
{
    PD_ASSERT (!x->x_vector[i].v_used);
    
    x->x_vector[i].v_pitch  = f;
    x->x_vector[i].v_used   = 1;
    x->x_vector[i].v_serial = x->x_serial++;
        
    outlet_float (x->x_outletRight,  x->x_velocity);
    outlet_float (x->x_outletMiddle, x->x_vector[i].v_pitch);
    outlet_float (x->x_outletLeft,   i + 1);
}

static void poly_removeAtIndex (t_poly *x, int i, int dump)
{
    int k = (dump && x->x_vector[i].v_used);
    int t = x->x_vector[i].v_pitch;
    
    x->x_vector[i].v_pitch  = (t_float)-1.0;
    x->x_vector[i].v_used   = 0;
    x->x_vector[i].v_serial = x->x_serial++;
    
    if (k) {
        outlet_float (x->x_outletRight,  0.0);
        outlet_float (x->x_outletMiddle, t);
        outlet_float (x->x_outletLeft,   i + 1);
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void poly_add (t_poly *x, t_float f)
{
    int i;
    int m = -1;
    int n = -1;
    uint64_t used  = POLY_UNDEFINED;
    uint64_t empty = POLY_UNDEFINED;
    
    for (i = 0; i < x->x_size; i++) {
        if (x->x_vector[i].v_used && x->x_vector[i].v_serial < used) {
            used = x->x_vector[i].v_serial; 
            m = i;
        } else if (!x->x_vector[i].v_used && x->x_vector[i].v_serial < empty) {
            empty = x->x_vector[i].v_serial;
            n = i;
        }
    }
        
    if (n != -1) { poly_addAtIndex (x, f, n); }
    else if (m != -1) {
        if (x->x_hasStealMode) {
            poly_removeAtIndex (x, m, 1); poly_addAtIndex (x, f, m);
        }
    }
}

static void poly_remove (t_poly *x, t_float f)
{
    int i;
    int m = -1;
    uint64_t used = POLY_UNDEFINED;
    
    for (i = 0; i < x->x_size; i++) {
        if (x->x_vector[i].v_used && x->x_vector[i].v_pitch == f && x->x_vector[i].v_serial < used) {
            used = x->x_vector[i].v_serial; 
            m = i;
        } 
    }
    
    if (m != -1) { poly_removeAtIndex (x, m, 1); }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void poly_float (t_poly *x, t_float f)
{
    if (x->x_velocity > 0.0) { poly_add (x, f); }
    else {
        poly_remove (x, f);
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void poly_stop (t_poly *x)
{
    int i;
    for (i = 0; i < x->x_size; i++) { poly_removeAtIndex (x, i, 1); }
}

static void poly_clear (t_poly *x)
{
    int i;
    for (i = 0; i < x->x_size; i++) { poly_removeAtIndex (x, i, 0); }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static t_buffer *poly_functionData (t_object *z, int flags)
{
    if (SAVED_DEEP (flags)) {
    //
    t_poly *x = (t_poly *)z;
    t_buffer *b = buffer_new();
    
    buffer_appendSymbol (b, sym__restore);
    buffer_appendFloat (b, x->x_velocity);
    
    return b;
    //
    }
    
    return NULL;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void poly_restoreEncapsulation (t_poly *x, t_poly *old)
{
    x->x_velocity     = old->x_velocity;
    x->x_hasStealMode = old->x_hasStealMode;
    x->x_serial       = old->x_serial;
    
    PD_ASSERT (x->x_size == old->x_size);
    
    t_voice *t = x->x_vector; x->x_vector = old->x_vector; old->x_vector = t;           /* Swap. */
}

static void poly_restore (t_poly *x, t_float f)
{
    t_poly *old = (t_poly *)instance_pendingFetch (cast_object (x));
    
    if (old) { poly_restoreEncapsulation (x, old); }
    else {
        x->x_velocity = f;
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void *poly_new (t_float voices, t_float steal)
{
    t_poly *x = (t_poly *)pd_new (poly_class);
    
    x->x_velocity     = 0;
    x->x_hasStealMode = (steal != 0.0);
    x->x_serial       = 0;
    x->x_size         = PD_MAX (1, (int)voices);
    x->x_vector       = (t_voice *)PD_MEMORY_GET (x->x_size * sizeof (t_voice));
    x->x_outletLeft   = outlet_newFloat (cast_object (x));
    x->x_outletMiddle = outlet_newFloat (cast_object (x));
    x->x_outletRight  = outlet_newFloat (cast_object (x));

    inlet_newFloat (cast_object (x), &x->x_velocity);
    
    poly_clear (x);
    
    return x;
}

static void poly_free (t_poly *x)
{
    PD_MEMORY_FREE (x->x_vector);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void poly_setup (void)
{
    t_class *c = NULL;
    
    c = class_new (sym_poly, 
            (t_newmethod)poly_new,
            (t_method)poly_free,
            sizeof (t_poly),
            CLASS_DEFAULT,
            A_DEFFLOAT,
            A_DEFFLOAT,
            A_NULL);
        
    class_addFloat (c, (t_method)poly_float);
    
    class_addMethod (c, (t_method)poly_stop,    sym_stop,       A_NULL);
    class_addMethod (c, (t_method)poly_stop,    sym_flush,      A_NULL);
    class_addMethod (c, (t_method)poly_clear,   sym_clear,      A_NULL);
    class_addMethod (c, (t_method)poly_restore, sym__restore,   A_FLOAT, A_NULL);

    class_setDataFunction (c, poly_functionData);
    class_requirePending (c);
    
    poly_class = c;
}

PD_LOCAL void poly_destroy (void)
{
    class_free (poly_class);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
