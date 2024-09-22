
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

void    rectangle_set               (t_rectangle *r, int a, int b, int c, int d);
void    rectangle_setNothing        (t_rectangle *r);
void    rectangle_setCopy           (t_rectangle *r, t_rectangle *toCopy);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#if defined ( PD_BUILDING_APPLICATION )

void    rectangle_setCopy           (t_rectangle *r, const juce::Rectangle<int>& toCopy);

#endif

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

int     rectangle_isNothing         (t_rectangle *r);
int     rectangle_areEquals         (t_rectangle *r1, t_rectangle *r2);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void    rectangle_addRectangle      (t_rectangle *r, t_rectangle *toAdd);
void    rectangle_addPoint          (t_rectangle *r, t_point *pt);

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

static inline t_point rectangle_getCentre (t_rectangle *r)
{
    return point_make (rectangle_getMiddleX (r), rectangle_getMiddleY (r));
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
