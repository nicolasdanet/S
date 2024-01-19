
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#include "../../m_headers.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

static t_class *rmag_tilde_class;           /* Shared. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

typedef struct _rmag_tilde {
    t_object    x_obj;                      /* Must be the first. */
    t_outlet    *x_outlet;
    } t_rmag_tilde;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

/* Notice that the two signals incoming could be theoretically just one. */
/* But as only loads are done, it is assumed safe to use restricted pointers. */

static void rmag_tilde_dsp (t_rmag_tilde *x, t_signal **sp)
{
    object_fetchAndCopySignalValuesIfRequired (cast_object (x));

    dsp_addInverseMagnitudePerform (sp[0]->s_vector, sp[1]->s_vector, sp[2]->s_vector, sp[0]->s_vectorSize);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static t_buffer *rmag_tilde_functionData (t_object *z, int flags)
{
    if (SAVED_DEEP (flags)) {
    //
    t_rmag_tilde *x = (t_rmag_tilde *)z;
    t_buffer *b = buffer_new();
    
    object_getSignalValues (cast_object (x), b);
    
    return b;
    //
    }
    
    return NULL;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void *rmag_tilde_new (t_symbol *s, int argc, t_atom *argv)
{
    t_rmag_tilde *x = (t_rmag_tilde *)pd_new (rmag_tilde_class);
    
    x->x_outlet = outlet_newSignal (cast_object (x));
    
    inlet_newSignal (cast_object (x));

    return x;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void rmag_tilde_setup (void)
{
    t_class *c = NULL;
    
    c = class_new (sym_rmag__tilde__,
            (t_newmethod)rmag_tilde_new,
            NULL,
            sizeof (t_rmag_tilde),
            CLASS_DEFAULT | CLASS_SIGNAL,
            A_GIMME,
            A_NULL);
    
    class_addDsp (c, (t_method)rmag_tilde_dsp);
        
    class_setDataFunction (c, rmag_tilde_functionData);
    
    rmag_tilde_class = c;
}

void rmag_tilde_destroy (void)
{
    class_free (rmag_tilde_class);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

