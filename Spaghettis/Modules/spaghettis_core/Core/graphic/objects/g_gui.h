
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
    int         x_buttons;
    int         x_isVertical;
    int         x_isLogarithmic;
    int         x_isMultiple;
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

inline int gui_getButtons (t_gui *x)
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

inline int gui_isMultiple (t_gui *x)
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

#define GUI_BUTTONS_MINIMUM         1
#define GUI_BUTTONS_MAXIMUM         32

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#define GUI_SIZE_DEFAULT            18
#define GUI_DIGITS_DEFAULT          5
#define GUI_BUTTONS_DEFAULT         8
#define GUI_LOGARITHMIC_DEFAULT     0
#define GUI_MULTIPLE_DEFAULT        0
#define GUI_MINIMUM_DEFAULT         0
#define GUI_MAXIMUM_DEFAULT         127
#define GUI_INTERVAL_DEFAULT        1

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL int  gui_updateValue               (t_gui *x, t_float f, int notify);
PD_LOCAL void gui_updateRange               (t_gui *x, t_float minimum, t_float maximum, int notify);
PD_LOCAL void gui_updateInterval            (t_gui *x, t_float interval, int notify);
PD_LOCAL void gui_updateLogarithmic         (t_gui *x, int isLogarithmic, int notify);
PD_LOCAL void gui_updateMultiple            (t_gui *x, int isMultiple, int notify);
PD_LOCAL void gui_updateDigits              (t_gui *x, int digits, int notify);
PD_LOCAL void gui_updateButtons             (t_gui *x, int buttons, int notify);

PD_LOCAL void gui_updateWidth               (t_gui *x, int width, int notify);
PD_LOCAL void gui_updateHeight              (t_gui *x, int height, int notify);
PD_LOCAL void gui_updateOrientation         (t_gui *x, int isVertical, int notify);
PD_LOCAL void gui_updateOrientationSwap     (t_gui *x, int isVertical, int notify);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#if defined ( PD_BUILDING_APPLICATION )

enum {
    GUI_NONE                = (1 << 0),
    GUI_VALUE               = (1 << 1),
    GUI_LOW                 = (1 << 2),
    GUI_HIGH                = (1 << 3),
    GUI_INTERVAL            = (1 << 4),
    GUI_LOGARITHMIC         = (1 << 5),
    GUI_MULTIPLE            = (1 << 6),
    GUI_DIGITS              = (1 << 7),
    GUI_BUTTONS             = (1 << 8),
    GUI_WIDTH               = (1 << 9),
    GUI_HEIGHT              = (1 << 10),
    GUI_ORIENTATION         = (1 << 11),
    GUI_SWAP                = (1 << 12)     // GUI_ORIENTATION
    };

#endif

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#if defined ( PD_BUILDING_APPLICATION )

PD_LOCAL void gui_getParameters (t_object *o, core::Group& group, const Tags& t, int flags);
PD_LOCAL void gui_setParameters (t_object *o, core::Group& group, int flags);

#endif

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#endif // G_UPDATE_H_
