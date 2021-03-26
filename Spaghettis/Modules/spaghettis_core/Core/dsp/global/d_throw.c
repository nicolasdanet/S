
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

static t_class *throw_tilde_class;          /* Shared. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

typedef struct _throw_tilde {
    t_object            x_obj;              /* Must be the first. */
    t_pointerAtomic     x_p;
    t_symbol            *x_name;
    } t_throw_tilde;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void throw_tilde_setProceed (t_throw_tilde *x, t_symbol *s, int verbose)
{
    t_catch_tilde *catcher = (t_catch_tilde *)symbol_getThingByClass ((x->x_name = s), catch_tilde_class);
    t_sample *t = catcher ? catcher->x_vector : NULL;
    
    PD_ATOMIC_POINTER_WRITE (t, &x->x_p);
    
    if (verbose && !t && x->x_name != &s_) { error_canNotFind (sym_throw__tilde__, x->x_name); }
}

static void throw_tilde_set (t_throw_tilde *x, t_symbol *s)
{
    throw_tilde_setProceed (x, s, 1);
}

static void throw_tilde_restore (t_throw_tilde *x, t_symbol *s)
{
    throw_tilde_setProceed (x, s, 0);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

/* No aliasing.*/

static t_int *throw_tilde_perform (t_int *w)
{
    t_throw_tilde *x  = (t_throw_tilde *)(w[1]);
    PD_RESTRICTED in  = (t_sample *)(w[2]);
    PD_RESTRICTED out = (t_sample *)PD_ATOMIC_POINTER_READ (&x->x_p);
    
    if (out) { int i; for (i = 0; i < INTERNAL_BLOCKSIZE; i++) { *out += *in; out++; in++; } }
    
    return (w + 3);
}

static void throw_tilde_dsp (t_throw_tilde *x, t_signal **sp)
{
    if (sp[0]->s_vectorSize != INTERNAL_BLOCKSIZE) { error_mismatch (sym_throw__tilde__, sym_size); }
    else {
    //
    if (object_dspNeedInitializer (cast_object (x))) {
    //
    t_throw_tilde *old = (t_throw_tilde *)garbage_fetch (cast_object (x));
    
    if (old) { x->x_name = old->x_name; object_copySignalValues (cast_object (x), cast_object (old)); }
    //
    }
    
    throw_tilde_setProceed (x, x->x_name, 1);
    
    PD_ASSERT (sp[0]->s_vector != (t_sample *)PD_ATOMIC_POINTER_READ (&x->x_p));
    
    dsp_add2 (throw_tilde_perform, x, sp[0]->s_vector);
    //
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static t_buffer *throw_tilde_functionData (t_object *z, int flags)
{
    if (SAVED_DEEP (flags)) {
    //
    t_throw_tilde *x = (t_throw_tilde *)z;
    t_buffer *b = buffer_new();
    
    buffer_appendSymbol (b, sym__restore);
    buffer_appendSymbol (b, x->x_name);
    buffer_appendComma (b);
    object_getSignalValues (cast_object (x), b);
    
    return b;
    //
    }
    
    return NULL;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void *throw_tilde_new (t_symbol *s)
{
    t_throw_tilde *x = (t_throw_tilde *)pd_new (throw_tilde_class);
    
    PD_ATOMIC_POINTER_WRITE (NULL, &x->x_p);

    x->x_name = s;

    return x;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void throw_tilde_setup (void)
{
    t_class *c = NULL;
    
    c = class_new (sym_throw__tilde__,
            (t_newmethod)throw_tilde_new,
            NULL,
            sizeof (t_throw_tilde),
            CLASS_DEFAULT | CLASS_SIGNAL,
            A_DEFSYMBOL,
            A_NULL);
    
    class_addDsp (c, (t_method)throw_tilde_dsp);
    
    class_addMethod (c, (t_method)throw_tilde_set,      sym_set,        A_SYMBOL, A_NULL);
    class_addMethod (c, (t_method)throw_tilde_restore,  sym__restore,   A_SYMBOL, A_NULL);
    
    class_setDataFunction (c, throw_tilde_functionData);
    
    throw_tilde_class = c;
}

PD_LOCAL void throw_tilde_destroy (void)
{
    class_free (throw_tilde_class);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
