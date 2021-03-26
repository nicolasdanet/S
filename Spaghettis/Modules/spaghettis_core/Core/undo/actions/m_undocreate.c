
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "../../m_spaghettis.h"
#include "../../m_core.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

static t_class *undocreate_class;       /* Shared. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

typedef struct _undocreate {
    t_undoaction    x_undo;             /* Must be the first. */
    t_undosnippet   *x_snippet;
    } t_undocreate;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL int    object_hasDspOrIsGraphicArray   (t_object *);
PD_LOCAL void   undoaction_setInletsAndOutlets  (t_undoaction *, t_object *);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void undocreate_undo (t_undocreate *z, t_symbol *s, int argc, t_atom *argv)
{
    t_undoaction *x = (t_undoaction *)z;
    
    undosnippet_update (z->x_snippet); unique_objectRemove (undoaction_getUnique (x));
}

static void undocreate_redo (t_undocreate *z, t_symbol *s, int argc, t_atom *argv)
{
    undosnippet_load (z->x_snippet); undosnippet_z (z->x_snippet);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

/* Take ownership of snippet. */

PD_LOCAL t_undoaction *undocreate_new (t_object *o, t_undosnippet *snippet)
{
    t_undoaction *x = (t_undoaction *)pd_new (undocreate_class);
    t_undocreate *z = (t_undocreate *)x;
    
    // -- TODO: Consider if arrays always require to rebuild the graph?
    
    int safe = (object_hasDspOrIsGraphicArray (o) == 0);
    
    x->ua_id    = object_getUnique (o);
    x->ua_type  = UNDO_CREATE;
    x->ua_safe  = safe;
    x->ua_label = sym_create;

    undoaction_setInletsAndOutlets (x, o);
    
    PD_ASSERT (snippet);
    
    z->x_snippet = snippet;

    return x;
}

static void undocreate_free (t_undocreate *z)
{
    undosnippet_free (z->x_snippet);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void undocreate_setup (void)
{
    t_class *c = NULL;
    
    c = class_new (sym_undocreate,
            NULL,
            (t_method)undocreate_free,
            sizeof (t_undocreate),
            CLASS_INVISIBLE,
            A_NULL);
    
    class_addMethod (c, (t_method)undocreate_undo, sym_undo, A_GIMME, A_NULL);
    class_addMethod (c, (t_method)undocreate_redo, sym_redo, A_GIMME, A_NULL);
    
    undocreate_class = c;
}

PD_LOCAL void undocreate_destroy (void)
{
    class_free (undocreate_class);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
