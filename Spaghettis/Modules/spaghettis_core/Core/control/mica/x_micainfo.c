
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#include "../../m_spaghettis.h"
#include "../../m_core.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

/* LLVM analyzer seems to report false positives. */

#ifndef __clang_analyzer__

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "x_mica.hpp"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#if PD_WITH_BELLE

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

static t_class *micainfo_class;          /* Shared. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

typedef struct _micainfo {
    t_micabase  x_base;                 /* Must be the first. */
    t_outlet    *x_outlet;
    } t_micainfo;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void micainfo_out (t_micainfo *x, t_symbol *s, int n)
{
    t_atom a; SET_FLOAT (&a, n); outlet_anything (x->x_outlet, s, 1, &a);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void micainfo_bang (t_micainfo *x)
{
    mica::Concept t (concept_fetch (((t_micabase *)x)->x_tag));
    
    micainfo_out (x, sym_undefined,     t.isUndefined());
    micainfo_out (x, sym_number,        t.isNumber());
    micainfo_out (x, sym_integer,       t.isInteger());
    micainfo_out (x, sym_numerator,     t.getNumerator());
    micainfo_out (x, sym_denominator,   t.getDenominator());
    micainfo_out (x, sym_sequence,      t.isSequence());
    micainfo_out (x, sym_cyclic,        t.isCyclic());
    micainfo_out (x, sym_length,        t.length());
}

static void micainfo_symbol (t_micainfo *x, t_symbol *s)
{
    ((t_micabase *)x)->x_tag = s; micainfo_bang (x);
}

static void micainfo_list (t_micainfo *x, t_symbol *s, int argc, t_atom *argv)
{
    int i; for (i = 0; i < argc; i++) { micainfo_symbol (x, atom_getSymbolAtIndex (i, argc, argv)); }
}

static void micainfo_anything (t_micainfo *x, t_symbol *s, int argc, t_atom *argv)
{
    utils_anythingToList (cast_pd (x), (t_listmethod)micainfo_list, s, argc, argv);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void *micainfo_new (t_symbol *s, int argc, t_atom *argv)
{
    t_micainfo *x = (t_micainfo *)pd_new (micainfo_class);
    
    ((t_micabase *)x)->x_tag = concept_tag (mica::Undefined);
    x->x_outlet = outlet_newAnything (cast_object (x));
    
    if (argc) { warning_unusedArguments (s, argc, argv); }
    
    return x;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void micainfo_setup (void)
{
    t_class *c = NULL;
    
    c = class_new (sym_mica__space__info,
            (t_newmethod)micainfo_new,
            NULL,
            sizeof (t_micainfo),
            CLASS_DEFAULT,
            A_GIMME,
            A_NULL);
    
    class_addBang (c, (t_method)micainfo_bang);
    class_addSymbol (c, (t_method)micainfo_symbol);
    class_addList (c, (t_method)micainfo_list);
    class_addAnything (c, (t_method)micainfo_anything);
    
    class_addMethod (c, (t_method)micabase_restore, sym__restore, A_SYMBOL, A_NULL);

    class_setDataFunction (c, micabase_functionData);
    class_requirePending (c);
    
    class_setHelpName (c, sym_mica);
    
    micainfo_class = c;
}

PD_LOCAL void micainfo_destroy (void)
{
    class_free (micainfo_class);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#else

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void micainfo_setup (void)     { }
PD_LOCAL void micainfo_destroy (void)   { }

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#endif // PD_WITH_BELLE

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#endif // __clang_analyzer__

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
