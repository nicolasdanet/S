
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

static t_class *subtract_tilde_class;               /* Shared. */
static t_class *subtractScalar_tilde_class;         /* Shared. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

typedef struct _binop_tilde t_subtract_tilde;
typedef struct _binopscalar_tilde t_subtractscalar_tilde;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void subtract_tilde_dsp (t_subtract_tilde *x, t_signal **sp)
{
    object_fetchAndCopySignalValuesIfRequired (cast_object (x));

    dsp_addSubtractPerformAliased (sp[0]->s_vector, sp[1]->s_vector, sp[2]->s_vector, sp[0]->s_vectorSize);
}

static void subtractScalar_tilde_dsp (t_subtractscalar_tilde *x, t_signal **sp)
{
    binopScalar_tilde_restore (x);
    
    dsp_addSubtractScalarPerform (sp[0]->s_vector, &x->x_scalar, sp[1]->s_vector, sp[0]->s_vectorSize);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void *subtract_tilde_newWithScalar (t_symbol *s, int argc, t_atom *argv)
{
    t_subtractscalar_tilde *x = (t_subtractscalar_tilde *)pd_new (subtractScalar_tilde_class);
    
    if (argc > 1) { warning_unusedArguments (cast_object (x), s, argc - 1, argv + 1); }
    
    x->x_scalar = atom_getFloatAtIndex (0, argc, argv);
    x->x_outlet = outlet_newSignal (cast_object (x));
    
    inlet_new2 (x, &s_float);
            
    return x;
}

static void *subtract_tilde_newWithSignal (t_symbol *s, int argc, t_atom *argv)
{
    t_subtract_tilde *x = (t_subtract_tilde *)pd_new (subtract_tilde_class);
    
    x->x_outlet = outlet_newSignal (cast_object (x));
    
    inlet_newSignal (cast_object (x));
        
    return x;
}

static void *subtract_tilde_new (t_symbol *s, int argc, t_atom *argv)
{
    if (argc) {
        return subtract_tilde_newWithScalar (s, argc, argv); 
    } else {
        return subtract_tilde_newWithSignal (s, argc, argv);
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void subtract_tilde_setup (void)
{
    subtract_tilde_class = class_new (sym___minus____tilde__,
                                    (t_newmethod)subtract_tilde_new,
                                    NULL,
                                    sizeof (t_subtract_tilde),
                                    CLASS_DEFAULT | CLASS_SIGNAL,
                                    A_GIMME,
                                    A_NULL);
    
    subtractScalar_tilde_class = class_new (sym___minus____tilde__,
                                    NULL,
                                    NULL,
                                    sizeof (t_subtractscalar_tilde),
                                    CLASS_DEFAULT | CLASS_SIGNAL,
                                    A_NULL);
            
    class_addDsp (subtract_tilde_class, (t_method)subtract_tilde_dsp);
    class_addDsp (subtractScalar_tilde_class, (t_method)subtractScalar_tilde_dsp);

    class_addMethod (subtractScalar_tilde_class,
        (t_method)binopScalar_tilde_float,
        sym__inlet2,
        A_FLOAT,
        A_NULL);

    class_setDataFunction (subtract_tilde_class, binop_tilde_functionData);
    class_setDataFunction (subtractScalar_tilde_class, binopScalar_tilde_functionData);
    
    class_setHelpName (subtract_tilde_class, sym_arithmetic__tilde__);
    class_setHelpName (subtractScalar_tilde_class, sym_arithmetic__tilde__);
}

PD_LOCAL void subtract_tilde_destroy (void)
{
    class_free (subtract_tilde_class);
    class_free (subtractScalar_tilde_class);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

