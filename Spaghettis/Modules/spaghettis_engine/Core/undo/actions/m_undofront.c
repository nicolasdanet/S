
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "../../m_headers.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

static t_class *undofront_class;        /* Shared. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

typedef struct _undofront {
    t_undoaction    x_undo;             /* Must be the first. */
    t_undosnippet   *x_snippet;
    } t_undofront;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void undofront_undo (t_undofront *z, t_symbol *s, int argc, t_atom *argv)
{
    undosnippet_z (z->x_snippet);
}

static void undofront_redo (t_undofront *z, t_symbol *s, int argc, t_atom *argv)
{
    undosnippet_front (z->x_snippet);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

t_undoaction *undofront_new (t_object *o, t_undosnippet *snippet)
{
    t_undoaction *x = (t_undoaction *)pd_new (undofront_class);
    t_undofront *z  = (t_undofront *)x;
    
    x->ua_id    = object_getUnique (o);
    x->ua_type  = UNDO_FRONT;
    x->ua_safe  = 1;
    x->ua_label = sym_front;
    
    PD_ASSERT (snippet);
    
    z->x_snippet = snippet;
    
    return x;
}

static void undofront_free (t_undofront *z)
{
    undosnippet_free (z->x_snippet);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void undofront_setup (void)
{
    t_class *c = NULL;
    
    c = class_new (sym_undofront,
            NULL,
            (t_method)undofront_free,
            sizeof (t_undofront),
            CLASS_INVISIBLE,
            A_NULL);
    
    class_addMethod (c, (t_method)undofront_undo, sym_undo, A_GIMME, A_NULL);
    class_addMethod (c, (t_method)undofront_redo, sym_redo, A_GIMME, A_NULL);
    
    undofront_class = c;
}

void undofront_destroy (void)
{
    class_free (undofront_class);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
