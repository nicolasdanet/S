
/* Copyright (c) 1997-2020 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#include "../../m_spaghettis.h"
#include "../../m_core.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#define VU_WIDTH_DEFAULT        15
#define VU_WIDTH_MINIMUM        8
#define VU_HEIGHT_DEFAULT       120
#define VU_HEIGHT_MINIMUM       40

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static t_class *vu_class;       /* Shared. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

typedef struct _vu {
    t_object    x_obj;          /* MUST be the first. */
    int         x_width;
    int         x_height;
    t_float     x_peakValue;
    t_float     x_decibelValue;
    t_outlet    *x_outletLeft;
    t_outlet    *x_outletRight;
    } t_vu;
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void vu_bang (t_vu *x)
{
    outlet_float (x->x_outletRight, x->x_peakValue);
    outlet_float (x->x_outletLeft, x->x_decibelValue);
}

static void vu_floatPeak (t_vu *x, t_float peak)
{
    x->x_peakValue = peak; outlet_float (x->x_outletRight, peak);
}

static void vu_float (t_vu *x, t_float decibel)
{
    x->x_decibelValue = decibel;
    
    if (x->x_decibelValue > x->x_peakValue) { vu_floatPeak (x, decibel); }
    
    outlet_float (x->x_outletLeft, decibel);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void vu_size (t_vu *x, t_symbol *s, int argc, t_atom *argv)
{
    if (argc) {
    //
    int n = atom_getFloatAtIndex (0, argc, argv); x->x_width = PD_MAX (n, VU_WIDTH_MINIMUM);
    //
    }
    
    if (argc > 1) {
    //
    int n = atom_getFloatAtIndex (1, argc, argv); x->x_height = PD_MAX (n, VU_HEIGHT_MINIMUM);
    //
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void vu_functionSave (t_object *z, t_buffer *b, int flags)
{
    t_vu *x = (t_vu *)z;
    
    buffer_appendSymbol (b, sym___hash__X);
    buffer_appendSymbol (b, sym_obj);
    buffer_appendFloat (b,  object_getX (cast_object (x)));
    buffer_appendFloat (b,  object_getY (cast_object (x)));
    buffer_appendSymbol (b, sym_vu);
    buffer_appendFloat (b,  x->x_width);
    buffer_appendFloat (b,  x->x_height);
    buffer_appendSemicolon (b);
    
    object_saveIdentifiers (z, b, flags);
}

static void vu_restore (t_vu *x)
{
    t_vu *old = (t_vu *)instance_pendingFetch (cast_object (x));
    
    if (old) { }    // -- ???
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void *vu_new (t_symbol *s, int argc, t_atom *argv)
{
    t_vu *x = (t_vu *)pd_new (vu_class);

    int width   = (argc > 1) ? (int)atom_getFloat (argv + 0) : VU_WIDTH_DEFAULT;
    int height  = (argc > 1) ? (int)atom_getFloat (argv + 1) : VU_HEIGHT_DEFAULT;

    x->x_width  = PD_MAX (width, VU_WIDTH_MINIMUM);
    x->x_height = PD_MAX (height, VU_HEIGHT_MINIMUM);
    
    inlet_new2 (x, &s_float);
    
    x->x_outletLeft  = outlet_newFloat (cast_object (x));
    x->x_outletRight = outlet_newFloat (cast_object (x));
    
    return x;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void vu_setup (void)
{
    t_class *c = NULL;
    
    c = class_new (sym_vu,
            (t_newmethod)vu_new,
            NULL,
            sizeof (t_vu),
            CLASS_DEFAULT,
            A_GIMME,
            A_NULL);
        
    class_addBang (c, (t_method)vu_bang);
    class_addFloat (c, (t_method)vu_float);
    
    class_addMethod (c, (t_method)vu_floatPeak, sym__inlet2,    A_FLOAT, A_NULL);
    class_addMethod (c, (t_method)vu_size,      sym_size,       A_GIMME, A_NULL);
    class_addMethod (c, (t_method)vu_restore,   sym__restore,   A_NULL);

    class_setSaveFunction (c, vu_functionSave);
    class_setDataFunction (c, object_functionData);
    class_requirePending (c);

    vu_class = c;
}

PD_LOCAL void vu_destroy (void)
{
    class_free (vu_class);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
