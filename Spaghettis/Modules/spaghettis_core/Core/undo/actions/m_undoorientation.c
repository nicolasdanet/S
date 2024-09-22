
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "../../m_headers.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

static t_class *undoorientation_class;      /* Shared. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

typedef struct _undoorientation {
    t_undoaction    x_undo;                 /* Must be the first. */
    int             x_isVertical;
    } t_undoorientation;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void undoorientation_undo (t_undoorientation *z, t_symbol *s, int argc, t_atom *argv)
{
    t_undoaction *x = (t_undoaction *)z;
    
    unique_objectOrientation (undoaction_getUnique (x), z->x_isVertical ? 0 : 1);
}

static void undoorientation_redo (t_undoorientation *z, t_symbol *s, int argc, t_atom *argv)
{
    t_undoaction *x = (t_undoaction *)z;
    
    unique_objectOrientation (undoaction_getUnique (x), z->x_isVertical ? 1 : 0);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

t_undoaction *undoorientation_new (t_object *o, int isVertical)
{
    t_undoaction *x      = (t_undoaction *)pd_new (undoorientation_class);
    t_undoorientation *z = (t_undoorientation *)x;
    
    x->ua_id        = object_getUnique (o);
    x->ua_type      = UNDO_ORIENTATION;
    x->ua_safe      = 1;
    x->ua_label     = sym_orientation;
    
    z->x_isVertical = isVertical;
    
    return x;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void undoorientation_setup (void)
{
    t_class *c = NULL;
    
    c = class_new (sym_undoorientation,
            NULL,
            NULL,
            sizeof (t_undoorientation),
            CLASS_INVISIBLE,
            A_NULL);
    
    class_addMethod (c, (t_method)undoorientation_undo, sym_undo, A_GIMME, A_NULL);
    class_addMethod (c, (t_method)undoorientation_redo, sym_redo, A_GIMME, A_NULL);
    
    undoorientation_class = c;
}

void undoorientation_destroy (void)
{
    class_free (undoorientation_class);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
