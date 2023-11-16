
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#include "../../m_spaghettis.h"
#include "../../m_core.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

static t_class *midisystemout_class;            /* Shared. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

typedef struct _midisystemout {
    t_object    x_obj;                          /* Must be the first. */
    t_float     x_port;
    } t_midisystemout;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void midisystemout_list (t_midisystemout *x, t_symbol *s, int argc, t_atom *argv)
{
    outmidi_system (x->x_port, argc, argv);
}

static void midisystemout_anything (t_midisystemout *x, t_symbol *s, int argc, t_atom *argv)
{
    utils_anythingToList (cast_pd (x), (t_listmethod)midisystemout_list, s, argc, argv);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static t_buffer *midisystemout_functionData (t_object *z, int flags)
{
    if (SAVED_DEEP (flags)) {
    //
    t_midisystemout *x = (t_midisystemout *)z;
    t_buffer *b = buffer_new();
    
    buffer_appendSymbol (b, sym__restore);
    buffer_appendFloat (b, x->x_port);
    
    return b;
    //
    }
    
    return NULL;
}

static void midisystemout_restore (t_midisystemout *x, t_float f)
{
    t_midisystemout *old = (t_midisystemout *)instance_pendingFetch (cast_object (x));

    x->x_port = old ? old->x_port : f;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void *midisystemout_new (t_float port)
{
    t_midisystemout *x = (t_midisystemout *)pd_new (midisystemout_class);
    
    x->x_port = port;
    
    inlet_newFloat (cast_object (x), &x->x_port);
    
    return x;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void midisystemout_setup (void)
{
    t_class *c = NULL;
    
    c = class_new (sym_midisystemout,
            (t_newmethod)midisystemout_new,
            NULL,
            sizeof (t_midisystemout),
            CLASS_DEFAULT,
            A_DEFFLOAT,
            A_NULL);
            
    class_addList (c, (t_method)midisystemout_list);
    class_addAnything (c, (t_method)midisystemout_anything);
    
    class_addMethod (c, (t_method)midisystemout_restore, sym__restore, A_FLOAT, A_NULL);

    class_setDataFunction (c, midisystemout_functionData);
    class_requirePending (c);
    
    class_setHelpName (c, sym_midisystemin);
    
    midisystemout_class = c;
}

void midisystemout_destroy (void)
{
    class_free (midisystemout_class);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
