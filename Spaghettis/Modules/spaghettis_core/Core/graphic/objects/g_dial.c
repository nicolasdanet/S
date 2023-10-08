
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
#define DIAL_DIGITS_MINIMUM             0
#define DIAL_DIGITS_MAXIMUM             64
#define DIAL_INTERVAL_DEFAULT           1
#define DIAL_SIZE_DEFAULT               40
#define DIAL_SIZE_MINIMUM               8
#define DIAL_SIZE_MAXIMUM               256
#define DIAL_MINIMUM_DEFAULT            0
#define DIAL_MAXIMUM_DEFAULT            127
       
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static t_class *dial_class;             /* Shared. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

typedef struct _dial {
    t_object        x_obj;              /* MUST be the first. */
    int             x_size;
    int             x_isLogarithmic;
    int             x_digits;
    int             x_interval;
    t_float         x_minimum;
    t_float         x_maximum;
    t_float         x_value;
    t_outlet        *x_outlet;
    } t_dial;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void dial_updateSize (t_dial *x, int size, int notify)
{
    int n = PD_CLAMP (size, DIAL_SIZE_MINIMUM, DIAL_SIZE_MAXIMUM);
    
    if (x->x_size != n) {
    //
    x->x_size = n;
    
    if (notify) {
        #if defined ( PD_BUILDING_APPLICATION )
        outputs_objectUpdated (cast_object (x), Tags::parameters (Tag::Width));
        #endif
    }
    //
    }
}

static void dial_updateLogarithmic (t_dial *x, int isLogarithmic, int notify)
{
    if (x->x_isLogarithmic != isLogarithmic) {
    //
    x->x_isLogarithmic = isLogarithmic;
    
    if (notify) {
        #if defined ( PD_BUILDING_APPLICATION )
        outputs_objectUpdated (cast_object (x), Tags::parameters (Tag::Logarithmic));
        #endif
    }
    //
    }
}

static void dial_updateDigits (t_dial *x, int digits, int notify)
{
    int n = PD_CLAMP (digits, DIAL_DIGITS_MINIMUM, DIAL_DIGITS_MAXIMUM);
    
    if (x->x_digits != n) {
    //
    x->x_digits = n;
    
    if (notify) {
        #if defined ( PD_BUILDING_APPLICATION )
        outputs_objectUpdated (cast_object (x), Tags::parameters (Tag::Digits));
        #endif
    }
    //
    }
}

static void dial_updateRange (t_dial *x, t_float minimum, t_float maximum, int notify)
{
    t_float min = x->x_minimum;
    t_float max = x->x_maximum;

    x->x_minimum = PD_MIN (minimum, maximum);
    x->x_maximum = PD_MAX (minimum, maximum);
    
    if (notify) {
        if (min != x->x_minimum) {
            #if defined ( PD_BUILDING_APPLICATION )
            outputs_objectUpdated (cast_object (x), Tags::parameters (Tag::Low));
            #endif
        }
        if (max != x->x_maximum) {
            #if defined ( PD_BUILDING_APPLICATION )
            outputs_objectUpdated (cast_object (x), Tags::parameters (Tag::High));
            #endif
        }
    }
}

static void dial_updateInterval (t_dial *x, t_float interval, int notify)
{
    t_float step = PD_MAX (0.0, interval);
    
    if (x->x_interval != step) {
    //
    x->x_interval = step;
    
    if (notify) {
        #if defined ( PD_BUILDING_APPLICATION )
        outputs_objectChanged (cast_object (x), Tags::parameters (Tag::Interval));
        #endif
    }
    //
    }
}

