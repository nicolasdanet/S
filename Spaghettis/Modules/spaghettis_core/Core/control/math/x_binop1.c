
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#include "../../m_headers.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "x_binop.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

static t_class *binopAdd_class;             /* Shared. */
static t_class *binopSubtract_class;        /* Shared. */
static t_class *binopMultiply_class;        /* Shared. */
static t_class *binopDivide_class;          /* Shared. */
static t_class *binopPower_class;           /* Shared. */
static t_class *binopLog_class;             /* Shared. */
static t_class *binopMaximum_class;         /* Shared. */
static t_class *binopMinimum_class;         /* Shared. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

void *binop_new (t_class *c, t_float f)
{
    t_binop *x = (t_binop *)pd_new (c);
    
    x->bo_f1 = 0.0;
    x->bo_f2 = f;
    x->bo_outlet = outlet_newFloat (cast_object (x));
    
    inlet_newFloat (cast_object (x), &x->bo_f2);

    return x;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

t_buffer *binop_functionData (t_object *z, int flags)
{
    if (SAVED_DEEP (flags)) {
    //
    t_binop *x  = (t_binop *)z;
    t_buffer *b = buffer_new();
    
    buffer_appendSymbol (b, sym__restore);
    buffer_appendFloat (b, x->bo_f1);
    buffer_appendFloat (b, x->bo_f2);
    
    return b;
    //
    }
    
    return NULL;
}

void binop_restore (t_binop *x, t_symbol *s, int argc, t_atom *argv)
{
    t_binop *old = (t_binop *)instance_pendingFetch (cast_object (x));
    
    t_float f1 = old ? old->bo_f1 : atom_getFloatAtIndex (0, argc, argv);
    t_float f2 = old ? old->bo_f2 : atom_getFloatAtIndex (1, argc, argv);
    
    x->bo_f1 = f1;
    x->bo_f2 = f2;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void *binopAdd_new (t_float f)
{
    return binop_new (binopAdd_class, f);
}

static void binopAdd_bang (t_binop *x)
{
    outlet_float (x->bo_outlet, x->bo_f1 + x->bo_f2);
}

static void binopAdd_float (t_binop *x, t_float f)
{
    x->bo_f1 = f; binopAdd_bang (x);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void *binopSubtract_new (t_float f)
{
    return binop_new (binopSubtract_class, f);
}

static void binopSubtract_bang (t_binop *x)
{
    outlet_float (x->bo_outlet, x->bo_f1 - x->bo_f2);
}

static void binopSubtract_float (t_binop *x, t_float f)
{
    x->bo_f1 = f; binopSubtract_bang (x);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void *binopMultiply_new (t_float f)
{
    return binop_new (binopMultiply_class, f);
}

static void binopMultiply_bang (t_binop *x)
{
    outlet_float (x->bo_outlet, x->bo_f1 * x->bo_f2);
}

static void binopMultiply_float (t_binop *x, t_float f)
{
    x->bo_f1 = f; binopMultiply_bang (x);
}
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void *binopDivide_new (t_float f)
{
    return binop_new (binopDivide_class, f);
}

static void binopDivide_bang (t_binop *x)
{
    outlet_float (x->bo_outlet, (x->bo_f2 != 0.0 ? x->bo_f1 / x->bo_f2 : 0.0));
}

static void binopDivide_float (t_binop *x, t_float f)
{
    x->bo_f1 = f; binopDivide_bang (x);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void *binopPower_new (t_float f)
{
    return binop_new (binopPower_class, f);
}

static void binopPower_bang (t_binop *x)
{
    /* Avoid divide by zero with negative power. */
    /* Avoid root of negative numbers with fractional power. */
    
    int err = (x->bo_f1 == 0.0 && x->bo_f2 < 0.0) || (x->bo_f1 < 0.0 && !PD_FLOAT64_IS_INTEGER (x->bo_f2));
    
    if (err) {
        outlet_float (x->bo_outlet, 0.0);
    } else {
        outlet_float (x->bo_outlet, pow (x->bo_f1, x->bo_f2));
    }
}

