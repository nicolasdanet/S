
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "../../m_headers.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

static t_class *undodisconnect_class;       /* Shared. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

typedef struct _undodisconnect {
    t_undoaction    x_undo;                 /* Must be the first. */
    t_id            x_src;
    t_id            x_dest;
    int             x_m;
    int             x_n;
    } t_undodisconnect;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

int undodisconnect_match (t_undoaction *a, t_id u, t_items *i, t_items *o)
{
    if (undoaction_getType (a) == UNDO_DISCONNECT) {
    //
    t_undodisconnect *z = (t_undodisconnect *)a;
    
    if (z->x_dest == u)     { if (items_isSetAtIndex (i, z->x_n)) { return 1; } }
    else if (z->x_src == u) { if (items_isSetAtIndex (o, z->x_m)) { return 1; } }
    //
    }
    
    return 0;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void undodisconnect_undo (t_undodisconnect *z, t_symbol *s, int argc, t_atom *argv)
{
    unique_lineConnect (z->x_src, z->x_m, z->x_dest, z->x_n);
}

static void undodisconnect_redo (t_undodisconnect *z, t_symbol *s, int argc, t_atom *argv)
{
    unique_lineDisconnect (z->x_src, z->x_m, z->x_dest, z->x_n);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

t_undoaction *undodisconnect_new (t_object *src, int m, t_object *dest, int n)
{
    t_undoaction *x     = (t_undoaction *)pd_new (undodisconnect_class);
    t_undodisconnect *z = (t_undodisconnect *)x;
    
    int safe = (object_isSignalOutlet (src, m) == 0);

    x->ua_id    = 0;
    x->ua_type  = UNDO_DISCONNECT;
    x->ua_safe  = safe;
    x->ua_label = sym_disconnect;
    
    z->x_src    = object_getUnique (src);
    z->x_dest   = object_getUnique (dest);
    z->x_m      = m;
    z->x_n      = n;
    
    return x;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void undodisconnect_setup (void)
{
    t_class *c = NULL;
    
    c = class_new (sym_undodisconnect,
            NULL,
            NULL,
            sizeof (t_undodisconnect),
            CLASS_INVISIBLE,
            A_NULL);
    
    class_addMethod (c, (t_method)undodisconnect_undo, sym_undo, A_GIMME, A_NULL);
    class_addMethod (c, (t_method)undodisconnect_redo, sym_redo, A_GIMME, A_NULL);
    
    undodisconnect_class = c;
}

void undodisconnect_destroy (void)
{
    class_free (undodisconnect_class);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
