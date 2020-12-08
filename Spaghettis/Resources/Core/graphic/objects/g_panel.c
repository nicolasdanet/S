
/* Copyright (c) 1997-2020 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#include "../../m_spaghettis.h"
#include "../../m_core.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#define PANEL_WIDTH_DEFAULT         275
#define PANEL_HEIGHT_DEFAULT        45
#define PANEL_SIZE_MINIMUM          1

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static t_class *panel_class;        /* Shared. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

typedef struct _panel {
    t_object    x_obj;              /* MUST be the first. */
    int         x_width;
    int         x_height;
    } t_panel;
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void panel_size (t_panel *x, t_symbol *s, int argc, t_atom *argv)
{
    if (argc) {
    //
    int n = atom_getFloatAtIndex (0, argc, argv); x->x_width  = PD_MAX (n, PANEL_SIZE_MINIMUM);
    //
    }
    
    if (argc > 1) {
    //
    int n = atom_getFloatAtIndex (1, argc, argv); x->x_height = PD_MAX (n, PANEL_SIZE_MINIMUM);
    //
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void panel_functionSave (t_object *z, t_buffer *b, int flags)
{
    t_panel *x = (t_panel *)z;
    
    buffer_appendSymbol (b, sym___hash__X);
    buffer_appendSymbol (b, sym_obj);
    buffer_appendFloat (b,  object_getX (z));
    buffer_appendFloat (b,  object_getY (z));
    buffer_appendSymbol (b, sym_panel);
    buffer_appendFloat (b,  x->x_width);
    buffer_appendFloat (b,  x->x_height);
    buffer_appendSemicolon (b);
    
    object_saveIdentifiers (z, b, flags);
}

static void panel_restore (t_panel *x)
{
    t_panel *old = (t_panel *)instance_pendingFetch (cast_object (x));
    
    if (old) { }    // ???
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void *panel_new (t_symbol *s, int argc, t_atom *argv)
{
    t_panel *x = (t_panel *)pd_new (panel_class);
    
    int width  = (argc > 1) ? (int)atom_getFloat (argv + 0) : PANEL_WIDTH_DEFAULT;
    int height = (argc > 1) ? (int)atom_getFloat (argv + 1) : PANEL_HEIGHT_DEFAULT;
        
    x->x_width  = PD_MAX (width, PANEL_SIZE_MINIMUM);
    x->x_height = PD_MAX (height, PANEL_SIZE_MINIMUM);

    return x;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void panel_setup (void)
{
    t_class *c = NULL;
    
    c = class_new (sym_panel,
            (t_newmethod)panel_new,
            NULL,
            sizeof (t_panel), 
            CLASS_DEFAULT | CLASS_NOINLET,
            A_GIMME,
            A_NULL);
        
    class_addMethod (c, (t_method)panel_size,       sym_size,       A_GIMME, A_NULL);
    class_addMethod (c, (t_method)panel_restore,    sym__restore,   A_NULL);

    class_setSaveFunction (c, panel_functionSave);
    class_setDataFunction (c, object_functionData);
    class_requirePending (c);
    
    panel_class = c;
}

PD_LOCAL void panel_destroy (void)
{
    class_free (panel_class);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
