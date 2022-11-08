
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "../../m_spaghettis.h"
#include "../../m_core.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#if defined ( PD_BUILDING_APPLICATION )

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

static t_class *undoconnect_class;          /* Shared. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

typedef struct _undoconnect {
    t_undoaction    x_undo;                 /* Must be the first. */
    t_id            x_src;
    t_id            x_dest;
    int             x_m;
    int             x_n;
    } t_undoconnect;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void undoconnect_undo (t_undoconnect *z, t_symbol *s, int argc, t_atom *argv)
{
    unique_objectLineDisconnect (z->x_src, z->x_m, z->x_dest, z->x_n);
}

static void undoconnect_redo (t_undoconnect *z, t_symbol *s, int argc, t_atom *argv)
{
    unique_objectLineConnect (z->x_src, z->x_m, z->x_dest, z->x_n);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL t_undoaction *undoconnect_new (t_object *src, int m, t_object *dest, int n)
{
    t_undoaction *x  = (t_undoaction *)pd_new (undoconnect_class);
    t_undoconnect *z = (t_undoconnect *)x;
    
    int safe = (object_isSignalOutlet (src, m) == 0);
    
    x->ua_id    = 0;
    x->ua_type  = UNDO_CONNECT;
    x->ua_safe  = safe;
    x->ua_label = sym_connect;
    
    z->x_src    = object_getUnique (src);
    z->x_dest   = object_getUnique (dest);
    z->x_m      = m;
    z->x_n      = n;
    
    return x;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void undoconnect_setup (void)
{
    t_class *c = NULL;
    
    c = class_new (sym_undoconnect,
            NULL,
            NULL,
            sizeof (t_undoconnect),
            CLASS_INVISIBLE,
            A_NULL);
    
    class_addMethod (c, (t_method)undoconnect_undo, sym_undo, A_GIMME, A_NULL);
    class_addMethod (c, (t_method)undoconnect_redo, sym_redo, A_GIMME, A_NULL);
    
    undoconnect_class = c;
}

PD_LOCAL void undoconnect_destroy (void)
{
    class_free (undoconnect_class);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#endif

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
