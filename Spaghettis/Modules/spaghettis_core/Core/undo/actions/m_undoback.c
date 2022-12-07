
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "../../m_spaghettis.h"
#include "../../m_core.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

static t_class *undoback_class;         /* Shared. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

typedef struct _undoback {
    t_undoaction    x_undo;             /* Must be the first. */
    t_undosnippet   *x_snippet;
    } t_undoback;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void undoback_undo (t_undoback *z, t_symbol *s, int argc, t_atom *argv)
{
    undosnippet_z (z->x_snippet);
}

static void undoback_redo (t_undoback *z, t_symbol *s, int argc, t_atom *argv)
{
    undosnippet_back (z->x_snippet);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL t_undoaction *undoback_new (t_object *o, t_undosnippet *snippet)
{
    t_undoaction *x = (t_undoaction *)pd_new (undoback_class);
    t_undoback *z   = (t_undoback *)x;
    
    x->ua_id    = object_getUnique (o);
    x->ua_type  = UNDO_BACK;
    x->ua_safe  = 1;
    x->ua_label = sym_back;
    
    PD_ASSERT (snippet);
    
    z->x_snippet = snippet;
    
    return x;
}

static void undoback_free (t_undoback *z)
{
    undosnippet_free (z->x_snippet);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void undoback_setup (void)
{
    t_class *c = NULL;
    
    c = class_new (sym_undoback,
            NULL,
            (t_method)undoback_free,
            sizeof (t_undoback),
            CLASS_INVISIBLE,
            A_NULL);
    
    class_addMethod (c, (t_method)undoback_undo, sym_undo, A_GIMME, A_NULL);
    class_addMethod (c, (t_method)undoback_redo, sym_redo, A_GIMME, A_NULL);
    
    undoback_class = c;
}

PD_LOCAL void undoback_destroy (void)
{
    class_free (undoback_class);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
