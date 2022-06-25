
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#ifndef M_RECTANGLE_H_
#define M_RECTANGLE_H_

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

typedef struct _rectangle {
    int64_t rect_topLeftX;                  /* Use int64_t to avoid Wstrict-overflow warnings. */
    int64_t rect_topLeftY;
    int64_t rect_bottomRightX;
    int64_t rect_bottomRightY;
    int     rect_isNothing;
    } t_rectangle;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void   rectangle_set               (t_rectangle *r, int a, int b, int c, int d);
PD_LOCAL void   rectangle_setNothing        (t_rectangle *r);
PD_LOCAL void   rectangle_setCopy           (t_rectangle *r, t_rectangle *toCopy);

#if defined ( PD_BUILDING_APPLICATION )

PD_LOCAL void   rectangle_setCopy           (t_rectangle *r, const juce::Rectangle<int>& toCopy);

#endif

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL int    rectangle_isNothing         (t_rectangle *r);
PD_LOCAL int    rectangle_areEquals         (t_rectangle *r1, t_rectangle *r2);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void   rectangle_addPoint          (t_rectangle *r, t_point *pt);
PD_LOCAL void   rectangle_add               (t_rectangle *r, int x, int y);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static inline int rectangle_getTopLeftX (t_rectangle *r)
{
    return (int)r->rect_topLeftX;
}

static inline int rectangle_getTopLeftY (t_rectangle *r)
{
    return (int)r->rect_topLeftY;
}

static inline int rectangle_getBottomRightX (t_rectangle *r)
{
    return (int)r->rect_bottomRightX;
}

static inline int rectangle_getBottomRightY (t_rectangle *r)
{
    return (int)r->rect_bottomRightY;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static inline int rectangle_getMiddleX (t_rectangle *r)
{
    return (int)((r->rect_topLeftX + r->rect_bottomRightX) / 2);
}

static inline int rectangle_getMiddleY (t_rectangle *r)
{
    return (int)((r->rect_topLeftY + r->rect_bottomRightY) / 2);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static inline int rectangle_getWidth (t_rectangle *r)
{
    return (int)(r->rect_bottomRightX - r->rect_topLeftX);
}

static inline int rectangle_getHeight (t_rectangle *r)
{
    return (int)(r->rect_bottomRightY - r->rect_topLeftY);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static inline void rectangle_setWidth (t_rectangle *r, int width)
{
    r->rect_bottomRightX = r->rect_topLeftX + PD_ABS (width);
}

static inline void rectangle_setHeight (t_rectangle *r, int heigth)
{
    r->rect_bottomRightY = r->rect_topLeftY + PD_ABS (heigth);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
#endif // M_RECTANGLE_H_
