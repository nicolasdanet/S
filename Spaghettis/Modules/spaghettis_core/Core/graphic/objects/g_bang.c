
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#include "../../m_spaghettis.h"
#include "../../m_core.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#define BANG_TIME_DEFAULT       250
#define BANG_TIME_MINIMUM       10
#define BANG_SIZE_DEFAULT       15
#define BANG_SIZE_MINIMUM       8

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static t_class *bng_class;      /* Shared. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

typedef struct _bng {
    t_object    x_obj;          /* MUST be the first. */
    int         x_flashed;
    int         x_width;
    int         x_time;
    t_outlet    *x_outlet;
    t_clock     *x_clock;
    } t_bng;
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void bng_taskFlash (t_bng *x)
{
    x->x_flashed = 0;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void bng_bang (t_bng *x)
{
    x->x_flashed = 1; clock_delay (x->x_clock, x->x_time);

    outlet_bang (x->x_outlet);
}

static void bng_float (t_bng *x, t_float f)
{
    bng_bang (x);
}

static void bng_symbol (t_bng *x, t_symbol *s)
{
    bng_bang (x);
}

static void bng_list (t_bng *x, t_symbol *s, int argc, t_atom *argv)
{
    bng_bang (x);
}

static void bng_anything (t_bng *x, t_symbol *s, int argc, t_atom *argv)
{
    bng_bang (x);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void bng_size (t_bng *x, t_symbol *s, int argc, t_atom *argv)
{
    if (argc) { int n = atom_getFloatAtIndex (0, argc, argv); x->x_width = PD_MAX (n, BANG_SIZE_MINIMUM); }
}

static void bng_flashtime (t_bng *x, t_float f)
{
    x->x_time = PD_MAX ((int)f, BANG_TIME_MINIMUM);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void bng_functionSave (t_object *z, t_buffer *b, int flags)
{
    t_bng *x = (t_bng *)z;
    
    buffer_appendSymbol (b, sym___hash__X);
    buffer_appendSymbol (b, sym_obj);
    buffer_appendFloat (b,  object_getX (cast_object (x)));
    buffer_appendFloat (b,  object_getY (cast_object (x)));
    buffer_appendSymbol (b, sym_bng);
    buffer_appendFloat (b,  x->x_width);
    buffer_appendFloat (b,  x->x_time);
    buffer_appendSemicolon (b);
    
    object_saveIdentifiers (z, b, flags);
}

static void bng_restore (t_bng *x)
{
    t_bng *old = (t_bng *)instance_pendingFetch (cast_object (x));
    
    if (old) { }    /* ??? */
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void *bng_new (t_symbol *s, int argc, t_atom *argv)
{
    t_bng *x = (t_bng *)pd_new (bng_class);
    
    int size = (argc > 1) ? (int)atom_getFloat (argv + 0) : BANG_SIZE_DEFAULT;
    int time = (argc > 1) ? (int)atom_getFloat (argv + 1) : BANG_TIME_DEFAULT;

    x->x_width  = PD_MAX (size, BANG_SIZE_MINIMUM);
    x->x_time   = PD_MAX (time, BANG_TIME_MINIMUM);
    
    x->x_outlet = outlet_newBang (cast_object (x));
    x->x_clock  = clock_new ((void *)x, (t_method)bng_taskFlash);
    
    return x;
}

static void bng_free (t_bng *x)
{
    clock_free (x->x_clock);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void bng_setup (void)
{
    t_class *c = NULL;
    
    c = class_new (sym_bng, 
            (t_newmethod)bng_new, 
            (t_method)bng_free, 
            sizeof (t_bng), 
            CLASS_DEFAULT,
            A_GIMME, 
            A_NULL);
    
    class_addBang (c, (t_method)bng_bang);
    class_addFloat (c, (t_method)bng_float);
    class_addSymbol (c, (t_method)bng_symbol);
    class_addList (c, (t_method)bng_list);
    class_addAnything (c, (t_method)bng_anything);
    
    class_addMethod (c, (t_method)bng_size,         sym_size,       A_GIMME, A_NULL);
    class_addMethod (c, (t_method)bng_flashtime,    sym_flashtime,  A_FLOAT, A_NULL);
    class_addMethod (c, (t_method)bng_restore,      sym__restore,   A_NULL);

    class_setSaveFunction (c, bng_functionSave);
    class_setDataFunction (c, object_functionData);
    class_requirePending (c);
    
    bng_class = c;
}

PD_LOCAL void bng_destroy (void)
{
    class_free (bng_class);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
