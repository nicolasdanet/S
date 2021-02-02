
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#include "../../m_spaghettis.h"
#include "../../m_core.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "d_math.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

static t_class *greater_tilde_class;            /* Shared. */
static t_class *greaterScalar_tilde_class;      /* Shared. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

typedef struct _binop_tilde t_greater_tilde;
typedef struct _binopscalar_tilde t_greaterscalar_tilde;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void greater_tilde_dsp (t_greater_tilde *x, t_signal **sp)
{
    object_fetchAndCopySignalValuesIfRequired (cast_object (x));

    dsp_addGreaterPerformAliased (sp[0]->s_vector, sp[1]->s_vector, sp[2]->s_vector, sp[0]->s_vectorSize);
}

static void greaterScalar_tilde_dsp (t_greaterscalar_tilde *x, t_signal **sp)
{
    binopScalar_tilde_restore (x);
    
    dsp_addGreaterScalarPerform (sp[0]->s_vector, &x->x_scalar, sp[1]->s_vector, sp[0]->s_vectorSize);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void *greater_tilde_newWithScalar (t_symbol *s, int argc, t_atom *argv)
{
    if (argc > 1) { warning_unusedArguments (s, argc - 1, argv + 1); }
    
    t_greaterscalar_tilde *x = (t_greaterscalar_tilde *)pd_new (greaterScalar_tilde_class);
    
    x->x_scalar = atom_getFloatAtIndex (0, argc, argv);
    x->x_outlet = outlet_newSignal (cast_object (x));

    inlet_new2 (x, &s_float);
    
    return x;
}

static void *greater_tilde_newWithSignal (t_symbol *s, int argc, t_atom *argv)
{
    t_greater_tilde *x = (t_greater_tilde *)pd_new (greater_tilde_class);
    
    x->x_outlet = outlet_newSignal (cast_object (x));
    
    inlet_newSignal (cast_object (x));

    return x;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

static void *greater_tilde_new (t_symbol *s, int argc, t_atom *argv)
{
    if (argc) {
        return greater_tilde_newWithScalar (s, argc, argv);
    } else {
        return greater_tilde_newWithSignal (s, argc, argv);
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void greater_tilde_setup (void)
{
    greater_tilde_class = class_new (sym___greater____tilde__,
                                    (t_newmethod)greater_tilde_new,
                                    NULL,
                                    sizeof (t_greater_tilde),
                                    CLASS_DEFAULT | CLASS_SIGNAL,
                                    A_GIMME,
                                    A_NULL);
    
    greaterScalar_tilde_class = class_new (sym___greater____tilde__,
                                    NULL,
                                    NULL,
                                    sizeof (t_greaterscalar_tilde),
                                    CLASS_DEFAULT | CLASS_SIGNAL,
                                    A_NULL);
        
    class_addDSP (greater_tilde_class, (t_method)greater_tilde_dsp);
    class_addDSP (greaterScalar_tilde_class, (t_method)greaterScalar_tilde_dsp);
    
    class_addMethod (greaterScalar_tilde_class,
        (t_method)binopScalar_tilde_float,
        sym__inlet2,
        A_FLOAT,
        A_NULL);

    class_setDataFunction (greater_tilde_class, binop_tilde_functionData);
    class_setDataFunction (greaterScalar_tilde_class, binopScalar_tilde_functionData);
    
    class_setHelpName (greater_tilde_class, sym_logical__tilde__);
    class_setHelpName (greaterScalar_tilde_class, sym_logical__tilde__);
}

PD_LOCAL void greater_tilde_destroy (void)
{
    class_free (greater_tilde_class);
    class_free (greaterScalar_tilde_class);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

