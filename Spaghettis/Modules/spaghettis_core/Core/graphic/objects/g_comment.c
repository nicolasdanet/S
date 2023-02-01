
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#include "../../m_spaghettis.h"
#include "../../m_core.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

t_class *comment_class;     /* Shared. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

static void comment_anything (t_object *x, t_symbol *s, int argc, t_atom *argv)
{
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void comment_makeObject (t_glist *glist, t_symbol *s, int argc, t_atom *argv)
{
    if (argc >= 2) {
    //
    t_object *x = (t_object *)pd_new (comment_class);
    
    int a = atom_getFloat (argv + 0);
    int b = atom_getFloat (argv + 1);
    
    t_buffer *t = buffer_new();

    if (argc > 2) { buffer_deserialize (t, argc - 2, argv + 2); }
    else {
        buffer_appendSymbol (t, sym_comment);
    }
    
    object_setBuffer (x, t);
    object_setX (x, a);
    object_setY (x, b);
    object_setType (x, TYPE_COMMENT);
    
    glist_objectAdd (glist, x);
    //
    } else { PD_BUG; }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void comment_setup (void)
{
    t_class *c = NULL;
    
    c = class_new (sym_comment,
            NULL,
            NULL, 
            sizeof (t_object),
            CLASS_DEFAULT | CLASS_NOINLET,
            A_NULL);
        
    class_addAnything (c, (t_method)comment_anything);
        
    comment_class = c;
}

PD_LOCAL void comment_destroy (void)
{
    class_free (comment_class);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
