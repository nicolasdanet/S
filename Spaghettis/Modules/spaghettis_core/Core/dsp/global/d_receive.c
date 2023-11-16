
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

static t_class *receive_tilde_class;            /* Shared. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

typedef struct _receive_tilde {
    t_object            x_obj;                  /* Must be the first. */
    t_pointerAtomic     x_p;
    t_symbol            *x_name;
    t_outlet            *x_outlet;
    } t_receive_tilde;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void receive_tilde_setProceed (t_receive_tilde *x, t_symbol *s, int verbose)
{
    t_send_tilde *sender = (t_send_tilde *)symbol_getThingByClass ((x->x_name = s), send_tilde_class);
    t_sample *t = sender ? sender->x_vector : NULL;
    
    PD_ATOMIC_POINTER_WRITE (t, &x->x_p);
    
    if (verbose && !t && x->x_name != &s_) {
        error_canNotFind (cast_object (x), sym_receive__tilde__, x->x_name);
    }
}

static void receive_tilde_set (t_receive_tilde *x, t_symbol *s)
{
    receive_tilde_setProceed (x, s, 1);
}

static void receive_tilde_restore (t_receive_tilde *x, t_symbol *s)
{
    receive_tilde_setProceed (x, s, 0);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

/* No aliasing. */

static t_int *receive_tilde_perform (t_int *w)
{
    t_receive_tilde *x = (t_receive_tilde *)(w[1]);
    PD_RESTRICTED out  = (t_sample *)(w[2]);
    PD_RESTRICTED in   = (t_sample *)PD_ATOMIC_POINTER_READ (&x->x_p);
    
    if (in) { memcpy (out, in, INTERNAL_BLOCKSIZE * sizeof (t_sample)); }
    else {
        memset (out, 0, INTERNAL_BLOCKSIZE * sizeof (t_sample));
    }
    
    return (w + 3);
}

static void receive_tilde_dsp (t_receive_tilde *x, t_signal **sp)
{
    int size = sp[0]->s_vectorSize;
    
    if (size != INTERNAL_BLOCKSIZE) { error_mismatch (cast_object (x), sym_receive__tilde__, sym_size); }
    else {
    //
    if (object_dspNeedInitializer (cast_object (x))) {
    //
    t_receive_tilde *old = (t_receive_tilde *)garbage_fetch (cast_object (x));
    
    if (old) { x->x_name = old->x_name; }
    //
    }
    
    receive_tilde_setProceed (x, x->x_name, 1);
    
    PD_ASSERT ((t_sample *)PD_ATOMIC_POINTER_READ (&x->x_p) != sp[0]->s_vector);
    
    dsp_add2 (receive_tilde_perform, x, sp[0]->s_vector);
    //
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static t_buffer *receive_tilde_functionData (t_object *z, int flags)
{
    if (SAVED_DEEP (flags)) {
    //
    t_receive_tilde *x = (t_receive_tilde *)z;
    t_buffer *b = buffer_new();
    
    buffer_appendSymbol (b, sym__restore);
    buffer_appendSymbol (b, x->x_name);
    
    return b;
    //
    }
    
    return NULL;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void *receive_tilde_new (t_symbol *s)
{
    t_receive_tilde *x = (t_receive_tilde *)pd_new (receive_tilde_class);
    
    x->x_name   = s;
    x->x_outlet = outlet_newSignal (cast_object (x));
    
    PD_ATOMIC_POINTER_WRITE (NULL, &x->x_p);
    
    return x;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void receive_tilde_setup (void)
{
    t_class *c = NULL;
    
    c = class_new (sym_receive__tilde__,
            (t_newmethod)receive_tilde_new,
            NULL,
            sizeof (t_receive_tilde),
            CLASS_DEFAULT,
            A_DEFSYMBOL,
            A_NULL);
            
    class_addCreator ((t_newmethod)receive_tilde_new, sym_r__tilde__, A_DEFSYMBOL, A_NULL);
    
    class_addDsp (c, (t_method)receive_tilde_dsp);
    
    class_addMethod (c, (t_method)receive_tilde_set,        sym_set,        A_SYMBOL, A_NULL);
    class_addMethod (c, (t_method)receive_tilde_restore,    sym__restore,   A_SYMBOL, A_NULL);
    
    class_setDataFunction (c, receive_tilde_functionData);
    class_setHelpName (c, sym_send__tilde__);
    
    receive_tilde_class = c;
}

void receive_tilde_destroy (void)
{
    class_free (receive_tilde_class);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
