
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

#define GUI_SIZE_MINIMUM           8
#define GUI_SIZE_MAXIMUM           256

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#define GUI_DIGITS_MINIMUM          0
#define GUI_DIGITS_MAXIMUM          64

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#define GUI_SIZE_DEFAULT            18
#define GUI_MINIMUM_DEFAULT         0
#define GUI_MAXIMUM_DEFAULT         127
#define GUI_INTERVAL_DEFAULT        1
#define GUI_LOGARITHMIC_DEFAULT     0
#define GUI_DIGITS_DEFAULT          5

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void gui_updateWidth        (t_gui *x, int width, int notify);
void gui_updateHeight       (t_gui *x, int height, int notify);
void gui_updateOrientation  (t_gui *x, int isVertical, int notify);

int  gui_updateValue        (t_gui *x, t_float f, int notify);
void gui_updateRange        (t_gui *x, t_float minimum, t_float maximum, int notify);
void gui_updateInterval     (t_gui *x, t_float interval, int notify);
void gui_updateLogarithmic  (t_gui *x, int isLogarithmic, int notify);
void gui_updateDigits       (t_gui *x, int digits, int notify);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#if defined ( PD_BUILDING_APPLICATION )

void gui_getValueParameters (t_object *o, core::Group& group, const Tags& t);
void gui_getSizeParameters (t_object *o, core::Group& group, const Tags& t);

void gui_setValueParameters (t_object *o, core::Group& group);
void gui_setSizeParameters (t_object *o, core::Group& group);

#endif

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#endif // G_UPDATE_H_