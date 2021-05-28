
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#include "../../m_spaghettis.h"
#include "../../m_core.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "d_global.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

static void send_tilde_dismiss (t_send_tilde *);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

t_class *send_tilde_class;      /* Shared. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

static void send_tilde_dsp (t_send_tilde *x, t_signal **sp)
{
    int size = sp[0]->s_vectorSize;
    
    if (size != INTERNAL_BLOCKSIZE) { error_mismatch (cast_object (x), sym_send__tilde__, sym_size); }
    else {
    //
    object_fetchAndCopySignalValuesIfRequired (cast_object (x));
    
    dsp_addCopyPerform (sp[0]->s_vector, x->x_vector, INTERNAL_BLOCKSIZE);
    //
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static t_buffer *send_tilde_functionData (t_object *z, int flags)
{
    if (SAVED_DEEP (flags)) {
    //
    t_send_tilde *x = (t_send_tilde *)z;
    t_buffer *b = buffer_new();
    
    object_getSignalValues (cast_object (x), b);
    
    return b;
    //
    }
    
    return NULL;
}

static void send_tilde_functionDismiss (t_object *z)
{
    send_tilde_dismiss ((t_send_tilde *)z);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void *send_tilde_new (t_symbol *s)
{
    t_send_tilde *x = (t_send_tilde *)pd_new (send_tilde_class);

    x->x_vector = (t_sample *)PD_MEMORY_GET (INTERNAL_BLOCKSIZE * sizeof (t_sample));
    x->x_name   = s;
    
    if (x->x_name != &s_) { pd_bind (cast_pd (x), s); }
        
    return x;
}

static void send_tilde_dismiss (t_send_tilde *x)
{
    if (x->x_name && x->x_name != &s_) { pd_unbind (cast_pd (x), x->x_name); x->x_name = NULL; }
}

static void send_tilde_free (t_send_tilde *x)
{
    send_tilde_dismiss (x); PD_MEMORY_FREE (x->x_vector);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void send_tilde_setup (void)
{
    t_class *c = NULL;
    
    c = class_new (sym_send__tilde__,
            (t_newmethod)send_tilde_new,
            (t_method)send_tilde_free,
            sizeof (t_send_tilde),
            CLASS_DEFAULT | CLASS_SIGNAL, 
            A_DEFSYMBOL,
            A_NULL);
            
    class_addCreator ((t_newmethod)send_tilde_new, sym_s__tilde__, A_DEFSYMBOL, A_NULL);
    
    class_addDsp (c, (t_method)send_tilde_dsp);
    
    class_setDataFunction (c, send_tilde_functionData);
    class_setDismissFunction (c, send_tilde_functionDismiss);

    send_tilde_class = c;
}

PD_LOCAL void send_tilde_destroy (void)
{
    class_free (send_tilde_class);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
