
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

static t_class *binopEquals_class;              /* Shared. */
static t_class *binopNotEquals_class;           /* Shared. */
static t_class *binopGreater_class;             /* Shared. */
static t_class *binopLess_class;                /* Shared. */
static t_class *binopGreaterEquals_class;       /* Shared. */
static t_class *binopLessEquals_class;          /* Shared. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

static void *binopEquals_new (t_float f)
{
    return binop_new (binopEquals_class, f);
}

static void binopEquals_bang (t_binop *x)
{
    outlet_float (x->bo_outlet, x->bo_f1 == x->bo_f2);
}

static void binopEquals_float (t_binop *x, t_float f)
{
    x->bo_f1 = f; binopEquals_bang (x);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void *binopNotEquals_new (t_float f)
{
    return binop_new (binopNotEquals_class, f);
}

static void binopNotEquals_bang (t_binop *x)
{
    outlet_float (x->bo_outlet, x->bo_f1 != x->bo_f2);
}

static void binopNotEquals_float (t_binop *x, t_float f)
{
    x->bo_f1 = f; binopNotEquals_bang (x);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void *binopGreater_new (t_float f)
{
    return binop_new (binopGreater_class, f);
}

static void binopGreater_bang (t_binop *x)
{
    outlet_float (x->bo_outlet, x->bo_f1 > x->bo_f2);
}

static void binopGreater_float (t_binop *x, t_float f)
{
    x->bo_f1 = f; binopGreater_bang (x);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void *binopLess_new (t_float f)
{
    return binop_new (binopLess_class, f);
}

static void binopLess_bang (t_binop *x)
{
    outlet_float (x->bo_outlet, x->bo_f1 < x->bo_f2);
}

static void binopLess_float (t_binop *x, t_float f)
{
    x->bo_f1 = f; binopLess_bang (x);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void *binopGreaterEquals_new (t_float f)
{
    return binop_new (binopGreaterEquals_class, f);
}

static void binopGreaterEquals_bang (t_binop *x)
{
    outlet_float (x->bo_outlet, x->bo_f1 >= x->bo_f2);
}

static void binopGreaterEquals_float (t_binop *x, t_float f)
{
    x->bo_f1 = f; binopGreaterEquals_bang (x);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void *binopLessEquals_new (t_float f)
{
    return binop_new (binopLessEquals_class, f);
}

static void binopLessEquals_bang (t_binop *x)
{
    outlet_float (x->bo_outlet, x->bo_f1 <= x->bo_f2);
}

static void binopLessEquals_float (t_binop *x, t_float f)
{
    x->bo_f1 = f; binopLessEquals_bang (x);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void binop2_setup (void)
{
    binopEquals_class = class_new (sym___equals____equals__,
                                (t_newmethod)binopEquals_new,
                                NULL,
                                sizeof (t_binop),
                                CLASS_DEFAULT,
                                A_DEFFLOAT,
                                A_NULL);
                            
    binopNotEquals_class = class_new (sym___exclamation____equals__,
                                (t_newmethod)binopNotEquals_new,
                                NULL,
                                sizeof (t_binop),
                                CLASS_DEFAULT,
                                A_DEFFLOAT,
                                A_NULL);
                            
    binopGreater_class = class_new (sym___greater__,
                                (t_newmethod)binopGreater_new,
                                NULL,
                                sizeof (t_binop),
                                CLASS_DEFAULT,
                                A_DEFFLOAT,
                                A_NULL);
                            
    binopLess_class = class_new (sym___less__,
                                (t_newmethod)binopLess_new,
                                NULL,
                                sizeof (t_binop),
                                CLASS_DEFAULT,
                                A_DEFFLOAT,
                                A_NULL);
                            
    binopGreaterEquals_class = class_new (sym___greater____equals__,
                                (t_newmethod)binopGreaterEquals_new,
                                NULL,
                                sizeof (t_binop),
                                CLASS_DEFAULT,
                                A_DEFFLOAT,
                                A_NULL);
                                
    binopLessEquals_class = class_new (sym___less____equals__,
                                (t_newmethod)binopLessEquals_new,
                                NULL,
                                sizeof (t_binop),
                                CLASS_DEFAULT,
                                A_DEFFLOAT,
                                A_NULL);
                                
    class_addBang (binopEquals_class,           (t_method)binopEquals_bang);
    class_addBang (binopNotEquals_class,        (t_method)binopNotEquals_bang);
    class_addBang (binopGreater_class,          (t_method)binopGreater_bang);
    class_addBang (binopLess_class,             (t_method)binopLess_bang);
    class_addBang (binopGreaterEquals_class,    (t_method)binopGreaterEquals_bang);
    class_addBang (binopLessEquals_class,       (t_method)binopLessEquals_bang);
        
    class_addFloat (binopEquals_class,          (t_method)binopEquals_float);
    class_addFloat (binopNotEquals_class,       (t_method)binopNotEquals_float);
    class_addFloat (binopGreater_class,         (t_method)binopGreater_float);
    class_addFloat (binopLess_class,            (t_method)binopLess_float);
    class_addFloat (binopGreaterEquals_class,   (t_method)binopGreaterEquals_float);
    class_addFloat (binopLessEquals_class,      (t_method)binopLessEquals_float);
    
    class_addMethod (binopEquals_class,         (t_method)binop_restore, sym__restore, A_GIMME, A_NULL);
    class_addMethod (binopNotEquals_class,      (t_method)binop_restore, sym__restore, A_GIMME, A_NULL);
    class_addMethod (binopGreater_class,        (t_method)binop_restore, sym__restore, A_GIMME, A_NULL);
    class_addMethod (binopLess_class,           (t_method)binop_restore, sym__restore, A_GIMME, A_NULL);
    class_addMethod (binopGreaterEquals_class,  (t_method)binop_restore, sym__restore, A_GIMME, A_NULL);
    class_addMethod (binopLessEquals_class,     (t_method)binop_restore, sym__restore, A_GIMME, A_NULL);
    
    class_setDataFunction (binopEquals_class,           binop_functionData);
    class_setDataFunction (binopNotEquals_class,        binop_functionData);
    class_setDataFunction (binopGreater_class,          binop_functionData);
    class_setDataFunction (binopLess_class,             binop_functionData);
    class_setDataFunction (binopGreaterEquals_class,    binop_functionData);
    class_setDataFunction (binopLessEquals_class,       binop_functionData);
    
    class_requirePending (binopEquals_class);
    class_requirePending (binopNotEquals_class);
    class_requirePending (binopGreater_class);
    class_requirePending (binopLess_class);
    class_requirePending (binopGreaterEquals_class);
    class_requirePending (binopLessEquals_class);

    class_setHelpName (binopEquals_class,               sym_logical);
    class_setHelpName (binopNotEquals_class,            sym_logical);
    class_setHelpName (binopGreater_class,              sym_logical);
    class_setHelpName (binopLess_class,                 sym_logical);
    class_setHelpName (binopGreaterEquals_class,        sym_logical);
    class_setHelpName (binopLessEquals_class,           sym_logical);
}

void binop2_destroy (void)
{
    class_free (binopEquals_class);
    class_free (binopNotEquals_class);
    class_free (binopGreater_class);
    class_free (binopLess_class);
    class_free (binopGreaterEquals_class);
    class_free (binopLessEquals_class);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
