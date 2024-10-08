
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
    int         x_isFlashed;
    int         x_isEmbedded;
    int         x_isIncluded;
    int         x_state;
    int         x_time;
    t_float     x_peak;
    t_float     x_nonZero;
    t_float     x_interval;
    t_float     x_low;
    t_float     x_high;
    t_float     x_value;
    t_symbol    *x_label;
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
    return x->x_buttons;
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
    return x->x_isMultiple;
}

inline int gui_isFlashed (t_gui *x)
{
    return x->x_isFlashed;
}

inline int gui_isEmbedded (t_gui *x)
{
    return x->x_isEmbedded;
}

inline int gui_isIncluded (t_gui *x)
{
    return x->x_isIncluded;
}

inline int gui_getState (t_gui *x)
{
    return x->x_state;
}

inline int gui_getTime (t_gui *x)
{
    return x->x_time;
}

inline t_float gui_getPeak (t_gui *x)
{
    return x->x_peak;
}

inline t_float gui_getNonZero (t_gui *x)
{
    return x->x_nonZero;
}

inline t_float gui_getInterval (t_gui *x)
{
    return x->x_interval;
}

inline t_float gui_getLow (t_gui *x)
{
    return x->x_low;
}

inline t_float gui_getHigh (t_gui *x)
{
    return x->x_high;
}

inline t_float gui_getValue (t_gui *x)
{
    return x->x_value;
}

inline t_symbol *gui_getLabel (t_gui *x)
{
    return x->x_label ? x->x_label : &s_;
}

inline int gui_hasLabel (t_gui *x)
{
    return gui_getLabel (x) != &s_;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#define GUI_TIME_MINIMUM                10
#define GUI_TIME_MAXIMUM                1000

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#define GUI_DIGITS_MINIMUM              0
#define GUI_DIGITS_MAXIMUM              64

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#define GUI_BUTTONS_MINIMUM             1
#define GUI_BUTTONS_MAXIMUM             32

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#define GUI_SIZE_MINIMUM                8
#define GUI_SIZE_MAXIMUM                1024

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#define GUI_STATE_DEFAULT               0
#define GUI_MINIMUM_DEFAULT             0
#define GUI_MAXIMUM_DEFAULT             127
#define GUI_INTERVAL_DEFAULT            1
#define GUI_NONZERO_DEFAULT             1
#define GUI_LOGARITHMIC_DEFAULT         0
#define GUI_MULTIPLE_DEFAULT            0
#define GUI_EMBEDDED_DEFAULT            0
#define GUI_TIME_DEFAULT                250
#define GUI_DIGITS_DEFAULT              5
#define GUI_BUTTONS_DEFAULT             8
#define GUI_SIZE_DEFAULT                18

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#define GUI_FLAG_NONE                   0
#define GUI_FLAG_NOTIFY                 1
#define GUI_FLAG_DIRTY                  2
#define GUI_FLAG_UNDO                   4

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#define GUI_UPDATE_NONE                 (GUI_FLAG_NONE)
#define GUI_UPDATE_NOTIFY               (GUI_FLAG_NOTIFY)
#define GUI_UPDATE_NOTIFY_DIRTY         (GUI_FLAG_NOTIFY | GUI_FLAG_DIRTY)
#define GUI_UPDATE_NOTIFY_DIRTY_UNDO    (GUI_FLAG_NOTIFY | GUI_FLAG_DIRTY | GUI_FLAG_UNDO)

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

int  gui_updateValue                    (t_gui *x, t_float f, int flag);
int  gui_updateState                    (t_gui *x, int n, int flag);
void gui_updateRange                    (t_gui *x, t_float minimum, t_float maximum, int flag);
void gui_updateInterval                 (t_gui *x, t_float interval, int flag);
void gui_updateNonZero                  (t_gui *x, t_float f, int flag);
int  gui_updatePeak                     (t_gui *x, t_float f, int flag);
void gui_updateLogarithmic              (t_gui *x, int isLogarithmic, int flag);
void gui_updateMultiple                 (t_gui *x, int isMultiple, int flag);
void gui_updateOrientation              (t_gui *x, int isVertical, int flag);
void gui_updateOrientationSwap          (t_gui *x, int isVertical, int flag);
void gui_updateFlashed                  (t_gui *x, int n, int flag);
void gui_updateEmbedded                 (t_gui *x, int n, int flag);
void gui_updateTime                     (t_gui *x, int n, int flag);
void gui_updateDigits                   (t_gui *x, int digits, int flag);
void gui_updateButtons                  (t_gui *x, int buttons, int flag);
void gui_updateWidth                    (t_gui *x, int width, int flag);
void gui_updateHeight                   (t_gui *x, int height, int flag);
void gui_updateIncluded                 (t_gui *x, int n, t_symbol *s, int flag);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

enum {
    GUI_NONE                = (1 << 0),
    GUI_VALUE               = (1 << 1),
    GUI_STATE               = (1 << 2),
    GUI_RANGE               = (1 << 3),
    GUI_INTERVAL            = (1 << 5),
    GUI_NONZERO             = (1 << 6),
    GUI_PEAK                = (1 << 7),
    GUI_LOGARITHMIC         = (1 << 8),
    GUI_MULTIPLE            = (1 << 9),
    GUI_ORIENTATION         = (1 << 10),
    GUI_SWAP                = (1 << 11),         // GUI_ORIENTATION
    GUI_FLASHED             = (1 << 12),
    GUI_EMBEDDED            = (1 << 13),
    GUI_TIME                = (1 << 14),
    GUI_DIGITS              = (1 << 15),
    GUI_BUTTONS             = (1 << 16),
    GUI_WIDTH               = (1 << 18),
    GUI_HEIGHT              = (1 << 19),
    GUI_INCLUDED            = (1 << 20)
    };

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#if defined ( PD_BUILDING_APPLICATION )

void gui_getParameters (t_object *, data::Group&, const Tags&, int);
bool gui_setParameters (t_object *, const data::Group&, int);

void object_getTextParameter (t_object *, data::Group&, const Tags&);
bool object_setTextParameter (t_object *, const data::Group&);

#endif

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

t_buffer *gui_functionDataInclude (t_object *);
t_buffer *gui_functionData (t_object *, int);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void gui_restore (t_gui *, int);
void gui_include (t_gui *, int, t_atom *);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#endif // G_UPDATE_H_
