
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
    int             x_size;
    int             x_numberOfButtons;
    int64_t         x_state;
    t_float         x_floatValue;
    t_outlet        *x_outlet;
    } t_radio;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void radio_setState (t_radio *x, int64_t n)
{
    if (x->x_isMultiple) { x->x_state = n & (((int64_t)1 << x->x_numberOfButtons) - 1); }
    else {
        x->x_state = PD_CLAMP (n, 0, (x->x_numberOfButtons - 1));
    }
}

static void radio_setValue (t_radio *x, t_float f)
{
    radio_setState (x, (int64_t)f); x->x_floatValue = f;
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
    radio_setValue (x, f); radio_bang (x);
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
    radio_setValue (x, f);
}

static void radio_buttonsNumber (t_radio *x, t_float numberOfButtons)
{
    int n = PD_CLAMP ((int)numberOfButtons, 1, RADIO_BUTTONS_MAXIMUM);

    if (n != x->x_numberOfButtons) { x->x_numberOfButtons = numberOfButtons; radio_setState (x, x->x_state); }
}

static void radio_mode (t_radio *x, t_symbol *s)
{
    int t = (s == sym_multiple) ? 1 : 0;
    
    if (t != x->x_isMultiple) { x->x_isMultiple = t; radio_setState (x, x->x_state); }
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
    buffer_appendFloat (b,  x->x_isMultiple);
    buffer_appendFloat (b,  x->x_numberOfButtons);
    if (SAVED_DEEP (flags)) { buffer_appendFloat (b, x->x_floatValue); }
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

    x->x_size               = PD_MAX (size, RADIO_SIZE_MINIMUM);
    x->x_numberOfButtons    = PD_CLAMP (numberOfButtons, 1, RADIO_BUTTONS_MAXIMUM);
    x->x_isMultiple         = isMultiple;
    x->x_outlet             = outlet_newFloat (cast_object (x));

    if (s == sym_vradio) { x->x_isVertical = 1; }
        
    radio_setValue (x, value);
    
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
