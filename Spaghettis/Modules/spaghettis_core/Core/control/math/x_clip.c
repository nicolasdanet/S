
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#include "../../m_spaghettis.h"
#include "../../m_core.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

static t_class *clip_class;     /* Shared. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

typedef struct _clip {
    t_object    x_obj;
    t_float     x_f1;
    t_float     x_f2;
    t_float     x_f3;
    t_outlet    *x_outlet;
    } t_clip;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void clip_bang (t_clip *x)
{
    t_float a = PD_MIN (x->x_f2, x->x_f3);
    t_float b = PD_MAX (x->x_f2, x->x_f3);
    
    outlet_float (x->x_outlet, PD_CLAMP (x->x_f1, a, b));
}

static void clip_float (t_clip *x, t_float f)
{
    x->x_f1 = f; clip_bang (x);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static t_buffer *clip_functionData (t_object *z, int flags)
{
    if (SAVED_DEEP (flags)) {
    //
    t_clip *x   = (t_clip *)z;
    t_buffer *b = buffer_new();
    
    buffer_appendSymbol (b, sym__restore);
    buffer_appendFloat (b, x->x_f1);
    buffer_appendFloat (b, x->x_f2);
    buffer_appendFloat (b, x->x_f3);
    
    return b;
    //
    }
    
    return NULL;
}

static void clip_restore (t_clip *x, t_symbol *s, int argc, t_atom *argv)
{
    t_clip *old = (t_clip *)instance_pendingFetch (cast_object (x));
    
    t_float f1 = old ? old->x_f1 : atom_getFloatAtIndex (0, argc, argv);
    t_float f2 = old ? old->x_f2 : atom_getFloatAtIndex (1, argc, argv);
    t_float f3 = old ? old->x_f3 : atom_getFloatAtIndex (2, argc, argv);
    
    x->x_f1 = f1;
    x->x_f2 = f2;
    x->x_f3 = f3;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void *clip_new (t_float f2, t_float f3)
{
    t_clip *x = (t_clip *)pd_new (clip_class);
    
    x->x_f2     = f2;
    x->x_f3     = f3;
    x->x_outlet = outlet_newFloat (cast_object (x));
    
    inlet_newFloat (cast_object (x), &x->x_f2);
    inlet_newFloat (cast_object (x), &x->x_f3);

    return x;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void clip_setup (void)
{
    t_class *c = NULL;
    
    c = class_new (sym_clip,
            (t_newmethod)clip_new,
            NULL,
            sizeof (t_clip),
            CLASS_DEFAULT,
            A_DEFFLOAT,
            A_DEFFLOAT,
            A_NULL);
            
    class_addBang (c, (t_method)clip_bang);
    class_addFloat (c, (t_method)clip_float); 
    
    class_addMethod (c, (t_method)clip_restore, sym__restore, A_GIMME, A_NULL);

    class_setDataFunction (c, clip_functionData);
    class_requirePending (c);
    
    class_setHelpName (c, sym_math);
    
    clip_class = c;
}

void clip_destroy (void)
{
    class_free (clip_class);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