static int dial_updateValue (t_dial *x, t_float f, int notify)
{
    if (x->x_value != f) {
    //
    x->x_value = f;
    
    if (notify) {
        #if defined ( PD_BUILDING_APPLICATION )
        outputs_objectChanged (cast_object (x), Tags::parameters (Tag::Value));
        #endif
    }
    
    return 1;
    //
    }
    
    return 0;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void dial_bang (t_dial *x)
{
    outlet_float (x->x_outlet, x->x_value);
}

static void dial_float (t_dial *x, t_float f)
{
    dial_updateValue (x, f, 1); dial_bang (x);
}

static void dial_list (t_dial *x, t_symbol *s, int argc, t_atom *argv)
{
    if (argc && IS_FLOAT (argv)) { dial_float (x, atom_getFloat (argv)); }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

/* Kept compatible with vanilla nbx object. */

static void dial_size (t_dial *x, t_symbol *s, int argc, t_atom *argv)
{
    int digits = (int)atom_getFloatAtIndex (0, argc, argv);
    int size   = (int)atom_getFloatAtIndex (1, argc, argv);

    if (argc > 0) { dial_updateDigits (x, digits, 1); }
    if (argc > 1) { dial_updateSize (x, size, 1); }
}

static void dial_range (t_dial *x, t_symbol *s, int argc, t_atom *argv)
{
    t_float minimum = atom_getFloatAtIndex (0, argc, argv);
    t_float maximum = atom_getFloatAtIndex (1, argc, argv);
    
    dial_updateRange (x, minimum, maximum, 1);
}

static void dial_set (t_dial *x, t_float f)
{
    dial_updateValue (x, f, 1);
}

static void dial_logarithmic (t_dial *x)
{
    dial_updateLogarithmic (x, 1, 1);
}

static void dial_linear (t_dial *x)
{
    dial_updateLogarithmic (x, 0, 1);
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
    buffer_appendFloat (b,  x->x_digits);
    buffer_appendFloat (b,  x->x_size);
    buffer_appendFloat (b,  x->x_minimum);
    buffer_appendFloat (b,  x->x_maximum);
    buffer_appendFloat (b,  x->x_isLogarithmic);
    buffer_appendFloat (b,  x->x_interval);
    if (SAVED_DEEP (flags)) { buffer_appendFloat (b, x->x_value); }
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

#if defined ( PD_BUILDING_APPLICATION )

static void dial_functionGetParameters (t_object *o, core::Group& group, const Tags& t)
{
    t_dial *x = (t_dial *)o;
    
    static DelegateCache delegate;
    
    if (t.contains (Tag::Value)) {
        group.addParameter (Tag::Value,
            NEEDS_TRANS ("Value"),
            NEEDS_TRANS ("Value of dial"),
            x->x_value,
            delegate);
    }
    
    if (t.contains (Tag::Low)) {
        group.addParameter (Tag::Low,
            NEEDS_TRANS ("Low Range"),
            NEEDS_TRANS ("Low value"),
            x->x_minimum,
            delegate);
    }
    
    if (t.contains (Tag::High)) {
        group.addParameter (Tag::High,
            NEEDS_TRANS ("High Range"),
            NEEDS_TRANS ("High value"),
            x->x_maximum,
            delegate);
    }
    
    if (t.contains (Tag::Interval)) {
        group.addParameter (Tag::Interval,
            NEEDS_TRANS ("Interval"),
            NEEDS_TRANS ("Step between dial values"),
            x->x_interval,
            delegate).setPositive<t_float>();
    }
    
    if (t.contains (Tag::Digits)) {
        group.addParameter (Tag::Digits,
            NEEDS_TRANS ("Digits"),
            NEEDS_TRANS ("Number of digits"),
            x->x_digits,
            delegate).setRange (juce::Range<int> (DIAL_DIGITS_MINIMUM, DIAL_DIGITS_MAXIMUM));
    }
    
    if (t.contains (Tag::Logarithmic)) {
        group.addParameter (Tag::Logarithmic,
            NEEDS_TRANS ("Logarithmic"),
            NEEDS_TRANS ("Scale is logarithmic"),
            static_cast<bool> (x->x_isLogarithmic),
            delegate);
    }
    
    if (t.contains (Tag::Width)) {
        group.addParameter (Tag::Width,
            NEEDS_TRANS ("Width"),
            NEEDS_TRANS ("Border size of dial"),
            x->x_size,
            delegate).setRange (juce::Range<int> (DIAL_SIZE_MINIMUM, DIAL_SIZE_MAXIMUM));
    }
}

static void dial_functionSetParameters (t_object *o, const core::Group& group)
{
    t_dial *x = (t_dial *)o;
    
    jassert (group.hasParameter (Tag::Value));
    jassert (group.hasParameter (Tag::Low));
    jassert (group.hasParameter (Tag::High));
    jassert (group.hasParameter (Tag::Interval));
    jassert (group.hasParameter (Tag::Digits));
    jassert (group.hasParameter (Tag::Logarithmic));
    jassert (group.hasParameter (Tag::Width));
    
    const t_float f         = group.getParameter (Tag::Value).getValueTyped<t_float>();
    const t_float min       = group.getParameter (Tag::Low).getValueTyped<t_float>();
    const t_float max       = group.getParameter (Tag::High).getValueTyped<t_float>();
    const t_float step      = group.getParameter (Tag::Interval).getValueTyped<t_float>();
    const bool logarithmic  = group.getParameter (Tag::Logarithmic).getValueTyped<bool>();
    const int digits        = group.getParameter (Tag::Digits).getValueTyped<int>();
    const int size          = group.getParameter (Tag::Width).getValueTyped<int>();
    
    dial_updateRange (x, min, max, 1);
    dial_updateInterval (x, step, 1);
    dial_updateDigits (x, digits, 1);
    dial_updateLogarithmic (x, logarithmic, 1);
    dial_updateSize (x, size, 1);
    
    if (dial_updateValue (x, f, 1)) { dial_bang (x); }
}

#endif

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void *dial_new (t_symbol *s, int argc, t_atom *argv)
{
    t_dial *x = (t_dial *)pd_new (dial_class);
    
    int digits          = (argc > 5) ? (int)atom_getFloat (argv + 0) : DIAL_DIGITS_DEFAULT;
    int size            = (argc > 5) ? (int)atom_getFloat (argv + 1) : DIAL_SIZE_DEFAULT;
    t_float minimum     = (argc > 5) ? atom_getFloat (argv + 2)      : DIAL_MINIMUM_DEFAULT;
    t_float maximum     = (argc > 5) ? atom_getFloat (argv + 3)      : DIAL_MAXIMUM_DEFAULT;
    int isLogarithmic   = (argc > 5) ? (int)atom_getFloat (argv + 4) : 0;
    int step            = (argc > 5) ? (int)atom_getFloat (argv + 5) : DIAL_INTERVAL_DEFAULT;
    t_float value       = (argc > 6) ? atom_getFloat (argv + 6)      : DIAL_MINIMUM_DEFAULT;
    
    dial_updateDigits (x, digits, 0);
    dial_updateSize (x, size, 0);
    dial_updateRange (x, minimum, maximum, 0);
    dial_updateLogarithmic (x, (isLogarithmic != 0), 0);
    dial_updateInterval (x, step, 0);
    dial_updateValue (x, value, 0);
    
    x->x_outlet = outlet_newFloat (cast_object (x));

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
    class_addMethod (c, (t_method)dial_logarithmic, sym_logarithmic,    A_NULL);
    class_addMethod (c, (t_method)dial_linear,      sym_linear,         A_NULL);
    class_addMethod (c, (t_method)dial_restore,     sym__restore,       A_NULL);

    #if defined ( PD_BUILDING_APPLICATION )
    
    class_setParametersFunctions (c, dial_functionGetParameters, dial_functionSetParameters);
    
    #endif
    
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
