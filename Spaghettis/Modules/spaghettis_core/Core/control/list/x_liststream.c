
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

static t_class *liststream_class;           /* Shared. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

typedef struct _liststream {
    t_listinlethelper   x_h;                /* Must be the first. */
    t_float             x_stream;
    t_outlet            *x_outlet;
    } t_liststream;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void liststream_output (t_liststream *x)
{
    t_atom *t = NULL; int count = listinlet_listSize (&x->x_h.lh_listinlet);
    
    if (!count) { outlet_list (x->x_outlet, 0, NULL); }
    else {
    //
    PD_ATOMS_ALLOCA (t, count);

    listinlet_copyAtomsUnchecked (&x->x_h.lh_listinlet, t);
    outlet_list (x->x_outlet, count, t);
    
    PD_ATOMS_FREEA (t, count);
    //
    }
}

static void liststream_append (t_liststream *x, int argc, t_atom *argv)
{
    int i, n = PD_MAX (1, (int)x->x_stream);
    
    for (i = 0; i < argc; i++) {
    //
    listinlet_listAppend (&x->x_h.lh_listinlet, 1, argv + i);
    
    int count = listinlet_listSize (&x->x_h.lh_listinlet);
    
    if (count >= n) {
    //
    while (count > n) { count--;  listinlet_listRemove (&x->x_h.lh_listinlet, 0, 1); }
    
    liststream_output (x);
    //
    }
    //
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void liststream_bang (t_liststream *x)
{
    liststream_output (x);
}

static void liststream_float (t_liststream *x, t_float f)
{
    t_atom a; SET_FLOAT (&a, f); liststream_append (x, 1, &a);
}

static void liststream_symbol (t_liststream *x, t_symbol *s)
{
    t_atom a; SET_SYMBOL (&a, s); liststream_append (x, 1, &a);
}

static void liststream_list (t_liststream *x, t_symbol *s, int argc, t_atom *argv)
{
    liststream_append (x, argc, argv);
}

static void liststream_anything (t_liststream *x, t_symbol *s, int argc, t_atom *argv)
{
    utils_anythingToList (cast_pd (x), (t_listmethod)liststream_list, s, argc, argv);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void liststream_clear (t_liststream *x)
{
    listinlet_listClear (&x->x_h.lh_listinlet);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static t_buffer *liststream_functionData (t_object *z, int flags)
{
    if (SAVED_DEEP (flags)) {
    //
    t_liststream *x = (t_liststream *)z;
    t_buffer *b = listhelper_functionData (z, flags);
    
    buffer_appendFloat (b, x->x_stream);

    return b;
    //
    }
    
    return NULL;
}

static void liststream_restore (t_liststream *x, t_symbol *s, int argc, t_atom *argv)
{
    if (argc) {
    //
    t_liststream *old = (t_liststream *)instance_pendingFetch (cast_object (x));

    x->x_stream = old ? old->x_stream : atom_getFloatAtIndex (argc - 1, argc, argv);
    
    argc--; listhelper_restore ((t_listinlethelper *)x, s, argc, argv);
    //
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void *liststream_new (t_symbol *s, int argc, t_atom *argv)
{
    t_liststream *x = (t_liststream *)pd_new (liststream_class);
    
    listinlet_init (&x->x_h.lh_listinlet);
    
    if (argc) { x->x_stream = atom_getFloat (argv); argc--; argv++; }
    
    x->x_outlet = outlet_newList (cast_object (x));
    
    inlet_newFloat (cast_object (x), &x->x_stream);
    
    if (argc) { warning_unusedArguments (cast_object (x), s, argc, argv); }
    
    return x;
}

static void liststream_free (t_liststream *x)
{
    listinlet_free (&x->x_h.lh_listinlet);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void liststream_setup (void)
{
    t_class *c = NULL;
    
    c = class_new (sym_list__space__stream,
            (t_newmethod)liststream_new,
            (t_method)liststream_free,
            sizeof (t_liststream),
            CLASS_DEFAULT,
            A_GIMME,
            A_NULL);
    
    class_addBang (c, (t_method)liststream_bang);
    class_addFloat (c, (t_method)liststream_float);
    class_addSymbol (c, (t_method)liststream_symbol);
    class_addList (c, (t_method)liststream_list);
    class_addAnything (c, (t_method)liststream_anything);
    
    class_addMethod (c, (t_method)liststream_clear,      sym_clear,      A_NULL);
    class_addMethod (c, (t_method)liststream_restore,    sym__restore,   A_GIMME, A_NULL);

    class_setDataFunction (c, liststream_functionData);
    class_requirePending (c);
    
    class_setHelpName (c, &s_list);
    
    liststream_class = c;
}

void liststream_destroy (void)
{
    class_free (liststream_class);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

