
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#ifndef G_UPDATE_H_
#define G_UPDATE_H_

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

typedef struct _gui {
    t_object    x_obj;                  /* MUST be the first. */
    int         x_width;
    int         x_height;
    int         x_digits;
    int         x_isVertical;
    int         x_isLogarithmic;
    t_float     x_interval;
    t_float     x_minimum;
    t_float     x_maximum;
    t_float     x_value;
    } t_gui;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#define cast_gui(x) ((t_gui *)(x))

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

inline int gui_getWidth (t_gui *x)
{
    return x->x_width;
}

inline int gui_getHeight (t_gui *x)
{
    return x->x_height;
}

inline int gui_getDigits (t_gui *x)
{
    return x->x_digits;
}

inline int gui_isVertical (t_gui *x)
{
    return x->x_isVertical;
}

inline int gui_isLogarithmic (t_gui *x)
{
    return x->x_isLogarithmic;
}

inline t_float gui_getInterval (t_gui *x)
{
    return x->x_interval;
}

inline t_float gui_getMinimum (t_gui *x)
{
    return x->x_minimum;
}

inline t_float gui_getMaximum (t_gui *x)
{
    return x->x_maximum;
}

inline t_float gui_getValue (t_gui *x)
{
    return x->x_value;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#define GUI_WIDTH_MINIMUM           8
#define GUI_WIDTH_MAXIMUM           256
#define GUI_WIDTH_DEFAULT           18

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#define GUI_DIGITS_MINIMUM          0
#define GUI_DIGITS_MAXIMUM          64
#define GUI_DIGITS_DEFAULT          5

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#define GUI_MINIMUM_DEFAULT         0
#define GUI_MAXIMUM_DEFAULT         127

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#define GUI_INTERVAL_DEFAULT        1

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void update_width           (t_gui *x, int width, int notify);
void update_height          (t_gui *x, int height, int notify);
void update_digits          (t_gui *x, int digits, int notify);
void update_orientation     (t_gui *x, int isVertical, int notify);
void update_logarithmic     (t_gui *x, int isLogarithmic, int notify);
void update_range           (t_gui *x, t_float minimum, t_float maximum, int notify);
void update_interval        (t_gui *x, t_float interval, int notify);
int  update_value           (t_gui *x, t_float f, int notify);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#endif // G_UPDATE_H_
