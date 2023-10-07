
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#include "../../m_spaghettis.h"
#include "../../m_core.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#define DIAL_DIGITS_DEFAULT             5
#define DIAL_DIGITS_MAXIMUM             64
#define DIAL_INTERVAL_DEFAULT           1
#define DIAL_SIZE_DEFAULT               40
#define DIAL_SIZE_MINIMUM               8
#define DIAL_SIZE_MAXIMUM               256
#define DIAL_MINIMUM_DEFAULT            0
#define DIAL_MAXIMUM_DEFAULT            127
       
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#define DIAL_STEPS_MAXIMUM              (1024 * 1024)       /* Arbitrary. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static t_class *dial_class;             /* Shared. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

typedef struct _dial {
    t_object        x_obj;              /* MUST be the first. */
    int             x_size;
    int             x_position;
    int             x_isLogarithmic;
    int             x_digitsNumber;
    int             x_interval;
    t_float         x_minimum;
    t_float         x_maximum;
    t_float         x_float;
    t_outlet        *x_outlet;
    } t_dial;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void dial_set (t_dial *, t_float);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static t_float dial_getStepValue (t_dial *x)
{
    if (x->x_isLogarithmic) {
        return (log (x->x_maximum / x->x_minimum) / (t_float)x->x_interval);
    } else {
        return ((x->x_maximum - x->x_minimum) / (t_float)x->x_interval);
    }
}

