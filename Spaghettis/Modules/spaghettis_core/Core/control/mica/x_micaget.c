
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

static t_class *micaget_class;          /* Shared. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

typedef struct _micaget {
    t_micabase  x_base;                 /* Must be the first. */
    t_buffer    *x_cache;
    t_outlet    *x_outlet;
    } t_micaget;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void micaget_bang (t_micaget *x)
{
    mica::Concept t (concept_fetch (((t_micabase *)x)->x_tag));
    
    buffer_clear (x->x_cache);
    
    if (t.isInteger())     { buffer_appendFloat (x->x_cache, t.getNumerator()); }
    else if (t.isNumber()) {
        buffer_appendFloat (x->x_cache, t.getNumerator());
        buffer_appendFloat (x->x_cache, t.getDenominator());
    } else {
        std::string s = t.toString();
        buffer_withStringUnzeroed (x->x_cache, s.c_str(), (int)s.length());
    }
    
    outlet_list (x->x_outlet, buffer_getSize (x->x_cache), buffer_getAtoms (x->x_cache));
}

static void micaget_symbol (t_micaget *x, t_symbol *s)
{
    ((t_micabase *)x)->x_tag = s; micaget_bang (x);
}

static void micaget_list (t_micaget *x, t_symbol *s, int argc, t_atom *argv)
{
    int i; for (i = 0; i < argc; i++) { micaget_symbol (x, atom_getSymbolAtIndex (i, argc, argv)); }
}

static void micaget_anything (t_micaget *x, t_symbol *s, int argc, t_atom *argv)
{
    utils_anythingToList (cast_pd (x), (t_listmethod)micaget_list, s, argc, argv);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void *micaget_new (t_symbol *s, int argc, t_atom *argv)
{
    t_micaget *x = (t_micaget *)pd_new (micaget_class);
    
    ((t_micabase *)x)->x_tag = concept_tag (mica::Undefined);
    x->x_cache  = buffer_new();
    x->x_outlet = outlet_newList (cast_object (x));
    
    if (argc) { warning_unusedArguments (s, argc, argv); }
    
    return x;
}

static void micaget_free (t_micaget *x)
{
    buffer_free (x->x_cache);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void micaget_setup (void)
{
    t_class *c = NULL;
    
    c = class_new (sym_mica__space__get,
            (t_newmethod)micaget_new,
            (t_method)micaget_free,
            sizeof (t_micaget),
            CLASS_DEFAULT,
            A_GIMME,
            A_NULL);
    
    class_addBang (c, (t_method)micaget_bang);
    class_addSymbol (c, (t_method)micaget_symbol);
    class_addList (c, (t_method)micaget_list);
    class_addAnything (c, (t_method)micaget_anything);
    
    class_addMethod (c, (t_method)micabase_restore, sym__restore, A_SYMBOL, A_NULL);

    class_setDataFunction (c, micabase_functionData);
    class_requirePending (c);
    
    class_setHelpName (c, sym_mica);
    
    micaget_class = c;
}

PD_LOCAL void micaget_destroy (void)
{
    class_free (micaget_class);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#else

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void micaget_setup (void)      { }
PD_LOCAL void micaget_destroy (void)    { }

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#endif // PD_WITH_BELLE

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#endif // __clang_analyzer__

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
