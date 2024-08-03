
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#include "../../m_headers.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

/* Hard coded values and workarounds. */
/* The core engine as no clue about graphic rendering. */
/* Sometimes it is required (e.g. encapsulation). */
/* Roughly make an estimation here. */

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
    
    if (c == vinlet_class)  { return gui_getInletWidth(); }
    if (c == voutlet_class) { return gui_getInletWidth(); }
    if (c == gatom_class)   { return gui_getDigits (cast_gui (x)) * FAKE_FONT; }
    if (c == garray_class)  { return gui_getWidth (cast_gui (x)); }
    
    return FAKE_FONT * buffer_getSize (object_getBuffer (x));
}

static int gui_getBoundingBoxHeight (t_object *x)
{
    t_class *c = pd_class (x);
    
    if (c == gatom_class)  { }
    if (c == garray_class) { return gui_getHeight (cast_gui (x)); }
    
    return FAKE_FONT;
}

t_rectangle gui_getBoundingBox (t_object *x)
{
    int a = object_getX (x);
    int b = object_getY (x);
    int c = a + gui_getBoundingBoxWidth (x);
    int d = b + gui_getBoundingBoxHeight (x);
        
    t_rectangle r;
    
    rectangle_set (&r, a, b, c, d);

    return r;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
