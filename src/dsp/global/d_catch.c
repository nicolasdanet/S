
/* Copyright (c) 1997-2020 Miller Puckette and others. */

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

static void catch_tilde_dismiss (t_catch_tilde *);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

t_class *catch_tilde_class;         /* Shared. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

static void catch_tilde_dsp (t_catch_tilde *x, t_signal **sp)
{
    if (sp[0]->s_vectorSize != INTERNAL_BLOCKSIZE) { error_mismatch (sym_catch__tilde__, sym_size); }
    else {
        dsp_addCopyZeroPerform (x->x_vector, sp[0]->s_vector, INTERNAL_BLOCKSIZE);
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void catch_tilde_functionDismiss (t_object *x)
{
    catch_tilde_dismiss ((t_catch_tilde *)x);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void *catch_tilde_new (t_symbol *s)
{
    t_catch_tilde *x = (t_catch_tilde *)pd_new (catch_tilde_class);
    
    x->x_vector = (t_sample *)PD_MEMORY_GET (INTERNAL_BLOCKSIZE * sizeof (t_sample));
    x->x_name   = s;
    x->x_outlet = outlet_newSignal (cast_object (x));
    
    if (x->x_name != &s_) { pd_bind (cast_pd (x), x->x_name); }
    
    return x;
}

static void catch_tilde_dismiss (t_catch_tilde *x)
{
    if (x->x_name && x->x_name != &s_) { pd_unbind (cast_pd (x), x->x_name); x->x_name = NULL; }
}

static void catch_tilde_free (t_catch_tilde *x)
{
    catch_tilde_dismiss (x); PD_MEMORY_FREE (x->x_vector);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void catch_tilde_setup (void)
{
    t_class *c = NULL;
    
    c = class_new (sym_catch__tilde__,
            (t_newmethod)catch_tilde_new,
            (t_method)catch_tilde_free,
            sizeof (t_catch_tilde),
            CLASS_DEFAULT | CLASS_NOINLET,
            A_DEFSYMBOL,
            A_NULL);
            
    class_addDSP (c, (t_method)catch_tilde_dsp);
    
    class_setHelpName (c, sym_throw__tilde__);
    class_setDismissFunction (c, catch_tilde_functionDismiss);

    catch_tilde_class = c;
}

PD_LOCAL void catch_tilde_destroy (void)
{
    class_free (catch_tilde_class);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
