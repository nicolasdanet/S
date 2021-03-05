
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#include "../../m_spaghettis.h"
#include "../../m_core.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "x_mica.hpp"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#if PD_WITH_BELLE

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

static t_class *micasequence_class;     /* Shared. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

typedef struct _micasequence {
    t_micabase  x_base;                 /* Must be the first. */
    t_outlet    *x_outlet;
    } t_micasequence;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void micasequence_bang (t_micasequence *x)
{
    mica::Concept t (concept_fetch (((t_micabase *)x)->x_tag));
    
    if (!t.isSequence()) { outlet_list (x->x_outlet, 0, NULL); }
    else {
    //
    int i, n = t.length();
    t_atom *a = NULL;
    
    PD_ATOMS_ALLOCA (a, n);
    
    for (i = 0; i < n; i++) {
        t_symbol *s = concept_tag (mica::item (t, i)); SET_SYMBOL (a + i, s);
    }
    
    outlet_list (x->x_outlet, n, a);
    
    PD_ATOMS_FREEA (a, n);
    //
    }
}

static void micasequence_symbol (t_micasequence *x, t_symbol *s)
{
    ((t_micabase *)x)->x_tag = s; micasequence_bang (x);
}

static void micasequence_list (t_micasequence *x, t_symbol *s, int argc, t_atom *argv)
{
    int i; for (i = 0; i < argc; i++) { micasequence_symbol (x, atom_getSymbolAtIndex (i, argc, argv)); }
}

static void micasequence_anything (t_micasequence *x, t_symbol *s, int argc, t_atom *argv)
{
    utils_anythingToList (cast_pd (x), (t_listmethod)micasequence_list, s, argc, argv);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void *micasequence_new (t_symbol *s, int argc, t_atom *argv)
{
    t_micasequence *x = (t_micasequence *)pd_new (micasequence_class);
    
    ((t_micabase *)x)->x_tag = concept_tag (mica::Undefined);
    x->x_outlet = outlet_newList (cast_object (x));
    
    if (argc) { warning_unusedArguments (s, argc, argv); }
    
    return x;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void micasequence_setup (void)
{
    t_class *c = NULL;
    
    c = class_new (sym_mica__space__sequence,
            (t_newmethod)micasequence_new,
            NULL,
            sizeof (t_micasequence),
            CLASS_DEFAULT,
            A_GIMME,
            A_NULL);
    
    class_addBang (c, (t_method)micasequence_bang);
    class_addSymbol (c, (t_method)micasequence_symbol);
    class_addList (c, (t_method)micasequence_list);
    class_addAnything (c, (t_method)micasequence_anything);
    
    class_addMethod (c, (t_method)micabase_restore, sym__restore, A_SYMBOL, A_NULL);

    class_setDataFunction (c, micabase_functionData);
    class_requirePending (c);
    
    class_setHelpName (c, sym_mica);
    
    micasequence_class = c;
}

PD_LOCAL void micasequence_destroy (void)
{
    class_free (micasequence_class);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#else

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void micasequence_setup (void)     { }
PD_LOCAL void micasequence_destroy (void)   { }

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#endif // PD_WITH_BELLE

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

