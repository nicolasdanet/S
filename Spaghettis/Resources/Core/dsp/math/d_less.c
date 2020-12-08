
/* Copyright (c) 1997-2020 Miller Puckette and others. */

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

static t_class *less_tilde_class;               /* Shared. */
static t_class *lessScalar_tilde_class;         /* Shared. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

typedef struct _binop_tilde t_less_tilde;
typedef struct _binopscalar_tilde t_lessscalar_tilde;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void less_tilde_dsp (t_less_tilde *x, t_signal **sp)
{
    object_fetchAndCopySignalValuesIfRequired (cast_object (x));

    dsp_addLessPerformAliased (sp[0]->s_vector, sp[1]->s_vector, sp[2]->s_vector, sp[0]->s_vectorSize);
}

static void lessScalar_tilde_dsp (t_lessscalar_tilde *x, t_signal **sp)
{
    binopScalar_tilde_restore (x);
    
    dsp_addLessScalarPerform (sp[0]->s_vector, &x->x_scalar, sp[1]->s_vector, sp[0]->s_vectorSize);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void *less_tilde_newWithScalar (t_symbol *s, int argc, t_atom *argv)
{
    if (argc > 1) { warning_unusedArguments (s, argc - 1, argv + 1); }
    
    t_lessscalar_tilde *x = (t_lessscalar_tilde *)pd_new (lessScalar_tilde_class);
    
    x->x_scalar = atom_getFloatAtIndex (0, argc, argv);
    x->x_outlet = outlet_newSignal (cast_object (x));

    inlet_new2 (x, &s_float);
    
    return x;
}

static void *less_tilde_newWithSignal (t_symbol *s, int argc, t_atom *argv)
{
    t_less_tilde *x = (t_less_tilde *)pd_new (less_tilde_class);
    
    x->x_outlet = outlet_newSignal (cast_object (x));
    
    inlet_newSignal (cast_object (x));

    return x;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

static void *less_tilde_new (t_symbol *s, int argc, t_atom *argv)
{
    if (argc) {
        return less_tilde_newWithScalar (s, argc, argv);
    } else {
        return less_tilde_newWithSignal (s, argc, argv);
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void less_tilde_setup (void)
{
    less_tilde_class = class_new (sym___less____tilde__,
                                (t_newmethod)less_tilde_new,
                                NULL,
                                sizeof (t_less_tilde),
                                CLASS_DEFAULT | CLASS_SIGNAL,
                                A_GIMME,
                                A_NULL);
    
    lessScalar_tilde_class = class_new (sym___less____tilde__,
                                NULL,
                                NULL,
                                sizeof (t_lessscalar_tilde),
                                CLASS_DEFAULT | CLASS_SIGNAL,
                                A_NULL);
                
    class_addDSP (less_tilde_class, (t_method)less_tilde_dsp);
    class_addDSP (lessScalar_tilde_class, (t_method)lessScalar_tilde_dsp);
    
    class_addMethod (lessScalar_tilde_class,
        (t_method)binopScalar_tilde_float,
        sym__inlet2,
        A_FLOAT,
        A_NULL);

    class_setDataFunction (less_tilde_class, binop_tilde_functionData);
    class_setDataFunction (lessScalar_tilde_class, binopScalar_tilde_functionData);
    
    class_setHelpName (less_tilde_class, sym_logical__tilde__);
    class_setHelpName (lessScalar_tilde_class, sym_logical__tilde__);
}

PD_LOCAL void less_tilde_destroy (void)
{
    class_free (less_tilde_class);
    class_free (lessScalar_tilde_class);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

