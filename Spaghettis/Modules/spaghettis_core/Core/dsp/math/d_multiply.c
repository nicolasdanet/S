
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

static t_class *multiply_tilde_class;               /* Shared. */
static t_class *multiplyScalar_tilde_class;         /* Shared. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

typedef struct _binop_tilde t_multiply_tilde;
typedef struct _binopscalar_tilde t_multiplyscalar_tilde;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void multiply_tilde_dsp (t_multiply_tilde *x, t_signal **sp)
{
    object_fetchAndCopySignalValuesIfRequired (cast_object (x));

    dsp_addMultiplyPerformAliased (sp[0]->s_vector, sp[1]->s_vector, sp[2]->s_vector, sp[0]->s_vectorSize);
}

static void multiplyScalar_tilde_dsp (t_multiplyscalar_tilde *x, t_signal **sp)
{
    binopScalar_tilde_restore (x);
    
    dsp_addMultiplyScalarPerform (sp[0]->s_vector, &x->x_scalar, sp[1]->s_vector, sp[0]->s_vectorSize);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void *multiply_tilde_newWithScalar (t_symbol *s, int argc, t_atom *argv)
{
    if (argc > 1) { warning_unusedArguments (s, argc - 1, argv + 1); }
    
    t_multiplyscalar_tilde *x = (t_multiplyscalar_tilde *)pd_new (multiplyScalar_tilde_class);
    
    x->x_scalar = atom_getFloatAtIndex (0, argc, argv);
    x->x_outlet = outlet_newSignal (cast_object (x));
        
    inlet_new2 (x, &s_float);

    return x;
}

static void *multiply_tilde_newWithSignal (t_symbol *s, int argc, t_atom *argv)
{
    t_multiply_tilde *x = (t_multiply_tilde *)pd_new (multiply_tilde_class);
    
    x->x_outlet = outlet_newSignal (cast_object (x));
    
    inlet_newSignal (cast_object (x));
        
    return x;
}

static void *multiply_tilde_new (t_symbol *s, int argc, t_atom *argv)
{
    if (argc) {
        return multiply_tilde_newWithScalar (s, argc, argv);
    } else {
        return multiply_tilde_newWithSignal (s, argc, argv);
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void multiply_tilde_setup (void)
{
    multiply_tilde_class = class_new (sym___asterisk____tilde__,
                                    (t_newmethod)multiply_tilde_new,
                                    NULL,
                                    sizeof (t_multiply_tilde),
                                    CLASS_DEFAULT | CLASS_SIGNAL,
                                    A_GIMME,
                                    A_NULL);
    
    multiplyScalar_tilde_class = class_new (sym___asterisk____tilde__,
                                    NULL,
                                    NULL,
                                    sizeof (t_multiplyscalar_tilde),
                                    CLASS_DEFAULT | CLASS_SIGNAL,
                                    A_NULL);
            
    class_addDsp (multiply_tilde_class, (t_method)multiply_tilde_dsp);
    class_addDsp (multiplyScalar_tilde_class, (t_method)multiplyScalar_tilde_dsp);
    
    class_addMethod (multiplyScalar_tilde_class,
        (t_method)binopScalar_tilde_float,
        sym__inlet2,
        A_FLOAT,
        A_NULL);

    class_setDataFunction (multiply_tilde_class, binop_tilde_functionData);
    class_setDataFunction (multiplyScalar_tilde_class, binopScalar_tilde_functionData);
    
    class_setHelpName (multiply_tilde_class, sym_arithmetic__tilde__);
    class_setHelpName (multiplyScalar_tilde_class, sym_arithmetic__tilde__);
}

PD_LOCAL void multiply_tilde_destroy (void)
{
    class_free (multiply_tilde_class);
    class_free (multiplyScalar_tilde_class);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

