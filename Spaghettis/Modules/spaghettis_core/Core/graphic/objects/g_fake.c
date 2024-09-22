
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#include "../../m_headers.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

/* Hard coded values and workarounds. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#define FAKE_FONT   16

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

int gui_getInletWidth()
{
    return FAKE_FONT * 3;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static int gui_getBoundingBoxWidth (t_object *x)
{
    t_class *c = pd_class (x);
    
    if (c == garray_class   ||
        c == bng_class      ||
        c == toggle_class   ||
        c == slider_class   ||
        c == dial_class     ||
        c == vu_class)
        {
            return gui_getWidth (cast_gui (x));
        }
    
    if (c == vinlet_class || c == voutlet_class) { return gui_getInletWidth(); }

    if (c == gatom_class)   { return gui_getDigits (cast_gui (x)) * FAKE_FONT; }
    if (c == radio_class)   {
        t_gui *y = cast_gui (x);
        if (gui_isVertical (y)) { return gui_getWidth (y); }
        else {
            return gui_getWidth (y) * gui_getButtons (y);
        }
    }
    
    return FAKE_FONT * buffer_getSize (object_getBuffer (x));
}

static int gui_getBoundingBoxHeight (t_object *x)
{
    t_class *c = pd_class (x);
    
    if (c == garray_class || c == slider_class || c == vu_class)    { return gui_getHeight (cast_gui (x)); }
    if (c == bng_class    || c == toggle_class || c == dial_class)  { return gui_getWidth (cast_gui (x));  }
    
    if (c == radio_class)  {
        t_gui *y = cast_gui (x);
        if (gui_isVertical (y)) { return gui_getWidth (y) * gui_getButtons (y); }
        else {
            return gui_getWidth (y);
        }
    }
    
    return FAKE_FONT;
}

/* The core engine is agnostic about graphic rendering. */
/* Sometimes it is required (e.g. encapsulation). */
/* Roughly make an estimation here. */
/* Not the best design by the way! */

t_rectangle gui_getBoundingBox (t_object *x)
{
    int a = object_getX (x);
    int b = object_getY (x);
    int w = gui_getBoundingBoxWidth (x);
    int h = gui_getBoundingBoxHeight (x);
    
    t_rectangle r;
    
    rectangle_set (&r, a, b, a + w, b + h);

    return r;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
