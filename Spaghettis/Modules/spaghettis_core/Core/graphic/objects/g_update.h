
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
    int         x_isVertical;
    } t_gui;

typedef struct _guivalue {
    t_gui       x_gui;                  /* MUST be the first. */
    int         x_isLogarithmic;
    t_float     x_interval;
    t_float     x_minimum;
    t_float     x_maximum;
    t_float     x_value;
    } t_guivalue;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#define cast_gui(x)         ((t_gui *)(x))
#define cast_guivalue(x)    ((t_guivalue *)(x))

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

int gui_getWidth (t_gui *x)
{
    return x->x_width;
}

int gui_getHeight (t_gui *x)
{
    return x->x_height;
}

int gui_isVertical (t_gui *x)
{
    return x->x_isVertical;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

int guivalue_isLogarithmic (t_guivalue *x)
{
    return x->x_isLogarithmic;
}

t_float guivalue_getInterval (t_guivalue *x)
{
    return x->x_interval;
}

t_float guivalue_getMinimum (t_guivalue *x)
{
    return x->x_minimum;
}

t_float guivalue_getMaximum (t_guivalue *x)
{
    return x->x_maximum;
}

t_float guivalue_getValue (t_guivalue *x)
{
    return x->x_value;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#define GUI_WIDTH_MINIMUM   8
#define GUI_WIDTH_MAXIMUM   256
#define GUI_WIDTH_DEFAULT   18

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void update_width           (t_gui *x, int width, int notify);
void update_height          (t_gui *x, int height, int notify);
void update_orientation     (t_gui *x, int isVertical, int notify);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void update_logarithmic     (t_guivalue *x, int isLogarithmic, int notify);
void update_range           (t_guivalue *x, t_float minimum, t_float maximum, int notify);
void update_interval        (t_guivalue *x, t_float interval, int notify);
int  update_value           (t_guivalue *x, t_float f, int notify);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#endif // G_UPDATE_H_
