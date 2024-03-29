
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#include "../../m_headers.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

static t_class *listlength_class;      /* Shared. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

typedef struct _listlength {
    t_object    x_obj;                  /* Must be the first. */
    t_outlet    *x_outlet;
    } t_listlength;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void listlength_list (t_listlength *x, t_symbol *s, int argc, t_atom *argv)
{
    outlet_float (x->x_outlet, (t_float)argc);
}

static void listlength_anything (t_listlength *x, t_symbol *s, int argc, t_atom *argv)
{
    outlet_float (x->x_outlet, (t_float)(argc + 1));
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void *listlength_new (t_symbol *s, int argc, t_atom *argv)
{
    t_listlength *x = (t_listlength *)pd_new (listlength_class);
    
    x->x_outlet = outlet_newFloat (cast_object (x));
    
    if (argc) { warning_unusedArguments (cast_object (x), s, argc, argv); }
    
    return x;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void listlength_setup (void)
{
    t_class *c = NULL;
    
    c = class_new (sym_list__space__length,
            (t_newmethod)listlength_new,
            NULL,
            sizeof (t_listlength),
            CLASS_DEFAULT,
            A_GIMME,
            A_NULL);
        
    class_addList (c, (t_method)listlength_list);
    class_addAnything (c, (t_method)listlength_anything);
    
    class_setHelpName (c, &s_list);
    
    listlength_class = c;
}

void listlength_destroy (void)
{
    class_free (listlength_class);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

