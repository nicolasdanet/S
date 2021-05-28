
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#include "../../m_spaghettis.h"
#include "../../m_core.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "x_list.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

static t_class *listgroup_class;            /* Shared. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

typedef struct _listgroup {
    t_listinlethelper   x_h;                /* Must be the first. */
    t_float             x_group;
    t_outlet            *x_outlet;
    } t_listgroup;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void listgroup_clear (t_listgroup *);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void listgroup_output (t_listgroup *x)
{
    int m = listinlet_listSize (&x->x_h.lh_listinlet);
    int n = PD_MAX (1, (int)x->x_group);
    t_atom *t = NULL;
    
    int count = PD_MIN (m, n);
    
    if (!count) { outlet_list (x->x_outlet, 0, NULL); }
    else {
    //
    PD_ATOMS_ALLOCA (t, m);
    
    listinlet_copyAtomsUnchecked (&x->x_h.lh_listinlet, t);
    outlet_list (x->x_outlet, count, t);
    
    PD_ATOMS_FREEA (t, m);
    //
    }
    
    listgroup_clear (x);
}

static void listgroup_append (t_listgroup *x, int argc, t_atom *argv)
{
    int i, n = PD_MAX (1, (int)x->x_group);
    
    for (i = 0; i < argc; i++) {
    //
    listinlet_listAppend (&x->x_h.lh_listinlet, 1, argv + i);
    
    if (listinlet_listSize (&x->x_h.lh_listinlet) >= n) {
        listgroup_output (x);
    }
    //
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void listgroup_bang (t_listgroup *x)
{
    listgroup_output (x);
}

static void listgroup_float (t_listgroup *x, t_float f)
{
    t_atom a; SET_FLOAT (&a, f); listgroup_append (x, 1, &a);
}

static void listgroup_symbol (t_listgroup *x, t_symbol *s)
{
    t_atom a; SET_SYMBOL (&a, s); listgroup_append (x, 1, &a);
}

static void listgroup_list (t_listgroup *x, t_symbol *s, int argc, t_atom *argv)
{
    listgroup_append (x, argc, argv);
}

static void listgroup_anything (t_listgroup *x, t_symbol *s, int argc, t_atom *argv)
{
    utils_anythingToList (cast_pd (x), (t_listmethod)listgroup_list, s, argc, argv);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void listgroup_clear (t_listgroup *x)
{
    listinlet_listClear (&x->x_h.lh_listinlet);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static t_buffer *listgroup_functionData (t_object *z, int flags)
{
    if (SAVED_DEEP (flags)) {
    //
    t_listgroup *x = (t_listgroup *)z;
    t_buffer *b = listhelper_functionData (z, flags);
    
    buffer_appendFloat (b, x->x_group);

    return b;
    //
    }
    
    return NULL;
}

static void listgroup_restore (t_listgroup *x, t_symbol *s, int argc, t_atom *argv)
{
    if (argc) {
    //
    t_listgroup *old = (t_listgroup *)instance_pendingFetch (cast_object (x));

    x->x_group = old ? old->x_group : atom_getFloatAtIndex (argc - 1, argc, argv);
    
    argc--; listhelper_restore ((t_listinlethelper *)x, s, argc, argv);
    //
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void *listgroup_new (t_symbol *s, int argc, t_atom *argv)
{
    t_listgroup *x = (t_listgroup *)pd_new (listgroup_class);
    
    listinlet_init (&x->x_h.lh_listinlet);
    
    if (argc) { x->x_group = atom_getFloat (argv); argc--; argv++; }
    
    x->x_outlet = outlet_newList (cast_object (x));
    
    inlet_newFloat (cast_object (x), &x->x_group);
    
    if (argc) { warning_unusedArguments (cast_object (x), s, argc, argv); }
    
    return x;
}

static void listgroup_free (t_listgroup *x)
{
    listinlet_free (&x->x_h.lh_listinlet);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void listgroup_setup (void)
{
    t_class *c = NULL;
    
    c = class_new (sym_list__space__group,
            (t_newmethod)listgroup_new,
            (t_method)listgroup_free,
            sizeof (t_listgroup),
            CLASS_DEFAULT,
            A_GIMME,
            A_NULL);
    
    class_addBang (c, (t_method)listgroup_bang);
    class_addFloat (c, (t_method)listgroup_float);
    class_addSymbol (c, (t_method)listgroup_symbol);
    class_addList (c, (t_method)listgroup_list);
    class_addAnything (c, (t_method)listgroup_anything);
    
    class_addMethod (c, (t_method)listgroup_clear,      sym_clear,      A_NULL);
    class_addMethod (c, (t_method)listgroup_restore,    sym__restore,   A_GIMME, A_NULL);

    class_setDataFunction (c, listgroup_functionData);
    class_requirePending (c);
    
    class_setHelpName (c, &s_list);
    
    listgroup_class = c;
}

PD_LOCAL void listgroup_destroy (void)
{
    class_free (listgroup_class);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

