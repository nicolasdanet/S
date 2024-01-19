
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#include "../../m_headers.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "d_math.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

static t_class *add_tilde_class;            /* Shared. */
static t_class *addScalar_tilde_class;      /* Shared. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

typedef struct _binop_tilde t_add_tilde;
typedef struct _binopscalar_tilde t_addscalar_tilde;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void add_tilde_dsp (t_add_tilde *x, t_signal **sp)
{
    object_fetchAndCopySignalValuesIfRequired (cast_object (x));

    dsp_addPlusPerformAliased (sp[0]->s_vector, sp[1]->s_vector, sp[2]->s_vector, sp[0]->s_vectorSize);
}

static void addScalar_tilde_dsp (t_addscalar_tilde *x, t_signal **sp)
{
    binopScalar_tilde_restore (x);
    
    dsp_addPlusScalarPerform (sp[0]->s_vector, &x->x_scalar, sp[1]->s_vector, sp[0]->s_vectorSize);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

t_buffer *binop_tilde_functionData (t_object *z, int flags)
{
    if (SAVED_DEEP (flags)) {
    //
    struct _binop_tilde *x = (struct _binop_tilde *)z;
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

t_buffer *binopScalar_tilde_functionData (t_object *z, int flags)
{
    if (SAVED_DEEP (flags)) {
    //
    struct _binopscalar_tilde *x = (struct _binopscalar_tilde *)z;
    t_buffer *b = buffer_new();
    
    buffer_appendSymbol (b, sym__inlet2);
    buffer_appendFloat (b,  PD_ATOMIC_FLOAT64_READ (&x->x_scalar));
    buffer_appendComma (b);
    object_getSignalValues (cast_object (x), b);
    
    return b;
    //
    }
    
    return NULL;
}

void binopScalar_tilde_restore (struct _binopscalar_tilde *x)
{
    if (object_dspNeedInitializer (cast_object (x))) {
    //
    struct _binopscalar_tilde *old = (struct _binopscalar_tilde *)garbage_fetch (cast_object (x));
    
    if (old) {
    //
    binopScalar_tilde_float (x, PD_ATOMIC_FLOAT64_READ (&old->x_scalar));
    
    object_copySignalValues (cast_object (x), cast_object (old));
    //
    }
    //
    }
}

void binopScalar_tilde_float (struct _binopscalar_tilde *x, t_float f)
{
    PD_ATOMIC_FLOAT64_WRITE (f, &x->x_scalar);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void *add_tilde_newWithScalar (t_symbol *s, int argc, t_atom *argv)
{
    t_addscalar_tilde *x = (t_addscalar_tilde *)pd_new (addScalar_tilde_class);
    
    if (argc > 1) { warning_unusedArguments (cast_object (x), s, argc - 1, argv + 1); }
    
    x->x_scalar = atom_getFloatAtIndex (0, argc, argv);
    x->x_outlet = outlet_newSignal (cast_object (x));

    inlet_new2 (x, &s_float);
    
    return x;
}

static void *add_tilde_newWithSignal (t_symbol *s, int argc, t_atom *argv)
{
    t_add_tilde *x = (t_add_tilde *)pd_new (add_tilde_class);
    
    x->x_outlet = outlet_newSignal (cast_object (x));
    
    inlet_newSignal (cast_object (x));

    return x;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

static void *add_tilde_new (t_symbol *s, int argc, t_atom *argv)
{
    if (argc) {
        return add_tilde_newWithScalar (s, argc, argv); 
    } else {
        return add_tilde_newWithSignal (s, argc, argv);
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void add_tilde_setup (void)
{
    add_tilde_class = class_new (sym___plus____tilde__,
                                (t_newmethod)add_tilde_new,
                                NULL,
                                sizeof (t_add_tilde),
                                CLASS_DEFAULT | CLASS_SIGNAL,
                                A_GIMME,
                                A_NULL);
    
    addScalar_tilde_class = class_new (sym___plus____tilde__,
                                NULL,
                                NULL,
                                sizeof (t_addscalar_tilde),
                                CLASS_DEFAULT | CLASS_SIGNAL,
                                A_NULL);
    
    class_addDsp (add_tilde_class, (t_method)add_tilde_dsp);
    class_addDsp (addScalar_tilde_class, (t_method)addScalar_tilde_dsp);
    
    class_addMethod (addScalar_tilde_class,
        (t_method)binopScalar_tilde_float,
        sym__inlet2,
        A_FLOAT,
        A_NULL);

    class_setDataFunction (add_tilde_class, binop_tilde_functionData);
    class_setDataFunction (addScalar_tilde_class, binopScalar_tilde_functionData);
    
    class_setHelpName (add_tilde_class, sym_arithmetic__tilde__);
    class_setHelpName (addScalar_tilde_class, sym_arithmetic__tilde__);
}

void add_tilde_destroy (void)
{
    class_free (add_tilde_class);
    class_free (addScalar_tilde_class);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

