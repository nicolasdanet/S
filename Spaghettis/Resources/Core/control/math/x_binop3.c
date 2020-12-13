
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#include "../../m_spaghettis.h"
#include "../../m_core.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "x_binop.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

static t_class *binopBitwiseAnd_class;          /* Shared. */
static t_class *binopLogicalAnd_class;          /* Shared. */
static t_class *binopBitwiseOr_class;           /* Shared. */
static t_class *binopLogicalOr_class;           /* Shared. */
static t_class *binopShiftLeft_class;           /* Shared. */
static t_class *binopShiftRight_class;          /* Shared. */
static t_class *binopModulo_class;              /* Shared. */
static t_class *binopIntegerModulo_class;       /* Shared. */
static t_class *binopIntegerDivide_class;       /* Shared. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

static void *binopBitwiseAnd_new (t_float f)
{
    return binop_new (binopBitwiseAnd_class, f);
}

static void binopBitwiseAnd_bang (t_binop *x)
{
    outlet_float (x->bo_outlet, (t_float)((int)(x->bo_f1) & (int)(x->bo_f2)));
}

static void binopBitwiseAnd_float (t_binop *x, t_float f)
{
    x->bo_f1 = f; binopBitwiseAnd_bang (x);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void *binopLogicalAnd_new (t_float f)
{
    return binop_new (binopLogicalAnd_class, f);
}

static void binopLogicalAnd_bang (t_binop *x)
{
    outlet_float (x->bo_outlet, (t_float)((int)(x->bo_f1) && (int)(x->bo_f2)));
}

static void binopLogicalAnd_float (t_binop *x, t_float f)
{
    x->bo_f1 = f; binopLogicalAnd_bang (x);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void *binopBitwiseOr_new (t_float f)
{
    return binop_new (binopBitwiseOr_class, f);
}

static void binopBitwiseOr_bang (t_binop *x)
{
    outlet_float (x->bo_outlet, (t_float)((int)(x->bo_f1) | (int)(x->bo_f2)));
}

static void binopBitwiseOr_float (t_binop *x, t_float f)
{
    x->bo_f1 = f; binopBitwiseOr_bang (x);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void *binopLogicalOr_new (t_float f)
{
    return binop_new (binopLogicalOr_class, f);
}

static void binopLogicalOr_bang (t_binop *x)
{
    outlet_float (x->bo_outlet, (t_float)((int)(x->bo_f1) || (int)(x->bo_f2)));
}

static void binopLogicalOr_float (t_binop *x, t_float f)
{
    x->bo_f1 = f; binopLogicalOr_bang (x);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void *binopShiftLeft_new (t_float f)
{
    return binop_new (binopShiftLeft_class, f);
}

static void binopShiftLeft_bang (t_binop *x)
{
    outlet_float (x->bo_outlet, (t_float)((int)(x->bo_f1) << (int)(x->bo_f2)));
}

static void binopShiftLeft_float (t_binop *x, t_float f)
{
    x->bo_f1 = f; binopShiftLeft_bang (x);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void *binopShiftRight_new (t_float f)
{
    return binop_new (binopShiftRight_class, f);
}

static void binopShiftRight_bang (t_binop *x)
{
    outlet_float (x->bo_outlet, (t_float)((int)(x->bo_f1) >> (int)(x->bo_f2)));
}

static void binopShiftRight_float (t_binop *x, t_float f)
{
    x->bo_f1 = f; binopShiftRight_bang (x);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void *binopModulo_new (t_float f)
{
    return binop_new (binopModulo_class, f);
}

static void binopModulo_bang (t_binop *x)
{
    int n1 = (int)x->bo_f1;
    int n2 = (int)x->bo_f2 == 0 ? 1 : (int)x->bo_f2;
    
    outlet_float (x->bo_outlet, (t_float)(n1 % n2));
}

static void binopModulo_float (t_binop *x, t_float f)
{
    x->bo_f1 = f; binopModulo_bang (x);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void *binopIntegerModulo_new (t_float f)
{
    return binop_new (binopIntegerModulo_class, f);
}

static void binopIntegerModulo_bang (t_binop *x)
{
    int n1 = (int)x->bo_f1;
    int n2 = (int)x->bo_f2 == 0 ? 1 : (int)x->bo_f2;
    int k  = n1 % n2;
    
    if (n2 > 0) {
        if (k < 0) { k += n2; }
    }
    
    outlet_float (x->bo_outlet, (t_float)k);
}

static void binopIntegerModulo_float (t_binop *x, t_float f)
{
    x->bo_f1 = f; binopIntegerModulo_bang (x);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void *binopIntegerDivide_new (t_float f)
{
    return binop_new (binopIntegerDivide_class, f);
}

static void binopIntegerDivide_bang (t_binop *x)
{
    int n1 = (int)x->bo_f1;
    int n2 = (int)x->bo_f2 == 0 ? 1 : (int)x->bo_f2;
    int k;
    
    if (n2 > 0) {
        if (n1 < 0) { n1 -= (n2 - 1); }
    } else {
        if (n1 < 0) { n1 = PD_ABS (n1); n2 = PD_ABS (n2); }
        else {
            n1 += (n2 - 1);
        }
    }
    
    k = n1 / n2;
    
    outlet_float (x->bo_outlet, (t_float)k);
}

static void binopIntegerDivide_float (t_binop *x, t_float f)
{
    x->bo_f1 = f; binopIntegerDivide_bang (x);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void binop3_setup (void)
{
    binopBitwiseAnd_class = class_new (sym___ampersand__, 
                                    (t_newmethod)binopBitwiseAnd_new,
                                    NULL,
                                    sizeof (t_binop),
                                    CLASS_DEFAULT,
                                    A_DEFFLOAT,
                                    A_NULL);
                                    
    binopLogicalAnd_class = class_new (sym___ampersand____ampersand__,
                                    (t_newmethod)binopLogicalAnd_new,
                                    NULL,
                                    sizeof (t_binop),
                                    CLASS_DEFAULT,
                                    A_DEFFLOAT,
                                    A_NULL);
                                    
    binopBitwiseOr_class = class_new (sym___bar__,
                                    (t_newmethod)binopBitwiseOr_new,
                                    NULL,
                                    sizeof (t_binop),
                                    CLASS_DEFAULT,
                                    A_DEFFLOAT,
                                    A_NULL);
                                    
    binopLogicalOr_class = class_new (sym___bar____bar__,
                                    (t_newmethod)binopLogicalOr_new,
                                    NULL,
                                    sizeof (t_binop),
                                    CLASS_DEFAULT,
                                    A_DEFFLOAT,
                                    A_NULL);
                                    
    binopShiftLeft_class = class_new (sym___less____less__,
                                    (t_newmethod)binopShiftLeft_new,
                                    NULL,
                                    sizeof (t_binop),
                                    CLASS_DEFAULT,
                                    A_DEFFLOAT,
                                    A_NULL);

    binopShiftRight_class = class_new (sym___greater____greater__,
                                    (t_newmethod)binopShiftRight_new,
                                    NULL,
                                    sizeof (t_binop),
                                    CLASS_DEFAULT,
                                    A_DEFFLOAT,
                                    A_NULL);
                                    
    binopModulo_class = class_new (sym___percent__,
                                    (t_newmethod)binopModulo_new,
                                    NULL,
                                    sizeof (t_binop),
                                    CLASS_DEFAULT,
                                    A_DEFFLOAT,
                                    A_NULL);

    binopIntegerModulo_class = class_new (sym_mod,
                                    (t_newmethod)binopIntegerModulo_new,
                                    NULL,
                                    sizeof (t_binop),
                                    CLASS_DEFAULT,
                                    A_DEFFLOAT,
                                    A_NULL);
                                    
    binopIntegerDivide_class = class_new (sym_div, 
                                    (t_newmethod)binopIntegerDivide_new,
                                    NULL,
                                    sizeof (t_binop),
                                    CLASS_DEFAULT,
                                    A_DEFFLOAT,
                                    A_NULL);
                                    
    class_addBang (binopBitwiseAnd_class,       (t_method)binopBitwiseAnd_bang);
    class_addBang (binopLogicalAnd_class,       (t_method)binopLogicalAnd_bang);
    class_addBang (binopBitwiseOr_class,        (t_method)binopBitwiseOr_bang);
    class_addBang (binopLogicalOr_class,        (t_method)binopLogicalOr_bang);
    class_addBang (binopShiftLeft_class,        (t_method)binopShiftLeft_bang);
    class_addBang (binopShiftRight_class,       (t_method)binopShiftRight_bang);
    class_addBang (binopModulo_class,           (t_method)binopModulo_bang);
    class_addBang (binopIntegerModulo_class,    (t_method)binopIntegerModulo_bang);
    class_addBang (binopIntegerDivide_class,    (t_method)binopIntegerDivide_bang);
        
    class_addFloat (binopBitwiseAnd_class,      (t_method)binopBitwiseAnd_float);
    class_addFloat (binopLogicalAnd_class,      (t_method)binopLogicalAnd_float);
    class_addFloat (binopBitwiseOr_class,       (t_method)binopBitwiseOr_float);
    class_addFloat (binopLogicalOr_class,       (t_method)binopLogicalOr_float);
    class_addFloat (binopShiftLeft_class,       (t_method)binopShiftLeft_float);
    class_addFloat (binopShiftRight_class,      (t_method)binopShiftRight_float);
    class_addFloat (binopModulo_class,          (t_method)binopModulo_float);
    class_addFloat (binopIntegerModulo_class,   (t_method)binopIntegerModulo_float);
    class_addFloat (binopIntegerDivide_class,   (t_method)binopIntegerDivide_float);
    
    class_addMethod (binopBitwiseAnd_class,     (t_method)binop_restore, sym__restore, A_GIMME, A_NULL);
    class_addMethod (binopLogicalAnd_class,     (t_method)binop_restore, sym__restore, A_GIMME, A_NULL);
    class_addMethod (binopBitwiseOr_class,      (t_method)binop_restore, sym__restore, A_GIMME, A_NULL);
    class_addMethod (binopLogicalOr_class,      (t_method)binop_restore, sym__restore, A_GIMME, A_NULL);
    class_addMethod (binopShiftLeft_class,      (t_method)binop_restore, sym__restore, A_GIMME, A_NULL);
    class_addMethod (binopShiftRight_class,     (t_method)binop_restore, sym__restore, A_GIMME, A_NULL);
    class_addMethod (binopModulo_class,         (t_method)binop_restore, sym__restore, A_GIMME, A_NULL);
    class_addMethod (binopIntegerModulo_class,  (t_method)binop_restore, sym__restore, A_GIMME, A_NULL);
    class_addMethod (binopIntegerDivide_class,  (t_method)binop_restore, sym__restore, A_GIMME, A_NULL);

    class_setDataFunction (binopBitwiseAnd_class,       binop_functionData);
    class_setDataFunction (binopLogicalAnd_class,       binop_functionData);
    class_setDataFunction (binopBitwiseOr_class,        binop_functionData);
    class_setDataFunction (binopLogicalOr_class,        binop_functionData);
    class_setDataFunction (binopShiftLeft_class,        binop_functionData);
    class_setDataFunction (binopShiftRight_class,       binop_functionData);
    class_setDataFunction (binopModulo_class,           binop_functionData);
    class_setDataFunction (binopIntegerModulo_class,    binop_functionData);
    class_setDataFunction (binopIntegerDivide_class,    binop_functionData);

    class_requirePending (binopBitwiseAnd_class);
    class_requirePending (binopLogicalAnd_class);
    class_requirePending (binopBitwiseOr_class);
    class_requirePending (binopLogicalOr_class);
    class_requirePending (binopShiftLeft_class);
    class_requirePending (binopShiftRight_class);
    class_requirePending (binopModulo_class);
    class_requirePending (binopIntegerModulo_class);
    class_requirePending (binopIntegerDivide_class);

    class_setHelpName (binopBitwiseAnd_class,           sym_bitwise);
    class_setHelpName (binopLogicalAnd_class,           sym_logical);
    class_setHelpName (binopBitwiseOr_class,            sym_bitwise);
    class_setHelpName (binopLogicalOr_class,            sym_logical);
    class_setHelpName (binopShiftLeft_class,            sym_bitwise);
    class_setHelpName (binopShiftRight_class,           sym_bitwise);
    class_setHelpName (binopModulo_class,               sym_math);
    class_setHelpName (binopIntegerModulo_class,        sym_math);
    class_setHelpName (binopIntegerDivide_class,        sym_math);
}

PD_LOCAL void binop3_destroy (void)
{
    class_free (binopBitwiseAnd_class);
    class_free (binopLogicalAnd_class);
    class_free (binopBitwiseOr_class);
    class_free (binopLogicalOr_class);
    class_free (binopShiftLeft_class);
    class_free (binopShiftRight_class);
    class_free (binopModulo_class);
    class_free (binopIntegerModulo_class);
    class_free (binopIntegerDivide_class);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
