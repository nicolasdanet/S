
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#include "../../m_spaghettis.h"
#include "../../m_core.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "x_unop.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

static t_class *mtof_class;         /* Shared. */
static t_class *ftom_class;         /* Shared. */
static t_class *powtodb_class;      /* Shared. */
static t_class *dbtopow_class;      /* Shared. */
static t_class *rmstodb_class;      /* Shared. */
static t_class *dbtorms_class;      /* Shared. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

typedef t_unop t_acoustic;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

t_buffer *unop_functionData (t_object *z, int flags)
{
    if (SAVED_DEEP (flags)) {
    //
    t_unop *x  = (t_unop *)z;
    t_buffer *b = buffer_new();
    
    buffer_appendSymbol (b, sym__restore);
    buffer_appendFloat (b, x->uo_f);
    
    return b;
    //
    }
    
    return NULL;
}

void unop_restore (t_unop *x, t_float f)
{
    t_unop *old = (t_unop *)instance_pendingFetch (cast_object (x));

    x->uo_f = old ? old->uo_f : f;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void *mtof_new (void)
{
    t_acoustic *x = (t_acoustic *)pd_new (mtof_class);
    
    x->uo_outlet = outlet_newFloat (cast_object (x));
    
    return x;
}

static void mtof_bang (t_acoustic *x)
{
    outlet_float (x->uo_outlet, math_midiToFrequency (x->uo_f));
}

static void mtof_float (t_acoustic *x, t_float f)
{
    x->uo_f = f; mtof_bang (x);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void *ftom_new (void)
{
    t_acoustic *x = (t_acoustic *)pd_new (ftom_class);
    
    x->uo_outlet = outlet_newFloat (cast_object (x));
    
    return x;
}

static void ftom_bang (t_acoustic *x)
{
    outlet_float (x->uo_outlet, math_frequencyToMidi (x->uo_f));
}

static void ftom_float (t_acoustic *x, t_float f)
{
    x->uo_f = f; ftom_bang (x);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void *powtodb_new (void)
{
    t_acoustic *x = (t_acoustic *)pd_new (powtodb_class);
    
    x->uo_outlet = outlet_newFloat (cast_object (x));
    
    return x;
}

static void powtodb_bang (t_acoustic *x)
{
    outlet_float (x->uo_outlet, math_powerToDecibel (x->uo_f));
}

static void powtodb_float (t_acoustic *x, t_float f)
{
    x->uo_f = f; powtodb_bang (x);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void *dbtopow_new (void)
{
    t_acoustic *x = (t_acoustic *)pd_new (dbtopow_class);
    
    x->uo_outlet = outlet_newFloat (cast_object (x));
    
    return x;
}

static void dbtopow_bang (t_acoustic *x)
{
    outlet_float (x->uo_outlet, math_decibelToPower (x->uo_f));
}

static void dbtopow_float (t_acoustic *x, t_float f)
{
    x->uo_f = f; dbtopow_bang (x);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void *rmstodb_new (void)
{
    t_acoustic *x = (t_acoustic *)pd_new (rmstodb_class);
    
    x->uo_outlet = outlet_newFloat (cast_object (x));
    
    return x;
}

static void rmstodb_bang (t_acoustic *x)
{
    outlet_float (x->uo_outlet, math_rootMeanSquareToDecibel (x->uo_f));
}

static void rmstodb_float (t_acoustic *x, t_float f)
{
    x->uo_f = f; rmstodb_bang (x);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void *dbtorms_new (void)
{
    t_acoustic *x = (t_acoustic *)pd_new (dbtorms_class);
    
    x->uo_outlet = outlet_newFloat (cast_object (x));
    
    return x;
}

static void dbtorms_bang (t_acoustic *x)
{
    outlet_float (x->uo_outlet, math_decibelToRootMeanSquare (x->uo_f));
}

static void dbtorms_float (t_acoustic *x, t_float f)
{
    x->uo_f = f; dbtorms_bang (x);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void acoustic_setup (void)
{
    mtof_class = class_new (sym_mtof, 
                        (t_newmethod)mtof_new,
                        NULL,
                        sizeof (t_acoustic),
                        CLASS_DEFAULT,
                        A_NULL);

    ftom_class = class_new (sym_ftom,
                        (t_newmethod)ftom_new,
                        NULL,
                        sizeof (t_acoustic),
                        CLASS_DEFAULT,
                        A_NULL);

    powtodb_class = class_new (sym_powtodb,
                        (t_newmethod)powtodb_new,
                        NULL,
                        sizeof (t_acoustic),
                        CLASS_DEFAULT,
                        A_NULL);
                        
    dbtopow_class = class_new (sym_dbtopow, 
                        (t_newmethod)dbtopow_new,
                        NULL,
                        sizeof (t_acoustic),
                        CLASS_DEFAULT,
                        A_NULL);

    rmstodb_class = class_new (sym_rmstodb,
                        (t_newmethod)rmstodb_new,
                        NULL,
                        sizeof (t_acoustic),
                        CLASS_DEFAULT,
                        A_NULL);

    dbtorms_class = class_new (sym_dbtorms,
                        (t_newmethod)dbtorms_new,
                        NULL,
                        sizeof (t_acoustic),
                        CLASS_DEFAULT,
                        A_NULL);
    
    class_addBang (mtof_class,              (t_method)mtof_bang);
    class_addBang (ftom_class,              (t_method)ftom_bang);
    class_addBang (powtodb_class,           (t_method)powtodb_bang);
    class_addBang (dbtopow_class,           (t_method)dbtopow_bang);
    class_addBang (rmstodb_class,           (t_method)rmstodb_bang);
    class_addBang (dbtorms_class,           (t_method)dbtorms_bang);
    
    class_addFloat (mtof_class,             (t_method)mtof_float);
    class_addFloat (ftom_class,             (t_method)ftom_float);
    class_addFloat (powtodb_class,          (t_method)powtodb_float);
    class_addFloat (dbtopow_class,          (t_method)dbtopow_float);
    class_addFloat (rmstodb_class,          (t_method)rmstodb_float);
    class_addFloat (dbtorms_class,          (t_method)dbtorms_float);
    
    class_addMethod (mtof_class,            (t_method)unop_restore, sym__restore, A_FLOAT, A_NULL);
    class_addMethod (ftom_class,            (t_method)unop_restore, sym__restore, A_FLOAT, A_NULL);
    class_addMethod (powtodb_class,         (t_method)unop_restore, sym__restore, A_FLOAT, A_NULL);
    class_addMethod (dbtopow_class,         (t_method)unop_restore, sym__restore, A_FLOAT, A_NULL);
    class_addMethod (rmstodb_class,         (t_method)unop_restore, sym__restore, A_FLOAT, A_NULL);
    class_addMethod (dbtorms_class,         (t_method)unop_restore, sym__restore, A_FLOAT, A_NULL);
    
    class_setDataFunction (mtof_class,      unop_functionData);
    class_setDataFunction (ftom_class,      unop_functionData);
    class_setDataFunction (powtodb_class,   unop_functionData);
    class_setDataFunction (dbtopow_class,   unop_functionData);
    class_setDataFunction (rmstodb_class,   unop_functionData);
    class_setDataFunction (dbtorms_class,   unop_functionData);
    
    class_requirePending (mtof_class);
    class_requirePending (ftom_class);
    class_requirePending (powtodb_class);
    class_requirePending (dbtopow_class);
    class_requirePending (rmstodb_class);
    class_requirePending (dbtorms_class);
    
    class_setHelpName (mtof_class,          sym_acoustic);
    class_setHelpName (ftom_class,          sym_acoustic);
    class_setHelpName (powtodb_class,       sym_acoustic);
    class_setHelpName (dbtopow_class,       sym_acoustic);
    class_setHelpName (rmstodb_class,       sym_acoustic);
    class_setHelpName (dbtorms_class,       sym_acoustic);
    
    class_addCreator ((t_newmethod)dbtorms_new, sym_dbtoamp, A_NULL);
    class_addCreator ((t_newmethod)rmstodb_new, sym_amptodb, A_NULL);
}

void acoustic_destroy (void)
{
    class_free (mtof_class);
    class_free (ftom_class);
    class_free (powtodb_class);
    class_free (dbtopow_class);
    class_free (rmstodb_class);
    class_free (dbtorms_class);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

