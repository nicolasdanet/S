
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#include "../../m_headers.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

static t_class *listfromsymbol_class;       /* Shared. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

typedef struct _listfromsymbol {
    t_object    x_obj;                      /* Must be the first. */
    t_outlet    *x_outlet;
    } t_listfromsymbol;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void listfromsymbol_symbol (t_listfromsymbol *x, t_symbol *s)
{
    t_atom *t = NULL; int n, count = (int)(strlen (s->s_name));
    
    if (!count) { outlet_list (x->x_outlet, 0, NULL); }
    else {
    //
    PD_ATOMS_ALLOCA (t, count);
    
    for (n = 0; n < count; n++) { SET_FLOAT (t + n, (unsigned char)s->s_name[n]); }
    
    outlet_list (x->x_outlet, count, t);
    
    PD_ATOMS_FREEA (t, count);
    //
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void *listfromsymbol_new (t_symbol *s, int argc, t_atom *argv)
{
    t_listfromsymbol *x = (t_listfromsymbol *)pd_new (listfromsymbol_class);
    
    x->x_outlet = outlet_newList (cast_object (x));
    
    if (argc) { warning_unusedArguments (cast_object (x), s, argc, argv); }
    
    return x;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void listfromsymbol_setup (void)
{
    t_class *c = NULL;
    
    c = class_new (sym_list__space__fromsymbol,
            (t_newmethod)listfromsymbol_new,
            NULL,
            sizeof (t_listfromsymbol),
            CLASS_DEFAULT,
            A_GIMME,
            A_NULL);
            
    class_addSymbol (c, (t_method)listfromsymbol_symbol);
    
    class_setHelpName (c, &s_list);
    
    listfromsymbol_class = c;
}

void listfromsymbol_destroy (void)
{
    class_free (listfromsymbol_class);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