static t_float dial_getValue (t_dial *x)
{
    t_float f, t = dial_getStepValue (x) * (t_float)x->x_position;
    
    if (x->x_isLogarithmic) { 
        f = x->x_minimum * exp (t); 
    } else {
        f = x->x_minimum + t;
    }
    
    if ((f < 1.0e-10) && (f > -1.0e-10)) { f = 0.0; }
    
    return f;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void dial_setRange (t_dial *x, t_float minimum, t_float maximum)
{
    t_error err = PD_ERROR_NONE;
    
    err |= minimum > maximum;
    
    if (x->x_isLogarithmic) {
        err |= (minimum == 0.0);
        err |= (maximum * minimum < 0.0);
    }
    
    if (err) { 
        x->x_isLogarithmic = 0;
        error_invalid (cast_object (x), sym_dial, sym_range);
        
    } else {
        x->x_minimum = minimum;
        x->x_maximum = maximum;
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void dial_bang (t_dial *x)
{
    outlet_float (x->x_outlet, x->x_float);
}

static void dial_float (t_dial *x, t_float f)
{
    dial_set (x, f); dial_bang (x);
}

static void dial_list (t_dial *x, t_symbol *s, int argc, t_atom *argv)
{
    if (argc && IS_FLOAT (argv)) { dial_float (x, atom_getFloat (argv)); }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void dial_size (t_dial *x, t_symbol *s, int argc, t_atom *argv)
{
    if (argc) {
    //
    int digits = (int)atom_getFloatAtIndex (0, argc, argv);

    x->x_digitsNumber = PD_CLAMP (digits, 1, DIAL_DIGITS_MAXIMUM);
    
    if (argc > 1) {
    //
    int n = (int)atom_getFloatAtIndex (1, argc, argv); x->x_size = PD_MAX (n, DIAL_SIZE_MINIMUM);
    //
    }
    //
    }
}

static void dial_range (t_dial *x, t_symbol *s, int argc, t_atom *argv)
{
    t_float minimum = atom_getFloatAtIndex (0, argc, argv);
    t_float maximum = atom_getFloatAtIndex (1, argc, argv);
    
    dial_setRange (x, minimum, maximum);
    
    x->x_float = dial_getValue (x);
}

static void dial_set (t_dial *x, t_float f)
{
    f = PD_CLAMP (f, x->x_minimum, x->x_maximum);
    
    if (x->x_isLogarithmic) { 
        x->x_position = (int)((log (f / x->x_minimum) / dial_getStepValue (x)) + 0.5);
    } else {
        x->x_position = (int)(((f - x->x_minimum) / dial_getStepValue (x)) + 0.5);
    }
    
    x->x_float = dial_getValue (x);
}

static void dial_steps (t_dial *x, t_float f)
{
    x->x_interval    = PD_CLAMP ((int)f, 1, DIAL_STEPS_MAXIMUM);
    x->x_position = PD_MIN (x->x_position, x->x_interval);
    
    x->x_float = dial_getValue (x);
}

static void dial_logarithmic (t_dial *x)
{
    x->x_isLogarithmic = 1;
    
    dial_setRange (x, x->x_minimum, x->x_maximum);
    
    x->x_float = dial_getValue (x);
}

static void dial_linear (t_dial *x)
{
    x->x_isLogarithmic = 0;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void dial_functionSave (t_object *z, t_buffer *b, int flags)
{
    t_dial *x = (t_dial *)z;
    
    buffer_appendSymbol (b, sym___hash__X);
    buffer_appendSymbol (b, sym_obj);
    buffer_appendFloat (b,  object_getX (z));
    buffer_appendFloat (b,  object_getY (z));
    buffer_appendSymbol (b, sym_dial);
    buffer_appendFloat (b,  x->x_digitsNumber);
    buffer_appendFloat (b,  x->x_size);
    buffer_appendFloat (b,  x->x_minimum);
    buffer_appendFloat (b,  x->x_maximum);
    buffer_appendFloat (b,  x->x_isLogarithmic);
    buffer_appendFloat (b,  x->x_interval);
    if (SAVED_DEEP (flags)) { buffer_appendFloat (b, x->x_float); }
    buffer_appendSemicolon (b);
    
    object_saveIdentifiers (z, b, flags);
}

static void dial_restore (t_dial *x)
{
    t_dial *old = (t_dial *)instance_pendingFetch (cast_object (x));
    
    if (old) { }    /* ??? */
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void *dial_new (t_symbol *s, int argc, t_atom *argv)
{
    t_dial *x = (t_dial *)pd_new (dial_class);
    
    int digits          = (argc > 5) ? (int)atom_getFloat (argv + 0) : DIAL_DIGITS_DEFAULT;
    int size            = (argc > 5) ? (int)atom_getFloat (argv + 1) : DIAL_SIZE_DEFAULT;
    t_float minimum     = (argc > 5) ? atom_getFloat (argv + 2) : DIAL_MINIMUM_DEFAULT;
    t_float maximum     = (argc > 5) ? atom_getFloat (argv + 3) : DIAL_MAXIMUM_DEFAULT;
    int isLogarithmic   = (argc > 5) ? (int)atom_getFloat (argv + 4) : 0;
    int steps           = (argc > 5) ? (int)atom_getFloat (argv + 5) : DIAL_INTERVAL_DEFAULT;
    t_float value       = (argc > 6) ? atom_getFloat (argv + 6) : DIAL_MINIMUM_DEFAULT;
    
    x->x_size           = PD_MAX (size, DIAL_SIZE_MINIMUM);
    x->x_isLogarithmic  = (isLogarithmic != 0);
    x->x_digitsNumber   = PD_CLAMP (digits, 1, DIAL_DIGITS_MAXIMUM);
    x->x_interval          = PD_CLAMP (steps, 1, DIAL_STEPS_MAXIMUM);
    x->x_outlet         = outlet_newFloat (cast_object (x));

    dial_setRange (x, minimum, maximum);
    dial_set (x, value);

    return x;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void dial_setup (void)
{
    t_class *c = NULL;
    
    c = class_new (sym_dial,
            (t_newmethod)dial_new,
            NULL,
            sizeof (t_dial),
            CLASS_DEFAULT,
            A_GIMME,
            A_NULL);
    
    class_addBang (c, (t_method)dial_bang);
    class_addFloat (c, (t_method)dial_float);
    class_addList (c, (t_method)dial_list);
    
    class_addMethod (c, (t_method)dial_size,        sym_size,           A_GIMME, A_NULL);
    class_addMethod (c, (t_method)dial_range,       sym_range,          A_GIMME, A_NULL);
    class_addMethod (c, (t_method)dial_set,         sym_set,            A_FLOAT, A_NULL);
    class_addMethod (c, (t_method)dial_steps,       sym_steps,          A_FLOAT, A_NULL);
    class_addMethod (c, (t_method)dial_logarithmic, sym_logarithmic,    A_NULL);
    class_addMethod (c, (t_method)dial_linear,      sym_linear,         A_NULL);
    class_addMethod (c, (t_method)dial_restore,     sym__restore,       A_NULL);

    class_setSaveFunction (c, dial_functionSave);
    class_setDataFunction (c, object_functionData);
    class_requirePending (c);
    
    dial_class = c;
}

PD_LOCAL void dial_destroy (void)
{
    class_free (dial_class);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
