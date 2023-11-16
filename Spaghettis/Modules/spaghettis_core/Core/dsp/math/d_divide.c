
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

static t_class *divide_tilde_class;             /* Shared. */
static t_class *divideScalar_tilde_class;       /* Shared. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

typedef struct _binop_tilde t_divide_tilde;
typedef struct _binopscalar_tilde t_dividescalar_tilde;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void divide_tilde_dsp (t_divide_tilde *x, t_signal **sp)
{
    object_fetchAndCopySignalValuesIfRequired (cast_object (x));

    dsp_addDividePerformAliased (sp[0]->s_vector, sp[1]->s_vector, sp[2]->s_vector, sp[0]->s_vectorSize);
}

static void divideScalar_tilde_dsp (t_dividescalar_tilde *x, t_signal **sp)
{
    binopScalar_tilde_restore (x);
    
    dsp_addDivideScalarPerform (sp[0]->s_vector, &x->x_scalar, sp[1]->s_vector, sp[0]->s_vectorSize);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void *divide_tilde_newWithScalar (t_symbol *s, int argc, t_atom *argv)
{
    t_dividescalar_tilde *x = (t_dividescalar_tilde *)pd_new (divideScalar_tilde_class);

    if (argc > 1) { warning_unusedArguments (cast_object (x), s, argc - 1, argv + 1); }
    
    x->x_scalar = atom_getFloatAtIndex (0, argc, argv);
    x->x_outlet = outlet_newSignal (cast_object (x));
    
    inlet_new2 (x, &s_float);
        
    return x;
}

static void *divide_tilde_newWithSignal (t_symbol *s, int argc, t_atom *argv)
{
    t_divide_tilde *x = (t_divide_tilde *)pd_new (divide_tilde_class);
    
    x->x_outlet = outlet_newSignal (cast_object (x));
    
    inlet_newSignal (cast_object (x));

    return x;
}

static void *divide_tilde_new (t_symbol *s, int argc, t_atom *argv)
{
    if (argc) {
        return divide_tilde_newWithScalar (s, argc, argv);
    } else {
        return divide_tilde_newWithSignal (s, argc, argv);
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void divide_tilde_setup (void)
{
    divide_tilde_class = class_new (sym___slash____tilde__,
                                    (t_newmethod)divide_tilde_new,
                                    NULL,
                                    sizeof (t_divide_tilde),
                                    CLASS_DEFAULT | CLASS_SIGNAL,
                                    A_GIMME,
                                    A_NULL);
                        
    divideScalar_tilde_class = class_new (sym___slash____tilde__,
                                    NULL,
                                    NULL,
                                    sizeof (t_dividescalar_tilde),
                                    CLASS_DEFAULT | CLASS_SIGNAL,
                                    A_NULL);
                                    
    class_addDsp (divide_tilde_class, (t_method)divide_tilde_dsp);
    class_addDsp (divideScalar_tilde_class, (t_method)divideScalar_tilde_dsp);
        
    class_addMethod (divideScalar_tilde_class,
        (t_method)binopScalar_tilde_float,
        sym__inlet2,
        A_FLOAT,
        A_NULL);

    class_setDataFunction (divide_tilde_class, binop_tilde_functionData);
    class_setDataFunction (divideScalar_tilde_class, binopScalar_tilde_functionData);
    
    class_setHelpName (divide_tilde_class, sym_arithmetic__tilde__);
    class_setHelpName (divideScalar_tilde_class, sym_arithmetic__tilde__);
}

void divide_tilde_destroy (void)
{
    class_free (divide_tilde_class);
    class_free (divideScalar_tilde_class);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

