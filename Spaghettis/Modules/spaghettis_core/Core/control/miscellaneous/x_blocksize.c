
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#include "../../m_headers.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

static t_class *blocksize_class;            /* Shared. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

typedef struct _blocksize {
    t_object    x_obj;                      /* Must be the first. */
    t_outlet    *x_outlet;
    } t_blocksize;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void blocksize_bang (t_blocksize *x)
{
    outlet_float (x->x_outlet, canvas_getBlockSize (object_getOwner (cast_object (x))));
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void *blocksize_new (void)
{
    t_blocksize *x = (t_blocksize *)pd_new (blocksize_class);
    
    x->x_outlet = outlet_newFloat (cast_object (x));
    
    return x;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void blocksize_setup (void)
{
    t_class *c = NULL;
    
    c = class_new (sym_blocksize,
            (t_newmethod)blocksize_new,
            NULL,
            sizeof (t_blocksize),
            CLASS_DEFAULT,
            A_NULL);

    class_addBang (c, (t_method)blocksize_bang);
    
    blocksize_class = c;
}

void blocksize_destroy (void)
{
    class_free (blocksize_class);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------