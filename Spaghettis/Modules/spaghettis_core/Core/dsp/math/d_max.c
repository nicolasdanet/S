
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

static t_class *max_tilde_class;                /* Shared. */
static t_class *maxScalar_tilde_class;          /* Shared. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

typedef struct _binop_tilde t_max_tilde;
typedef struct _binopscalar_tilde t_maxscalar_tilde;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void max_tilde_dsp (t_max_tilde *x, t_signal **sp)
{
    object_fetchAndCopySignalValuesIfRequired (cast_object (x));

    dsp_addMaximumPerformAliased (sp[0]->s_vector, sp[1]->s_vector, sp[2]->s_vector, sp[0]->s_vectorSize);
}

static void maxScalar_tilde_dsp (t_maxscalar_tilde *x, t_signal **sp)
{
    binopScalar_tilde_restore (x);
    
    dsp_addMaximumScalarPerform (sp[0]->s_vector, &x->x_scalar, sp[1]->s_vector, sp[0]->s_vectorSize);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void *max_tilde_newWithScalar (t_symbol *s, int argc, t_atom *argv)
{
    t_maxscalar_tilde *x = (t_maxscalar_tilde *)pd_new (maxScalar_tilde_class);
    
    if (argc > 1) { warning_unusedArguments (cast_object (x), s, argc - 1, argv + 1); }
        
    x->x_scalar = atom_getFloatAtIndex (0, argc, argv);
    x->x_outlet = outlet_newSignal (cast_object (x));
    
    inlet_new2 (x, &s_float);
            
    return x;
}

static void *max_tilde_newWithSignal (t_symbol *s, int argc, t_atom *argv)
{
    t_max_tilde *x = (t_max_tilde *)pd_new (max_tilde_class);
    
    x->x_outlet = outlet_newSignal (cast_object (x));
    
    inlet_newSignal (cast_object (x));

    return x;
}

static void *max_tilde_new (t_symbol *s, int argc, t_atom *argv)
{
    if (argc) {
        return max_tilde_newWithScalar (s, argc, argv);
    } else {
        return max_tilde_newWithSignal (s, argc, argv);
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void max_tilde_setup (void)
{
    max_tilde_class = class_new (sym_max__tilde__,
                                    (t_newmethod)max_tilde_new,
                                    NULL,
                                    sizeof (t_max_tilde),
                                    CLASS_DEFAULT | CLASS_SIGNAL,
                                    A_GIMME,
                                    A_NULL);
    
    maxScalar_tilde_class = class_new (sym_max__tilde__,
                                    NULL,
                                    NULL,
                                    sizeof (t_maxscalar_tilde),
                                    CLASS_DEFAULT | CLASS_SIGNAL,
                                    A_NULL);
                
    class_addDsp (max_tilde_class, (t_method)max_tilde_dsp);
    class_addDsp (maxScalar_tilde_class, (t_method)maxScalar_tilde_dsp);
    
    class_addMethod (maxScalar_tilde_class,
        (t_method)binopScalar_tilde_float,
        sym__inlet2,
        A_FLOAT,
        A_NULL);

    class_setDataFunction (max_tilde_class, binop_tilde_functionData);
    class_setDataFunction (maxScalar_tilde_class, binopScalar_tilde_functionData);
    
    class_setHelpName (max_tilde_class, sym_math__tilde__);
    class_setHelpName (maxScalar_tilde_class, sym_math__tilde__);
}

PD_LOCAL void max_tilde_destroy (void)
{
    class_free (max_tilde_class);
    class_free (maxScalar_tilde_class);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

