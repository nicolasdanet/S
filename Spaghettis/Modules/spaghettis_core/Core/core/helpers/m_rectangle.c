
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "../../m_spaghettis.h"
#include "../../m_core.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void rectangle_set (t_rectangle *r, int a, int b, int c, int d)
{
    r->rect_topLeftX     = PD_MIN (a, c);
    r->rect_topLeftY     = PD_MIN (b, d);
    r->rect_bottomRightX = PD_MAX (a, c);
    r->rect_bottomRightY = PD_MAX (b, d);
    r->rect_isNothing    = 0;
}

PD_LOCAL void rectangle_setNothing (t_rectangle *r)
{
    rectangle_set (r, 0, 0, 0, 0); r->rect_isNothing = 1;
}

PD_LOCAL void rectangle_setCopy (t_rectangle *r, t_rectangle *toCopy)
{
    r->rect_topLeftX     = toCopy->rect_topLeftX;
    r->rect_topLeftY     = toCopy->rect_topLeftY;
    r->rect_bottomRightX = toCopy->rect_bottomRightX;
    r->rect_bottomRightY = toCopy->rect_bottomRightY;
    r->rect_isNothing    = toCopy->rect_isNothing;
}

#if defined ( PD_BUILDING_APPLICATION )

PD_LOCAL void rectangle_setCopy (t_rectangle *r, const juce::Rectangle<int>& toCopy)
{
    rectangle_set (r,
        toCopy.getTopLeft().getX(),
        toCopy.getTopLeft().getY(),
        toCopy.getBottomRight().getX(),
        toCopy.getBottomRight().getY());
}

#endif

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL int rectangle_areEquals (t_rectangle *r1, t_rectangle *r2)
{
    if (r1->rect_isNothing         != r2->rect_isNothing)    { return 0; }
    else if (r1->rect_topLeftX     != r2->rect_topLeftX)     { return 0; }
    else if (r1->rect_topLeftY     != r2->rect_topLeftY)     { return 0; }
    else if (r1->rect_bottomRightX != r2->rect_bottomRightX) { return 0; }
    else if (r1->rect_bottomRightY != r2->rect_bottomRightY) { return 0; }
    
    return 1;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL int rectangle_isNothing (t_rectangle *r)
{
    t_rectangle t; rectangle_setNothing (&t);
    
    return rectangle_areEquals (r, &t);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void rectangle_addRectangle (t_rectangle *r, t_rectangle *toAdd)
{
    if (!rectangle_isNothing (toAdd)) { 

        if (rectangle_isNothing (r)) { rectangle_setCopy (r, toAdd); }
        else {
            r->rect_topLeftX     = PD_MIN (r->rect_topLeftX,     toAdd->rect_topLeftX);
            r->rect_topLeftY     = PD_MIN (r->rect_topLeftY,     toAdd->rect_topLeftY);
            r->rect_bottomRightX = PD_MAX (r->rect_bottomRightX, toAdd->rect_bottomRightX);
            r->rect_bottomRightY = PD_MAX (r->rect_bottomRightY, toAdd->rect_bottomRightY);
        }
    }
}

#if defined ( PD_BUILDING_APPLICATION )

PD_LOCAL void rectangle_addPoint (t_rectangle *r, t_point *pt)
{
    rectangle_add (r, point_getX (pt), point_getY (pt));
}

PD_LOCAL void rectangle_add (t_rectangle *r, int x, int y)
{
    t_rectangle t; rectangle_set (&t, x, y, x, y);
    
    rectangle_addRectangle (r, &t);
}

#endif

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
