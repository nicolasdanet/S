
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "../../m_spaghettis.h"
#include "../../m_core.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

static t_class *undomotion_class;       /* Shared. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

typedef struct _undomotion {
    t_undoaction    x_undo;             /* Must be the first. */
    int             x_oldX;
    int             x_oldY;
    int             x_newX;
    int             x_newY;
    } t_undomotion;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void undomotion_collapse (t_undoaction *kept, t_undoaction *deleted)
{
    t_undomotion *a = (t_undomotion *)kept;
    t_undomotion *b = (t_undomotion *)deleted;
    
    PD_ASSERT (pd_class (kept)    == undomotion_class);
    PD_ASSERT (pd_class (deleted) == undomotion_class);
    
    a->x_oldX = b->x_oldX;
    a->x_oldY = b->x_oldY;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void undomotion_undo (t_undomotion *z, t_symbol *s, int argc, t_atom *argv)
{
    t_undoaction *x = (t_undoaction *)z;
    
    unique_objectPosition (undoaction_getUnique (x), z->x_oldX, z->x_oldY);
}

static void undomotion_redo (t_undomotion *z, t_symbol *s, int argc, t_atom *argv)
{
    t_undoaction *x = (t_undoaction *)z;
    
    unique_objectPosition (undoaction_getUnique (x), z->x_newX, z->x_newY);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL t_undoaction *undomotion_new (t_object *o, int oldX, int oldY, int newX, int newY)
{
    t_undoaction *x = (t_undoaction *)pd_new (undomotion_class);
    t_undomotion *z = (t_undomotion *)x;
    
    x->ua_id    = object_getUnique (o);
    x->ua_type  = UNDO_MOTION;
    x->ua_safe  = 1;
    x->ua_label = sym_motion;
    
    z->x_oldX   = oldX;
    z->x_oldY   = oldY;
    z->x_newX   = newX;
    z->x_newY   = newY;
    
    return x;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void undomotion_setup (void)
{
    t_class *c = NULL;
    
    c = class_new (sym_undomotion,
            NULL,
            NULL,
            sizeof (t_undomotion),
            CLASS_INVISIBLE,
            A_NULL);
    
    class_addMethod (c, (t_method)undomotion_undo, sym_undo, A_GIMME, A_NULL);
    class_addMethod (c, (t_method)undomotion_redo, sym_redo, A_GIMME, A_NULL);

    undomotion_class = c;
}

PD_LOCAL void undomotion_destroy (void)
{
    class_free (undomotion_class);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
