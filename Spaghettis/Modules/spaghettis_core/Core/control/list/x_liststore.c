
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

static t_class *liststore_class;        /* Shared. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

static t_error liststore_parseIndexAndRange (t_liststore *x, int *r1, int *r2)
{
    int count = listinlet_listSize (&x->x_h.lh_listinlet);

    if (count) {
    //
    int i = (*r1);
    int n = (*r2);
    
    if (i < 0) { i = count + i; }                               /* Negative index means from the end. */
    if (n < 0) { int t = i; i = i + n; n = (t - i) + 1; }       /* A negative size means before. */
    else if (n == 0) { n = count - i; }                         /* A zero size means all. */
    
    /* Accept only slices in range. */
    
    if (i >= 0 && i < count && n > 0 && n <= count - i) {
    //
    (*r1) = i; (*r2) = n;
    
    return PD_ERROR_NONE;
    //
    }
    //
    }
    
    (*r1) = 0; (*r2) = 0;
    
    return PD_ERROR;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void liststore_clear (t_liststore *x)
{
    listinlet_listClear (&x->x_h.lh_listinlet);
}

static void liststore_append (t_liststore *x, t_symbol *s, int argc, t_atom *argv)
{
    listinlet_listAppend (&x->x_h.lh_listinlet, argc, argv);
}

static void liststore_prepend (t_liststore *x, t_symbol *s, int argc, t_atom *argv)
{
    listinlet_listPrepend (&x->x_h.lh_listinlet, argc, argv);
}

static void liststore_set (t_liststore *x, t_symbol *s, int argc, t_atom *argv)
{
    if (argc > 1) {
    //
    int i = atom_getFloatAtIndex (0, argc, argv);
    int n = argc - 1;
    
    if (liststore_parseIndexAndRange (x, &i, &n) == PD_ERROR_NONE) {
    //
    listinlet_listSetAt (&x->x_h.lh_listinlet, i, argc - 1, argv + 1);
    //
    }
    //
    }
}

static void liststore_insert (t_liststore *x, t_symbol *s, int argc, t_atom *argv)
{
    if (argc > 1) {
    //
    int i = atom_getFloatAtIndex (0, argc, argv);
    int n = 1;
    
    if (liststore_parseIndexAndRange (x, &i, &n) == PD_ERROR_NONE) {
    //
    listinlet_listInsert (&x->x_h.lh_listinlet, i, argc - 1, argv + 1);
    //
    }
    //
    }
}

static void liststore_remove (t_liststore *x, t_symbol *s, int argc, t_atom *argv)
{
    if (argc) {
    //
    int i = atom_getFloatAtIndex (0, argc, argv);
    int n = (argc > 1) ? atom_getFloatAtIndex (1, argc, argv) : 1;
    
    if (liststore_parseIndexAndRange (x, &i, &n)  == PD_ERROR_NONE) {
    //
    listinlet_listRemove (&x->x_h.lh_listinlet, i, n);
    //
    }
    //
    }
}

static void liststore_replace (t_liststore *x, t_symbol *s, int argc, t_atom *argv)
{
    if (argc > 1 && IS_FLOAT (argv + 0) && IS_FLOAT (argv + 1)) {
    //
    int i = GET_FLOAT (argv + 0);
    int n = GET_FLOAT (argv + 1);
    
    if (liststore_parseIndexAndRange (x, &i, &n) == PD_ERROR_NONE) {
    //
    listinlet_listReplace (&x->x_h.lh_listinlet, i, n, argc - 2, argv + 2);
    //
    }
    //
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void liststore_getProceed (t_liststore *x, int i, int n, int pop)
{
    t_atom *t = NULL; int count = listinlet_listSize (&x->x_h.lh_listinlet);
    
    PD_ASSERT (count);

    PD_ATOMS_ALLOCA (t, count);
    
    listinlet_copyAtomsUnchecked (&x->x_h.lh_listinlet, t);
    if (pop) { listinlet_listRemove (&x->x_h.lh_listinlet, i, n); }
    outlet_list (x->x_outlet, n, t + i);
    
    PD_ATOMS_FREEA (t, count);
}

static void liststore_get (t_liststore *x, t_symbol *s, int argc, t_atom *argv)
{
    if (argc) {
    //
    int i = atom_getFloatAtIndex (0, argc, argv);
    int n = (argc > 1) ? atom_getFloatAtIndex (1, argc, argv) : 1;
    
    if (liststore_parseIndexAndRange (x, &i, &n) == PD_ERROR_NONE) {
    //
    liststore_getProceed (x, i, n, (s == sym_cut) ? 1 : 0);
    //
    } else {
        outlet_list (x->x_outlet, 0, NULL);
    }
    //
    }
}

static void liststore_pop (t_liststore *x, t_symbol *s, int argc, t_atom *argv)
{
    int count = listinlet_listSize (&x->x_h.lh_listinlet);
    int n = argc ? (int)atom_getFloat (argv) : 1;
    
    if (n == 0) { n = count; }
    
    if (n > 0 && n <= count) { liststore_getProceed (x, count - n, n, 1); }
    else {
        outlet_list (x->x_outlet, 0, NULL);
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void *liststore_new (t_symbol *s, int argc, t_atom *argv)
{
    t_liststore *x = (t_liststore *)pd_new (liststore_class);
    
    listinlet_init (&x->x_h.lh_listinlet);
    listinlet_listSet (&x->x_h.lh_listinlet, argc, argv);
    
    x->x_outlet = outlet_newList (cast_object (x));
    
    inlet_new (cast_object (x), cast_pd (&x->x_h.lh_listinlet), NULL, NULL);
    
    return x;
}

static void liststore_free (t_liststore *x)
{
    listinlet_free (&x->x_h.lh_listinlet);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void liststore_setup (void)
{
    t_class *c = NULL;
    
    c = class_new (sym_list__space__store,
            (t_newmethod)liststore_new,
            (t_method)liststore_free,
            sizeof (t_liststore),
            CLASS_DEFAULT,
            A_GIMME,
            A_NULL);
            
    class_addList (c, (t_method)listappend_list);
    class_addAnything (c, (t_method)listappend_anything);
    
    class_addMethod (c, (t_method)liststore_clear,      sym_clear,      A_NULL);
    class_addMethod (c, (t_method)liststore_append,     sym_append,     A_GIMME, A_NULL);
    class_addMethod (c, (t_method)liststore_prepend,    sym_prepend,    A_GIMME, A_NULL);
    class_addMethod (c, (t_method)liststore_set,        sym_set,        A_GIMME, A_NULL);
    class_addMethod (c, (t_method)liststore_insert,     sym_insert,     A_GIMME, A_NULL);
    class_addMethod (c, (t_method)liststore_remove,     sym_remove,     A_GIMME, A_NULL);
    class_addMethod (c, (t_method)liststore_replace,    sym_replace,    A_GIMME, A_NULL);
    class_addMethod (c, (t_method)liststore_get,        sym_get,        A_GIMME, A_NULL);
    class_addMethod (c, (t_method)liststore_pop,        sym_pop,        A_GIMME, A_NULL);
    class_addMethod (c, (t_method)liststore_get,        sym_cut,        A_GIMME, A_NULL);

    class_addMethod (c, (t_method)listhelper_restore,   sym__restore,   A_GIMME, A_NULL);

    class_setDataFunction (c, listhelper_functionData);
    class_requirePending (c);
    
    class_setHelpName (c, &s_list);
    
    liststore_class = c;
}

void liststore_destroy (void)
{
    class_free (liststore_class);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

