
/* Copyright (c) 1997-2020 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#include "../../m_spaghettis.h"
#include "../../m_core.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#define RADIO_SIZE_DEFAULT              15
#define RADIO_SIZE_MINIMUM              8
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
    int             x_size;
    int             x_numberOfButtons;
    int64_t         x_state;
    t_float         x_floatValue;
    t_symbol        *x_mode;
    t_glist         *x_owner;
    t_outlet        *x_outlet;
    } t_radio;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void radio_setState (t_radio *x, int64_t n)
{
    if (x->x_mode == sym_multiple) {
        x->x_state = n & (((int64_t)1 << x->x_numberOfButtons) - 1);
    } else {
        x->x_state = PD_CLAMP (n, 0, (x->x_numberOfButtons - 1));
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void radio_bang (t_radio *x)
{
    outlet_float (x->x_outlet, x->x_floatValue);
}

static void radio_float (t_radio *x, t_float f)
{
    radio_setState (x, (int64_t)f); x->x_floatValue = f; radio_bang (x);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void radio_size (t_radio *x, t_symbol *s, int argc, t_atom *argv)
{
    if (argc) { int n = atom_getFloatAtIndex (0, argc, argv); x->x_size = PD_MAX (n, RADIO_SIZE_MINIMUM); }
}

static void radio_set (t_radio *x, t_float f)
{
    radio_setState (x, (int64_t)f); x->x_floatValue = f;
}

static void radio_buttonsNumber (t_radio *x, t_float numberOfButtons)
{
    int n = PD_CLAMP ((int)numberOfButtons, 1, RADIO_BUTTONS_MAXIMUM);

    if (n != x->x_numberOfButtons) { x->x_numberOfButtons = numberOfButtons; radio_setState (x, x->x_state); }
}

static void radio_mode (t_radio *x, t_symbol *s)
{
    t_symbol *old = x->x_mode;
    
    x->x_mode = (s == sym_multiple) ? sym_multiple : sym_single;
    
    if (old != x->x_mode) { radio_setState (x, x->x_state); glist_setDirty (x->x_owner, 1); }
}

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
    buffer_appendSymbol (b, x->x_mode);
    buffer_appendFloat (b,  x->x_numberOfButtons);
    if (SAVED_DEEP (flags)) { buffer_appendFloat (b, x->x_floatValue); }
    buffer_appendSemicolon (b);
    
    object_saveIdentifiers (z, b, flags);
}

static void radio_restore (t_radio *x)
{
    t_radio *old = (t_radio *)instance_pendingFetch (cast_object (x));
    
    if (old) { } // -- ???
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void *radio_new (t_symbol *s, int argc, t_atom *argv)
{
    t_radio *x = (t_radio *)pd_new (radio_class);
    
    int size            = (argc > 2) ? (int)atom_getFloat (argv + 0) : RADIO_SIZE_DEFAULT;
    t_symbol *mode      = (argc > 2) ? atom_getSymbol (argv + 1) : NULL;
    int numberOfButtons = (argc > 2) ? (int)atom_getFloat (argv + 2) : RADIO_BUTTONS_DEFAULT;
    t_float floatValue  = (argc > 3) ? atom_getFloat (argv + 3) : 0.0;

    x->x_size               = PD_MAX (size, RADIO_SIZE_MINIMUM);
    x->x_numberOfButtons    = PD_CLAMP (numberOfButtons, 1, RADIO_BUTTONS_MAXIMUM);
    x->x_floatValue         = floatValue;
    x->x_mode               = (mode == sym_multiple) ? sym_multiple : sym_single;
    x->x_owner              = instance_contextGetCurrent();
    x->x_outlet             = outlet_newFloat (cast_object (x));

    if (s == sym_vradio) { x->x_isVertical = 1; }
        
    radio_setState (x, (int64_t)(x->x_floatValue));
    
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
    
    class_addMethod (c, (t_method)radio_size,           sym_size,           A_GIMME, A_NULL);
    class_addMethod (c, (t_method)radio_set,            sym_set,            A_FLOAT, A_NULL);
    class_addMethod (c, (t_method)radio_buttonsNumber,  sym_buttonsnumber,  A_FLOAT, A_NULL);
    class_addMethod (c, (t_method)radio_mode,           sym_mode,           A_DEFSYMBOL, A_NULL);
    class_addMethod (c, (t_method)radio_restore,        sym__restore,       A_NULL);

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
