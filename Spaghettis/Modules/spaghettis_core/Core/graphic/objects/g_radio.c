
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#include "../../m_spaghettis.h"
#include "../../m_core.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#define RADIO_SIZE_DEFAULT              18
#define RADIO_SIZE_MINIMUM              8
#define RADIO_SIZE_MAXIMUM              256
#define RADIO_BUTTONS_DEFAULT           8
#define RADIO_BUTTONS_MAXIMUM           32

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

static t_class *radio_class;            /* Shared. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

typedef struct _radio {
    t_object        x_obj;              /* MUST be the first. */
    int             x_isVertical;
    int             x_isMultiple;
    int             x_numberOfButtons;
    int             x_size;
    t_float         x_value;
    t_outlet        *x_outlet;
    } t_radio;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void radio_updateValue (t_radio *, t_float, int);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void radio_bang (t_radio *x)
{
    outlet_float (x->x_outlet, x->x_value);
}

static void radio_float (t_radio *x, t_float f)
{
    radio_updateValue (x, f, 1); radio_bang (x);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void radio_updateValue (t_radio *x, t_float f, int notify)
{
    if (x->x_value != f) {
    //
    x->x_value = f;
    
    if (notify) {
        #if defined ( PD_BUILDING_APPLICATION )
        outputs_objectChanged (cast_object (x), Tags::parameters (Tag::Value));
        #endif
    }
    //
    }
}

static void radio_updateOrientation (t_radio *x, int isVertical, int notify)
{
    if (x->x_isVertical != isVertical) {
    //
    x->x_isVertical = isVertical;
    
    if (notify) {
        #if defined ( PD_BUILDING_APPLICATION )
        outputs_objectUpdated (cast_object (x), Tags::parameters (Tag::Vertical));
        #endif
    }
    //
    }
}

static void radio_updateMode (t_radio *x, int isMultiple, int notify)
{
    if (x->x_isMultiple != isMultiple) {
    //
    x->x_isMultiple = isMultiple;
    
    if (notify) {
        #if defined ( PD_BUILDING_APPLICATION )
        outputs_objectUpdated (cast_object (x), Tags::parameters (Tag::Multiple));
        #endif
    }
    //
    }
}

static void radio_updateButtons (t_radio *x, int numberOfButtons, int notify)
{
    int n = PD_CLAMP ((int)numberOfButtons, 1, RADIO_BUTTONS_MAXIMUM);

    if (x->x_numberOfButtons != n) {
    //
    x->x_numberOfButtons = n;
    
    if (notify) {
        #if defined ( PD_BUILDING_APPLICATION )
        outputs_objectUpdated (cast_object (x), Tags::parameters (Tag::Buttons));
        #endif
    }
    //
    }
}

static void radio_updateSize (t_radio *x, int n, int notify)
{
    n = PD_CLAMP (n, RADIO_SIZE_MINIMUM, RADIO_SIZE_MAXIMUM);
    
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

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void radio_set (t_radio *x, t_float f)
{
    radio_updateValue (x, f, 1);
}

static void radio_mode (t_radio *x, t_symbol *s)
{
    radio_updateMode (x, ((s == sym_multiple) ? 1 : 0), 1);
}

static void radio_buttonsNumber (t_radio *x, t_float f)
{
    radio_updateButtons (x, (int)f, 1);
}

static void radio_size (t_radio *x, t_symbol *s, int argc, t_atom *argv)
{
    if (argc) { radio_updateSize (x, (int)atom_getFloatAtIndex (0, argc, argv), 1); }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#if defined ( PD_BUILDING_APPLICATION )

static void radio_functionGetParameters (t_object *o, core::Group& group, const Tags& t)
{
    t_radio *x = (t_radio *)o;
    
    static DelegateCache delegate;
    
    if (t.contains (Tag::Value)) {
        group.addParameter (Tag::Value,
            NEEDS_TRANS ("Value"),
            NEEDS_TRANS ("Value of selector"),
            x->x_value,
            delegate);
    }
    
    if (t.contains (Tag::Vertical)) {
        group.addParameter (Tag::Vertical,
            NEEDS_TRANS ("Vertical"),
            NEEDS_TRANS ("Orientation is vertical"),
            static_cast<bool> (x->x_isVertical),
            delegate);
    }
    
    if (t.contains (Tag::Multiple)) {
        group.addParameter (Tag::Multiple,
            NEEDS_TRANS ("Multiple"),
            NEEDS_TRANS ("Set in multiple mode"),
            static_cast<bool> (x->x_isMultiple),
            delegate);
    }
    
    if (t.contains (Tag::Buttons)) {
        group.addParameter (Tag::Buttons,
            NEEDS_TRANS ("Buttons"),
            NEEDS_TRANS ("Number of buttons"),
            x->x_numberOfButtons,
            delegate).setRange (juce::Range<int> (1, RADIO_BUTTONS_MAXIMUM));
    }
    
    if (t.contains (Tag::Width)) {
        group.addParameter (Tag::Width,
            NEEDS_TRANS ("Width"),
            NEEDS_TRANS ("Border size of a button"),
            x->x_size,
            delegate).setRange (juce::Range<int> (RADIO_SIZE_MINIMUM, RADIO_SIZE_MAXIMUM));
    }
}

static void radio_functionSetParameters (t_object *o, const core::Group& group)
{
    t_radio *x = (t_radio *)o;
    
    jassert (group.hasParameter (Tag::Value));
    jassert (group.hasParameter (Tag::Vertical));
    jassert (group.hasParameter (Tag::Multiple));
    jassert (group.hasParameter (Tag::Buttons));
    jassert (group.hasParameter (Tag::Width));
}

#endif

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void radio_functionSave (t_object *z, t_buffer *b, int flags)
{
    t_radio *x = (t_radio *)z;
    
    buffer_appendSymbol (b, sym___hash__X);
    buffer_appendSymbol (b, sym_obj);
    buffer_appendFloat (b,  object_getX (z));
    buffer_appendFloat (b,  object_getY (z));
    buffer_appendSymbol (b, x->x_isVertical ? sym_vradio : sym_hradio);
    buffer_appendFloat (b,  x->x_size);
    buffer_appendFloat (b,  x->x_isMultiple);
    buffer_appendFloat (b,  x->x_numberOfButtons);
    if (SAVED_DEEP (flags)) { buffer_appendFloat (b, x->x_value); }
    buffer_appendSemicolon (b);
    
    object_saveIdentifiers (z, b, flags);
}

static void radio_restore (t_radio *x)
{
    t_radio *old = (t_radio *)instance_pendingFetch (cast_object (x));
    
    if (old) { } /* ??? */
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void *radio_new (t_symbol *s, int argc, t_atom *argv)
{
    t_radio *x = (t_radio *)pd_new (radio_class);
    
    int size            = (argc > 2) ? (int)atom_getFloat (argv + 0) : RADIO_SIZE_DEFAULT;
    int isMultiple      = (argc > 2) ? atom_getFloat (argv + 1) : 0.0;
    int numberOfButtons = (argc > 2) ? (int)atom_getFloat (argv + 2) : RADIO_BUTTONS_DEFAULT;
    t_float value       = (argc > 3) ? atom_getFloat (argv + 3) : 0.0;

    x->x_isMultiple         = isMultiple;
    x->x_numberOfButtons    = PD_CLAMP (numberOfButtons, 1, RADIO_BUTTONS_MAXIMUM);
    x->x_size               = PD_MAX (size, RADIO_SIZE_MINIMUM);
    x->x_outlet             = outlet_newFloat (cast_object (x));

    if (s == sym_vradio) { x->x_isVertical = 1; }
        
    radio_updateValue (x, value, 0);
    
    return x;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void radio_setup (void)
{
    t_class *c = NULL;
    
    c = class_new (sym_hradio, 
            (t_newmethod)radio_new,
            NULL,
            sizeof (t_radio),
            CLASS_DEFAULT,
            A_GIMME,
            A_NULL);
    
    class_addCreator ((t_newmethod)radio_new, sym_vradio, A_GIMME, A_NULL);
        
    class_addBang (c, (t_method)radio_bang);
    class_addFloat (c, (t_method)radio_float);
    
    class_addMethod (c, (t_method)radio_set,            sym_set,            A_FLOAT, A_NULL);
    class_addMethod (c, (t_method)radio_buttonsNumber,  sym_buttonsnumber,  A_FLOAT, A_NULL);
    class_addMethod (c, (t_method)radio_mode,           sym_mode,           A_DEFSYMBOL, A_NULL);
    class_addMethod (c, (t_method)radio_size,           sym_size,           A_GIMME, A_NULL);
    class_addMethod (c, (t_method)radio_restore,        sym__restore,       A_NULL);

    #if defined ( PD_BUILDING_APPLICATION )
    
    class_setParametersFunctions (c, radio_functionGetParameters, radio_functionSetParameters);
    
    #endif
    
    class_setHelpName (c, sym_radio);
    class_setSaveFunction (c, radio_functionSave);
    class_setDataFunction (c, object_functionData);
    class_requirePending (c);
    
    radio_class = c;
}

PD_LOCAL void radio_destroy (void)
{
    class_free (radio_class);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