static void binopPower_float (t_binop *x, t_float f)
{
    x->bo_f1 = f; binopPower_bang (x);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void *binopLog_new (t_float f)
{
    return binop_new (binopLog_class, f);
}

static void binopLog_bang (t_binop *x)
{
    t_float f = -1000.0;
    
    if (x->bo_f1 > 0.0) { f = (x->bo_f2 <= 0.0) ? log (x->bo_f1) : (log (x->bo_f1) / log (x->bo_f2)); }
    
    outlet_float (x->bo_outlet, f);
}

static void binopLog_float (t_binop *x, t_float f)
{
    x->bo_f1 = f; binopLog_bang (x);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void *binopMaximum_new (t_float f)
{
    return binop_new (binopMaximum_class, f);
}

static void binopMaximum_bang (t_binop *x)
{
    outlet_float (x->bo_outlet, PD_MAX (x->bo_f1, x->bo_f2));
}

static void binopMaximum_float (t_binop *x, t_float f)
{
    x->bo_f1 = f; binopMaximum_bang (x);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void *binopMinimum_new (t_float f)
{
    return binop_new (binopMinimum_class, f);
}

static void binopMinimum_bang (t_binop *x)
{
    outlet_float (x->bo_outlet, PD_MIN (x->bo_f1, x->bo_f2));
}

static void binopMinimum_float (t_binop *x, t_float f)
{
    x->bo_f1 = f; binopMinimum_bang (x);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void binop1_setup (void)
{
    binopAdd_class = class_new (sym___plus__,
                            (t_newmethod)binopAdd_new,
                            NULL,
                            sizeof (t_binop),
                            CLASS_DEFAULT,
                            A_DEFFLOAT,
                            A_NULL);
                            
    binopSubtract_class = class_new (sym___minus__,
                            (t_newmethod)binopSubtract_new,
                            NULL,
                            sizeof (t_binop),
                            CLASS_DEFAULT,
                            A_DEFFLOAT,
                            A_NULL);
                            
    binopMultiply_class = class_new (sym___asterisk__,
                            (t_newmethod)binopMultiply_new,
                            NULL,
                            sizeof (t_binop),
                            CLASS_DEFAULT,
                            A_DEFFLOAT,
                            A_NULL);

    binopDivide_class = class_new (sym___slash__,
                            (t_newmethod)binopDivide_new,
                            NULL,
                            sizeof (t_binop),
                            CLASS_DEFAULT,
                            A_DEFFLOAT,
                            A_NULL);
                            
    binopPower_class = class_new (sym_pow,
                            (t_newmethod)binopPower_new,
                            NULL,
                            sizeof (t_binop),
                            CLASS_DEFAULT,
                            A_DEFFLOAT,
                            A_NULL);
    
    binopLog_class = class_new (sym_log,
                            (t_newmethod)binopLog_new,
                            NULL,
                            sizeof (t_binop),
                            CLASS_DEFAULT,
                            A_DEFFLOAT,
                            A_NULL);
    
    binopMaximum_class = class_new (sym_max,
                            (t_newmethod)binopMaximum_new,
                            NULL,
                            sizeof (t_binop),
                            CLASS_DEFAULT,
                            A_DEFFLOAT,
                            A_NULL);
                            
    binopMinimum_class = class_new (sym_min,
                            (t_newmethod)binopMinimum_new,
                            NULL,
                            sizeof (t_binop),
                            CLASS_DEFAULT,
                            A_DEFFLOAT,
                            A_NULL);

    class_addBang (binopAdd_class,              (t_method)binopAdd_bang);
    class_addBang (binopSubtract_class,         (t_method)binopSubtract_bang);
    class_addBang (binopMultiply_class,         (t_method)binopMultiply_bang);
    class_addBang (binopDivide_class,           (t_method)binopDivide_bang);
    class_addBang (binopPower_class,            (t_method)binopPower_bang);
    class_addBang (binopLog_class,              (t_method)binopLog_bang);
    class_addBang (binopMaximum_class,          (t_method)binopMaximum_bang);
    class_addBang (binopMinimum_class,          (t_method)binopMinimum_bang);
        
    class_addFloat (binopAdd_class,             (t_method)binopAdd_float);
    class_addFloat (binopSubtract_class,        (t_method)binopSubtract_float);
    class_addFloat (binopMultiply_class,        (t_method)binopMultiply_float);
    class_addFloat (binopDivide_class,          (t_method)binopDivide_float);
    class_addFloat (binopPower_class,           (t_method)binopPower_float);
    class_addFloat (binopLog_class,             (t_method)binopLog_float);
    class_addFloat (binopMaximum_class,         (t_method)binopMaximum_float);
    class_addFloat (binopMinimum_class,         (t_method)binopMinimum_float);
    
    class_addMethod (binopAdd_class,            (t_method)binop_restore, sym__restore, A_GIMME, A_NULL);
    class_addMethod (binopSubtract_class,       (t_method)binop_restore, sym__restore, A_GIMME, A_NULL);
    class_addMethod (binopMultiply_class,       (t_method)binop_restore, sym__restore, A_GIMME, A_NULL);
    class_addMethod (binopDivide_class,         (t_method)binop_restore, sym__restore, A_GIMME, A_NULL);
    class_addMethod (binopPower_class,          (t_method)binop_restore, sym__restore, A_GIMME, A_NULL);
    class_addMethod (binopLog_class,            (t_method)binop_restore, sym__restore, A_GIMME, A_NULL);
    class_addMethod (binopMaximum_class,        (t_method)binop_restore, sym__restore, A_GIMME, A_NULL);
    class_addMethod (binopMinimum_class,        (t_method)binop_restore, sym__restore, A_GIMME, A_NULL);
    
    class_setDataFunction (binopAdd_class,      binop_functionData);
    class_setDataFunction (binopSubtract_class, binop_functionData);
    class_setDataFunction (binopMultiply_class, binop_functionData);
    class_setDataFunction (binopDivide_class,   binop_functionData);
    class_setDataFunction (binopPower_class,    binop_functionData);
    class_setDataFunction (binopLog_class,      binop_functionData);
    class_setDataFunction (binopMaximum_class,  binop_functionData);
    class_setDataFunction (binopMinimum_class,  binop_functionData);
    
    class_requirePending (binopAdd_class);
    class_requirePending (binopSubtract_class);
    class_requirePending (binopMultiply_class);
    class_requirePending (binopDivide_class);
    class_requirePending (binopPower_class);
    class_requirePending (binopLog_class);
    class_requirePending (binopMaximum_class);
    class_requirePending (binopMinimum_class);
    
    class_setHelpName (binopAdd_class,          sym_arithmetic);
    class_setHelpName (binopSubtract_class,     sym_arithmetic);
    class_setHelpName (binopMultiply_class,     sym_arithmetic);
    class_setHelpName (binopDivide_class,       sym_arithmetic);
    class_setHelpName (binopPower_class,        sym_math);
    class_setHelpName (binopLog_class,          sym_math);
    class_setHelpName (binopMaximum_class,      sym_math);
    class_setHelpName (binopMinimum_class,      sym_math);
}

void binop1_destroy (void)
{
    class_free (binopAdd_class);
    class_free (binopSubtract_class);
    class_free (binopMultiply_class);
    class_free (binopDivide_class);
    class_free (binopPower_class);
    class_free (binopLog_class);
    class_free (binopMaximum_class);
    class_free (binopMinimum_class);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
