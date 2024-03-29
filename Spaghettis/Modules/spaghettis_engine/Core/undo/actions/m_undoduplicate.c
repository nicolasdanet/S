
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "../../m_headers.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

static t_class *undoduplicate_class;        /* Shared. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

typedef struct _undoduplicate {
    t_undoaction    x_undo;                 /* Must be the first. */
    } t_undoduplicate;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

t_undoaction *undoduplicate_new (void)
{
    t_undoaction *x = (t_undoaction *)pd_new (undoduplicate_class);
    
    x->ua_id    = 0;
    x->ua_type  = UNDO_DUPLICATE;
    x->ua_safe  = 1;
    x->ua_label = sym_duplicate;
    
    return x;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void undoduplicate_setup (void)
{
    t_class *c = NULL;
    
    c = class_new (sym_undoduplicate,
            NULL,
            NULL,
            sizeof (t_undoduplicate),
            CLASS_INVISIBLE,
            A_NULL);
    
    undoduplicate_class = c;
}

void undoduplicate_destroy (void)
{
    class_free (undoduplicate_class);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
