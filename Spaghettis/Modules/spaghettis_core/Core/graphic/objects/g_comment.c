
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#include "../../m_headers.h"

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

void comment_makeObject (t_glist *glist, t_symbol *s, int argc, t_atom *argv)
{
    if (argc >= 2) {
    //
    t_object *x = (t_object *)pd_new (comment_class);
    
    int a = atom_getFloat (argv + 0);
    int b = atom_getFloat (argv + 1);
    
    t_buffer *t = buffer_new();

    if (argc > 2) { buffer_deserialize (t, argc - 2, argv + 2); }
    
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

#if defined ( PD_BUILDING_APPLICATION )

static void comment_functionGetParameters (t_object *o, data::Group& group, const Tags& t)
{
    gui_getParameters (o, group, t, GUI_TEXT);
}

static void comment_functionSetParameters (t_object *o, const data::Group& group)
{
    gui_setParameters (o, group, GUI_TEXT);
}

#endif

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void comment_setup (void)
{
    t_class *c = NULL;
    
    c = class_new (sym_comment,
            NULL,
            NULL, 
            sizeof (t_object),
            CLASS_DEFAULT | CLASS_NOINLET,
            A_NULL);
        
    class_addAnything (c, (t_method)comment_anything);
    
    #if defined ( PD_BUILDING_APPLICATION )
    
    class_setParametersFunctions (c, comment_functionGetParameters, comment_functionSetParameters);
    
    #endif
    
    comment_class = c;
}

void comment_destroy (void)
{
    class_free (comment_class);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
