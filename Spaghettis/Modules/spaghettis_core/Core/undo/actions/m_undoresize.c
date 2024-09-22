
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "../../m_headers.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

static t_class *undoresize_class;           /* Shared. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

typedef struct _undoresize {
    t_undoaction    x_undo;                 /* Must be the first. */
    int             x_oldWidth;
    int             x_oldHeight;
    int             x_newWidth;
    int             x_newHeight;
    } t_undoresize;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void undoresize_collapse (t_undoaction *kept, t_undoaction *deleted)
{
    t_undoresize *a = (t_undoresize *)kept;
    t_undoresize *b = (t_undoresize *)deleted;              /* Previous action. */
    
    PD_ASSERT (pd_class (kept)    == undoresize_class);
    PD_ASSERT (pd_class (deleted) == undoresize_class);
    
    a->x_oldWidth  = b->x_oldWidth;
    a->x_oldHeight = b->x_oldHeight;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void undoresize_undo (t_undoresize *z, t_symbol *s, int argc, t_atom *argv)
{
    t_undoaction *x = (t_undoaction *)z;
    
    unique_objectResize (undoaction_getUnique (x), z->x_oldWidth, z->x_oldHeight);
}

static void undoresize_redo (t_undoresize *z, t_symbol *s, int argc, t_atom *argv)
{
    t_undoaction *x = (t_undoaction *)z;
    
    unique_objectResize (undoaction_getUnique (x), z->x_newWidth, z->x_newHeight);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

t_undoaction *undoresize_new (t_object *o, int oldWidth, int oldHeight, int newWidth, int newHeight)
{
    t_undoaction *x = (t_undoaction *)pd_new (undoresize_class);
    t_undoresize *z = (t_undoresize *)x;
    
    x->ua_id    = object_getUnique (o);
    x->ua_type  = UNDO_RESIZE;
    x->ua_safe  = 1;
    x->ua_label = sym_resize;
    
    z->x_oldWidth  = oldWidth;
    z->x_oldHeight = oldHeight;
    z->x_newWidth  = newWidth;
    z->x_newHeight = newHeight;
    
    return x;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void undoresize_setup (void)
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

void undoresize_destroy (void)
{
    class_free (undoresize_class);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
