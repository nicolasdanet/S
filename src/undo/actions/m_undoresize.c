
/* Copyright (c) 1997-2020 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "../../m_spaghettis.h"
#include "../../m_core.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

static t_class *undoresize_class;           /* Shared. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

typedef struct _undoresize {
    t_undoaction    x_undo;                 /* Must be the first. */
    int             x_old;
    int             x_new;
    } t_undoresize;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void undoresize_collapse (t_undoaction *kept, t_undoaction *deleted)
{
    t_undoresize *a = (t_undoresize *)kept;
    t_undoresize *b = (t_undoresize *)deleted;
    
    PD_ASSERT (pd_class (kept)    == undoresize_class);
    PD_ASSERT (pd_class (deleted) == undoresize_class);
    
    a->x_old = b->x_old;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void undoresize_undo (t_undoresize *z, t_symbol *s, int argc, t_atom *argv)
{
    t_undoaction *x = (t_undoaction *)z;
    
    unique_objectResizeBox (undoaction_getUnique (x), z->x_old);
}

static void undoresize_redo (t_undoresize *z, t_symbol *s, int argc, t_atom *argv)
{
    t_undoaction *x = (t_undoaction *)z;
    
    unique_objectResizeBox (undoaction_getUnique (x), z->x_new);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL t_undoaction *undoresize_new (t_object *o, int m, int n)
{
    t_undoaction *x = (t_undoaction *)pd_new (undoresize_class);
    t_undoresize *z = (t_undoresize *)x;
    
    x->ua_id    = object_getUnique (o);
    x->ua_type  = UNDO_RESIZE;
    x->ua_safe  = 1;
    x->ua_label = sym_resize;
    
    z->x_old    = m;
    z->x_new    = n;
    
    return x;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void undoresize_setup (void)
{
    t_class *c = NULL;
    
    c = class_new (sym_undoresize,
            NULL,
            NULL,
            sizeof (t_undoresize),
            CLASS_INVISIBLE,
            A_NULL);
    
    class_addMethod (c, (t_method)undoresize_undo, sym_undo, A_GIMME, A_NULL);
    class_addMethod (c, (t_method)undoresize_redo, sym_redo, A_GIMME, A_NULL);
    
    undoresize_class = c;
}

PD_LOCAL void undoresize_destroy (void)
{
    class_free (undoresize_class);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
