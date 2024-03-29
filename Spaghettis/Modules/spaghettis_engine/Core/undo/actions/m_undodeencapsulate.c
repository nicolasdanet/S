
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "../../m_headers.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

static t_class *undodeencapsulate_class;        /* Shared. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

typedef struct _undodeencapsulate {
    t_undoaction    x_undo;                     /* Must be the first. */
    } t_undodeencapsulate;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

t_undoaction *undodeencapsulate_new (void)
{
    t_undoaction *x = (t_undoaction *)pd_new (undodeencapsulate_class);
    
    x->ua_id    = 0;
    x->ua_type  = UNDO_DEENCAPSULATE;
    x->ua_safe  = 0;
    x->ua_label = sym_de__dash__encapsulate;
    
    return x;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void undodeencapsulate_setup (void)
{
    t_class *c = NULL;
    
    c = class_new (sym_undodeencapsulate,
            NULL,
            NULL,
            sizeof (t_undodeencapsulate),
            CLASS_INVISIBLE,
            A_NULL);
    
    undodeencapsulate_class = c;
}

void undodeencapsulate_destroy (void)
{
    class_free (undodeencapsulate_class);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
